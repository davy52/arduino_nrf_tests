#include "i2c_master.h"
#include "i2c_internal_types.h"

#define __at(addr)  __attribute__((address (addr)))

// TWI Bit Rade Register
volatile static i2c_TWBR_t i2c_bitrate __at(0xB8);

// TWI Control Register
volatile static i2c_TWCR_t i2c_control __at(0xBC);

// TWI Status Register 
volatile static i2c_TWSR_t i2c_status __at(0xB9);

// TWI Data Register
volatile static i2c_TWDR_t i2c_data __at(0xBB);


// static functions
static i2c_error_t i2c_clockSetup(uint32_t cpu_clock, uint32_t scl_freq, uint8_t* bit_rate, uint8_t* prescaler)
{
    i2c_error_t ret_val = I2C_ERR_NOT_OK;
    
    const float clock_err = 0.05;
    uint32_t scl_real = 0;
    uint8_t temp_bitrate = 0;
    uint8_t temp_prescaler = 0;

    // prescalers               4       16      64
    const uint8_t pre_arr[] = { 0x01,   0x02,   0x03};
    
    // try each prescaler
    for(uint8_t i = 0; i < 3; i++){
        temp_prescaler = pre_arr[i];
        temp_bitrate = (uint32_t)((((cpu_clock * temp_prescaler) / scl_freq) - 16) << 1);
        scl_real = (uint32_t)(cpu_clock/(16 + 2 * temp_bitrate * temp_prescaler));
        // if calculated clock if within limits return succes else check the next prescaler
        if(((scl_real - scl_freq) & 0x7F) <= scl_freq * clock_err){
            (*prescaler) = temp_prescaler;
            (*bit_rate) = temp_bitrate;
            ret_val = I2C_ERR_OK;
            return ret_val;
        }
    }

    // return I2C_ERR_NOT_OK if values not found
    return ret_val;
}




void i2c_master_init(i2c_settings_t settings)
{
    TWCR |= settings.ack_enable ? (1u << TWEA) : 0;
    TWCR |= settings.int_enable ? (1u << TWIE) : 0;
    TWAR |= settings.general_call_enable ? (1u << TWGCE) : 0;
    
}

void i2c_master_sendTest()
{
    TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while ( !(TWCR & (1<<TWINT)) );

    // if (TWSR & 0xF8 == 0x08){
    //     return;
    // }
    
    TWDR = 0x55;        // slave addr + write bit
    TWCR = (1<<TWINT) | (1<<TWEN);

    while (! (TWCR & (1 << TWINT)));

    //if ((TWSR & 0xF8) != MT_SLA_ACK)

    TWDR = 0x85;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while (!(TWCR & (1<<TWINT)));

    //if ((TWSR & 0xF8)!= MT_DATA_ACK)

    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    
}
void i2c_master_sendData(uint8_t *data, uint8_t data_size)
{
}

void i2c_master_resend(void)
{
}

i2c_error_t i2c_master_isBusy(void)
{
    return i2c_error_t();
}

i2c_error_t i2c_master_getState(void)
{
    return i2c_error_t();
}

i2c_error_t i2c_master_readData(uint8_t *data, uint8_t data_size)
{
    return i2c_error_t();
}

ISR(TWI_vect)
{
}