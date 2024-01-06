#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include "stdint-gcc.h"
#include "avr/interrupt.h"

#include "i2c_common.h"

// its hal already but...


/**
 * @brief TWI modeule init 
 * 
 * @param settings 
 * @return i2c_error_t 
 */
i2c_error_t i2c_master_init(uint32_t f_cpu, uint32_t baudrate, uint8_t transmission_buffer);

/**
 * @brief change size of transmission buffer
 * 
 * @param new_size 
 * @return i2c_error_t 
 */
i2c_error_t i2c_changeBufferSize(uint8_t new_size);

/**
 * @brief send data to adder
 * 
 * @param adder 
 * @param data 
 * @param data_size 
 * @return i2c_error_t 
 */
i2c_error_t i2c_master_sendData(uint8_t adder, uint8_t *data, uint8_t data_size);

/**
 * @brief read data from slave
 * 
 * @param data pointer to array for data
 * @param data_size size of data to be read (in bytes)
 * @return i2c_error_t 
 */
i2c_error_t i2c_master_readData(uint8_t adder, uint8_t *data, uint8_t data_size);

/**
 * @brief Get message that was read 
 *  use after i2c_master_readData()
 * 
 * @param data pointer to array where message should be copied to 
 * @param arr_size size of array
 * @return i2c_error_t 
 */
i2c_error_t i2c_master_getMessage(uint8_t *data, uint8_t arr_size);


#endif /* I2C_MASTER_H */
