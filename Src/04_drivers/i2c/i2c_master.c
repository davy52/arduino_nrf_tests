#include "i2c_master.h"
#include "i2c_internal_types.h"

#include <stdlib.h>

#include "debug.h"


#define __at(addr)  __attribute__((address (addr)))

typedef enum __i2c_states {
    I2C_STATE_IDLE,

    I2C_STATE_START,

    I2C_STATE_SLA_WRITE,
    I2C_STATE_WRITE,

    I2C_STATE_SLA_READ,
    I2C_STATE_READ,

    I2C_STATE_STOP,

    I2C_STATE_ERROR
} i2c_states_t;

typedef enum __i2c_status {
    I2C_STATUS_START_DONE       = 0x08,
    I2C_STATUS_RSTART_DONE      = 0x10,
    // Master Transmit
    I2C_STATUS_SLAW_ACK         = 0x18,
    I2C_STATUS_SLAW_NOACK       = 0x20,
    I2C_STATUS_BYTE_SENT_ACK    = 0x28,
    I2C_STATUS_BYTE_SENT_NOACK  = 0x30,
    I2C_STATUS_ARBI_LOST_IN_W   = 0x38,
    // Master Receive
    I2C_STATUS_ARBI_LOST_IN_R   = 0x38,
    I2C_STATUS_SLAR_ACK         = 0x40,
    I2C_STATUS_SLAR_NOACK       = 0x48,
    I2C_STATUS_BYTE_READ_ACK    = 0x50,
    I2C_STATUS_BYTE_READ_NOACK  = 0x58
} i2c_master_status_t;

typedef enum __i2c_rw_bit {
    I2C_RW_WRITE = 0x00,
    I2C_RW_READ = 0x01
} i2c_rw_bit_t;

// TWI Bit Rade Register
volatile static i2c_TWBR_t i2c_bitrate __at(0xB8);

// TWI Control Register
volatile static i2c_TWCR_t i2c_control __at(0xBC);

// TWI Status Register 
volatile static i2c_TWSR_t i2c_status __at(0xB9);

// TWI Data Register
volatile static i2c_TWDR_t i2c_data __at(0xBB);

volatile static uint8_t* i2c_data_buff;
volatile static uint8_t i2c_data_buff_size;
volatile static uint8_t i2c_data_size;
volatile static uint8_t i2c_data_buff_iter;
volatile static uint8_t i2c_adder;
volatile static uint8_t i2c_no_noack;
static const uint8_t I2C_MAX_NO_NOACK = 1;

static i2c_states_t i2c_state;


// static functions
// FIXME: !!! dont know what is wrong prescaler gets set to 00
static i2c_error_t i2c_clockSetup(uint32_t cpu_clock, uint32_t scl_freq, uint8_t* bit_rate, uint8_t* prescaler)
{
    i2c_error_t ret_val = I2C_ERR_NOT_OK;
    
    const float clock_err = 0.05;
    uint32_t scl_real = 0;
    uint8_t temp_bitrate = 0;
    uint8_t temp_prescaler = 0;

    // prescalers               4       16      64
    const uint8_t pre_arr[] = { 0x01,   0x02,   0x03};
    const uint8_t pre_real[] = {4,      16,     64};
    
    // try each prescaler
    for(uint8_t i = 0; i < 3; i++){
        temp_prescaler = pre_real[i];
        temp_bitrate = (uint8_t)((cpu_clock - (16 * scl_freq)) / 2 * temp_prescaler * scl_freq);
        scl_real = (uint32_t)(cpu_clock/(16 + (2 * temp_bitrate * temp_prescaler)));
        // if calculated clock if within limits return succes else check the next prescaler
        if(scl_real <= scl_freq + scl_freq * clock_err || scl_real >= scl_freq - scl_freq * clock_err){
            (*prescaler) = pre_arr[i];
            (*bit_rate) = temp_bitrate;
            ret_val = I2C_ERR_OK;
            return ret_val;
        }
    }

    // return I2C_ERR_NOT_OK if values not found
    return ret_val;
}

static void i2c_sendStart()
{
    i2c_control.int_en = 1;

    i2c_control.reg = 0b10100101;
}

static void i2c_sendStop()
{
    i2c_control.int_en = 0;

    i2c_control.write_start_bit = 0;
    i2c_control.write_stop_bit = 1;
    i2c_control.int_flag = 1;

}

static void i2c_sendAdder()
{
    if ((i2c_adder & 0x1) == I2C_RW_READ)
    {
        i2c_state = I2C_STATE_READ;
    } else if ((i2c_adder & 0x1) == I2C_RW_WRITE){
        i2c_state = I2C_STATE_WRITE;
    }
    
    i2c_data = i2c_adder;
    // i2c_data = 0x54 | I2C_RW_WRITE;
    i2c_control.reg = 0b10000101;
}

static void i2c_writeByte()
{
    i2c_data = i2c_data_buff[i2c_data_buff_iter];
    i2c_control.write_stop_bit = 0;
    i2c_control.write_start_bit = 0;
    i2c_control.int_flag = 1;
}

static void i2c_readByte()
{
    i2c_data_buff[i2c_data_buff_iter] = i2c_data;
}

static void i2c_sendAck()
{
    i2c_control.write_start_bit = 0;
    i2c_control.write_stop_bit = 0;
    i2c_control.ack_en = 1;
    i2c_control.int_flag = 1;
}

static void i2c_sendNoAck()
{
    i2c_control.write_start_bit = 0;
    i2c_control.write_stop_bit = 0;
    i2c_control.ack_en = 0;
    i2c_control.int_flag = 1;
}



i2c_error_t i2c_master_init(uint32_t f_cpu, uint32_t baudrate, uint8_t transmission_buffer)
{
    i2c_error_t ret_val = I2C_ERR_OK;

    uint8_t bitrate = 0;
    uint8_t prescaler = 0;
    
    ret_val = i2c_clockSetup(f_cpu, baudrate, &bitrate, &prescaler);
    if(ret_val == I2C_ERR_NOT_OK){
        return ret_val;
    }
    
    i2c_control.i2c_en = 1;
    i2c_control.int_en = 1;
    // FIXME: i2c_clockSetup ERROR
    // i2c_bitrate = bitrate;
    // i2c_status.prescaler = prescaler;
    i2c_bitrate = 48;
    i2c_status.prescaler = 0x01;

    
    i2c_data_buff = (uint8_t*)malloc(sizeof(uint8_t) * transmission_buffer);
    if(i2c_data_buff == NULL){
        ret_val = I2C_ERR_NOT_OK;
        return ret_val;
    }
    
    i2c_data_buff_size = transmission_buffer;
    

   return ret_val; 
}

i2c_error_t i2c_changeBufferSize(uint8_t new_size)
{
    i2c_error_t ret_val = I2C_ERR_OK;
    
    i2c_data_buff = (uint8_t*)realloc(i2c_data_buff, sizeof(uint8_t) * new_size);
    
    if(i2c_data_buff == NULL){
        ret_val = I2C_ERR_NOT_OK;
        return ret_val;
    }
    
    return ret_val;
}

i2c_error_t i2c_master_sendData(uint8_t adder, uint8_t *data, uint8_t data_size)
{
    i2c_error_t ret_val = I2C_ERR_OK;
    
    if((i2c_state != I2C_STATE_IDLE) && (i2c_state != I2C_STATE_ERROR)){
        ret_val = I2C_ERR_BUSY;
        return ret_val;
    }
    
    if(data_size > i2c_data_buff_size){
        ret_val = I2C_ERR_BUFF_TOO_SMALL;
        return ret_val;
    }

    for(int i = 0; i < data_size; i++){
        i2c_data_buff[i] = data[i];
    }
    
    i2c_data_size = data_size;
    i2c_data_buff_iter = 0;
    i2c_no_noack = 0;

    i2c_adder = adder | I2C_RW_WRITE;

    i2c_state = I2C_STATE_START;
    i2c_sendStart();

    return ret_val;
}

i2c_error_t i2c_master_readData(uint8_t adder, uint8_t *data, uint8_t data_size)
{
    i2c_error_t ret_val = I2C_ERR_OK;

    if(i2c_state != I2C_STATE_IDLE && i2c_state != I2C_STATE_ERROR){
        ret_val = I2C_ERR_BUSY;
        return ret_val;
    }
    
    if(data_size > i2c_data_buff_size){
        ret_val = I2C_ERR_BUFF_TOO_SMALL;
        return ret_val;
    }

    for(int i = 0; i < data_size; i++){
        i2c_data_buff[i] = data[i];
    }
    
    i2c_data_size = data_size;
    i2c_data_buff_iter = 0;

    i2c_adder = adder | I2C_RW_READ;
    
    i2c_state = I2C_STATE_START;
    i2c_sendStart();

    return ret_val;
}

i2c_error_t i2c_master_getMessage(uint8_t *data, uint8_t arr_size)
{
    i2c_error_t ret_val = I2C_ERR_OK;
    
    if(i2c_state != I2C_STATE_IDLE && i2c_state != I2C_STATE_ERROR){
        ret_val = I2C_ERR_BUSY;
        return ret_val;
    }
    
    if(i2c_state == I2C_STATE_ERROR){
        ret_val = I2C_ERR_NOT_OK;
        return ret_val;
    }
    
    if(i2c_data_buff_iter == 0){
        ret_val = I2C_ERR_NO_READ;
        return ret_val;
    }
    
    if(arr_size < i2c_data_size){
        ret_val = I2C_ERR_ARR_TOO_SMALL;
        return ret_val;
    }
    
    for(int i = 0; i < i2c_data_size; i++){
        data[i] = i2c_data_buff[i];
    }

    return ret_val;
}


ISR(TWI_vect)
{
    blink_slow(1);

    switch (i2c_status.reg & (~0x07))
    {
    case I2C_STATUS_START_DONE:
        switch (i2c_state)
        {
        case I2C_STATE_START:
            i2c_sendAdder();
            break;
        
        case I2C_STATE_WRITE:
            i2c_writeByte();
            break;
            
        case I2C_STATE_READ:
            i2c_readByte();
            break;
        
        default:
            break;
        }
        break;

    case I2C_STATUS_SLAW_ACK:
        i2c_no_noack = 0;
        i2c_state = I2C_STATE_WRITE;
        i2c_writeByte();
        break;

    case I2C_STATUS_SLAW_NOACK:
        i2c_no_noack++;
        if(i2c_no_noack > I2C_MAX_NO_NOACK){
            i2c_state = I2C_STATE_ERROR;
            i2c_sendStop();
            break;
        }
        i2c_sendStart();
        break;
    
    case I2C_STATUS_BYTE_SENT_ACK:
        i2c_no_noack = 0;
        i2c_data_buff_iter++;
        // check if there is more data
        if(i2c_data_buff_iter >= i2c_data_size){
            i2c_state = I2C_STATE_IDLE;
            i2c_data_buff_iter = 0;
            i2c_sendStop();
        }else{
            i2c_writeByte();
        }
        break;
        
    case I2C_STATUS_BYTE_SENT_NOACK:
        i2c_no_noack++;
        if(i2c_no_noack > I2C_MAX_NO_NOACK){
            i2c_state = I2C_STATE_ERROR;
            i2c_sendStop();
            break;
        }
        i2c_sendStart();
        break;

    case I2C_STATUS_ARBI_LOST_IN_W:
    // case I2C_STATUS_ARBI_LOST_IN_R: same val
        i2c_no_noack++;
        if(i2c_no_noack > I2C_MAX_NO_NOACK){
            i2c_state = I2C_STATE_ERROR;
            i2c_sendStop();
            break;
        }
        i2c_sendStart();
        break;
        
    case I2C_STATUS_SLAR_ACK:
        i2c_no_noack = 0;
        if(i2c_data_size > 2){
           i2c_sendAck();
        } else {
            i2c_sendNoAck();
        } 
        break;
        
    case I2C_STATUS_SLAR_NOACK:
        i2c_no_noack++;
        if(i2c_no_noack > I2C_MAX_NO_NOACK){
            i2c_state = I2C_STATE_ERROR;
            i2c_sendStop();
            break;
        }
        i2c_sendStart();
        break;
        
    case I2C_STATUS_BYTE_READ_ACK:
        i2c_readByte();
        i2c_data_buff_iter++;
        
        if(i2c_data_buff_iter == i2c_data_size - 1){
            i2c_sendNoAck();
            break;
        } else if(i2c_data_buff_iter >= i2c_data_size){
            i2c_state = I2C_STATE_IDLE;
            break;
        } else {
            i2c_sendAck();
            break;
        }
        break;
    }

    blink_slow(0);
}