#include "i2c_master.h"

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
/*
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
*/