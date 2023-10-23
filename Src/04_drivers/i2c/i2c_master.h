#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include "stdint-gcc.h"
#include "avr/interrupt.h"

#include "i2c_common.h"


typedef struct {
    uint8_t sda_PULLUP : 1;
    uint8_t scl_PULLUP : 1;
    uint8_t ack_enable : 1;
    uint8_t int_enable : 1;
    uint8_t general_call_enable;
} i2c_settings_t;

/**
 * @brief I2C Master init - first function to be called before using i2c
 * 
 */
void i2c_master_init(i2c_settings_t settings);

/**
 * @brief send data to slave
 * 
 * @param data 
 * @param data_size 
 */
void i2c_master_sendData(uint8_t *data, uint8_t data_size);

/**
 * @brief resend last message
 * 
 */
void i2c_master_resend(void);

/**
 * @brief check if i2c module is busy
 * 
 * @return uint8_t 
 */
i2c_error_t i2c_master_isBusy(void);

/**
 * @brief get module state
 * 
 * @return i2c_error_t { I2C_OK, I2C_NOT_OK, I2C_BUSY }
 */
i2c_error_t i2c_master_getState(void);

/**
 * @brief read data from slave
 * 
 * @param data pointer to array for data
 * @param data_size size of data to be read (in bytes)
 * @return i2c_error_t 
 */
i2c_error_t i2c_master_readData(uint8_t *data, uint8_t data_size);

/**
 * @brief Construct a new ISR object for i2c interrupts\
 *      to be implemented by user
 * 
 */
ISR(TWI_vect);



void i2c_master_sendTest();

#endif /* I2C_MASTER_H */
