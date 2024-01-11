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
i2c_error_t i2c_master_init(uint32_t f_cpu, uint32_t baudrate);

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
i2c_error_t i2c_master_sendData(i2c_job_t* job);

i2c_error_t i2c_master_appendJob(i2c_job_t* job);

i2c_error_t i2c_master_startTransaction();


#endif /* I2C_MASTER_H */
