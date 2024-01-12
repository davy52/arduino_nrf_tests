#include "i2c_master.h"
#include "i2c_internal_types.h"

#include <stdlib.h>


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

// TWI Bit Rade Register
volatile static i2c_TWBR_t i2c_bitrate __at(0xB8);

// TWI Control Register
volatile static i2c_TWCR_t i2c_control __at(0xBC);

// TWI Status Register 
volatile static i2c_TWSR_t i2c_status __at(0xB9);

// TWI Data Register
volatile static i2c_TWDR_t i2c_data __at(0xBB);

volatile static uint8_t i2c_no_noack;
static const uint8_t I2C_MAX_NO_NOACK = 1;

volatile static i2c_job_t* i2c_job_list;
volatile static uint8_t i2c_job_iter;


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
    i2c_job_iter = 0;

    i2c_control.reg = 0b10100101;
}

#define i2c_handleNextJob();                                \
 do{                                                          \
    i2c_job_list->status = I2C_ERR_OK;                      \
    if(i2c_job_list->__next != NULL){                       \
        if(i2c_job_list->repeated_start != 0){              \
            i2c_job_list = i2c_job_list->__next;            \
            i2c_job_iter = 0;                               \
            i2c_start();                                    \
        }                                                   \
        else{                                               \
            i2c_job_list = i2c_job_list->__next;            \
            i2c_job_iter = 0;                               \
            i2c_stopStart();                                \
        }                                                   \
    }                                                       \
    else {                                                  \
        i2c_job_list = i2c_job_list->__next;                \
        i2c_job_iter = 0;                                   \
        i2c_control.int_en = 0;                             \
        i2c_stop();                                         \
    }                                                       \
 }while(0);                                                   \

#define i2c_start();                            \
    do{                                         \
        i2c_control.write_start_bit     = 1;    \
        i2c_control.write_stop_bit      = 0;    \
        i2c_control.ack_en              = 0;    \
        i2c_control.int_flag            = 1;    \
    } while(0);

#define i2c_stop();                             \
    do{                                         \
        i2c_control.write_stop_bit      = 1;    \
        i2c_control.write_start_bit     = 0;    \
        i2c_control.ack_en              = 0;    \
        i2c_control.int_flag            = 1;    \
    } while(0);

#define i2c_stopStart();                        \
    do{                                         \
        i2c_control.write_stop_bit      = 1;    \
        i2c_control.write_start_bit     = 1;    \
        i2c_control.ack_en              = 0;    \
        i2c_control.int_flag            = 1;    \
    } while(0);

#define i2c_bits_data();                        \
    do{                                         \
        i2c_control.write_stop_bit      = 0;    \
        i2c_control.write_start_bit     = 0;    \
        i2c_control.ack_en              = 0;    \
        i2c_control.int_flag            = 1;    \
    } while(0);

#define i2c_ack();                              \
    do{                                         \
        i2c_control.write_stop_bit      = 0;    \
        i2c_control.write_start_bit     = 0;    \
        i2c_control.ack_en              = 1;    \
        i2c_control.int_flag            = 1;    \
    } while(0);

#define i2c_noack();                            \
    do{                                         \
        i2c_control.write_stop_bit      = 0;    \
        i2c_control.write_start_bit     = 0;    \
        i2c_control.ack_en              = 0;    \
        i2c_control.int_flag            = 1;    \
    } while(0);


i2c_error_t i2c_master_init(uint32_t f_cpu, uint32_t baudrate)
{
    i2c_error_t ret_val = I2C_ERR_OK;

    uint8_t bitrate = 0;
    uint8_t prescaler = 0;
    
    ret_val = i2c_clockSetup(f_cpu, baudrate, &bitrate, &prescaler);
    if(ret_val == I2C_ERR_NOT_OK){
        return ret_val;
    }
    
    i2c_control.i2c_en = 1;
    i2c_control.int_en = 0;
    // FIXME: i2c_clockSetup ERROR
    // i2c_bitrate = bitrate;
    // i2c_status.prescaler = prescaler;
    i2c_bitrate = 48;
    i2c_status.prescaler = 0x01;
    
    i2c_job_iter = 0;

   return ret_val; 
}

i2c_error_t i2c_master_sendData(i2c_job_t* job)
{
    i2c_error_t ret_val = I2C_ERR_OK;
    
    if(job->RW == I2C_RW_WRITE){
        job->adder = (job->adder & 0xFE) | I2C_RW_WRITE;
    } 
    else{
        job->adder = (job->adder & 0xFE) | I2C_RW_READ;
    }
    job->status = I2C_ERR_BUSY;
    
    if(i2c_job_list == NULL){
        i2c_job_list = job;
        i2c_sendStart();
    }
    else{
        i2c_job_t* job_iter = i2c_job_list;
        while(job_iter->__next != NULL){
            job_iter = job_iter->__next;
        }
        job_iter->__next = job;
    }
    
    return ret_val;
}

i2c_error_t i2c_master_appendJob(i2c_job_t* job)
{
    i2c_error_t ret_val = I2C_ERR_OK;
    
    if(i2c_control.int_en == 1){
        ret_val = I2C_ERR_BUSY;
        return ret_val;
    }
    
    if(job->RW == I2C_RW_WRITE){
        job->adder = (job->adder & 0xFE) | I2C_RW_WRITE;
    } 
    else{
        job->adder = (job->adder & 0xFE) | I2C_RW_READ;
    }
    job->status = I2C_ERR_BUSY;
    
    if(i2c_job_list == NULL){
        i2c_job_list = job;
    }
    else{
        i2c_job_t* job_ptr = i2c_job_list;
        while(job_ptr->__next != NULL){
            job_ptr = job_ptr->__next;
        }
        job_ptr->__next = job;
    }
    
    return ret_val;
}

i2c_error_t i2c_master_startTransaction()
{
    i2c_error_t ret_val = I2C_ERR_OK;

    if(i2c_job_list == NULL){
        ret_val = I2C_ERR_NOT_OK;
        return ret_val;
    }

    if(i2c_control.int_en == 1){
        ret_val = I2C_ERR_NOT_OK;
        return ret_val;
    }

    i2c_sendStart();
    return ret_val;
}

// FIXME: handle error cases - noack on write before end
ISR(TWI_vect, ISR_BLOCK)
{
    if(i2c_job_list == NULL){
        i2c_control.int_en = 0;
        return;
    }

    switch (i2c_status.reg & (~0x07))
    {
    case I2C_STATUS_START_DONE:
    case I2C_STATUS_RSTART_DONE:
        i2c_data = i2c_job_list->adder;
        i2c_bits_data();
        break;

    case I2C_STATUS_SLAW_ACK:
        if(i2c_job_list->data_len - i2c_job_iter < 1){
            if(i2c_job_list->repeated_start){
                i2c_start();
            }
            else{
                i2c_stop();
            }
        }
        else{
            i2c_data = i2c_job_list->data[i2c_job_iter];
            i2c_bits_data();
        }
        break;

    case I2C_STATUS_SLAW_NOACK:
        i2c_start();
        break;
    
    case I2C_STATUS_BYTE_SENT_ACK:
        i2c_job_iter = i2c_job_iter + 1;
        if(i2c_job_list->data_len - i2c_job_iter < 1){
            i2c_handleNextJob();
        }
        else {
            i2c_data = i2c_job_list->data[i2c_job_iter];
            i2c_bits_data();
        }
        break;
        
    case I2C_STATUS_BYTE_SENT_NOACK:
        i2c_handleNextJob();
        break;

    case I2C_STATUS_ARBI_LOST_IN_W:
            break;      // FIXME: case should be handled

    /*
    case I2C_STATUS_ARBI_LOST_IN_R:
        break;      // FIXME: case should be handled
    */

    case I2C_STATUS_SLAR_NOACK:
        i2c_handleNextJob();
        break;

    case I2C_STATUS_SLAR_ACK:
        if(i2c_job_list->data_len - i2c_job_iter < 1){
            i2c_handleNextJob();
        }
        else if(i2c_job_list->data_len - i2c_job_iter == 1){
            i2c_noack();
        }
        else {
            i2c_ack();
        }
        break;
            
    case I2C_STATUS_BYTE_READ_ACK:
    case I2C_STATUS_BYTE_READ_NOACK:
        i2c_job_list->data[i2c_job_iter] = i2c_data;
        i2c_job_iter = i2c_job_iter + 1;
        if(i2c_job_list->data_len - i2c_job_iter < 1){
            i2c_handleNextJob();
        }
        else if(i2c_job_list->data_len - i2c_job_iter == 1){
            i2c_noack();
        }
        else {
            i2c_ack();
        }
        break;

    default:            // should not happen
        break;
    }
}