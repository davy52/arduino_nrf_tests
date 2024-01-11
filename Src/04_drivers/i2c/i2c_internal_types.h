#ifndef I2C_INTERNAL_TYPES_H
#define I2C_INTERNAL_TYPES_H

#include <stdint.h>

#define __packed    __attribute__((packed))

// TWI Bit Rate Register
typedef uint8_t i2c_TWBR_t;

// TWI Control Register
typedef union __i2c_TWCR {
    uint8_t reg;
    struct {
        uint8_t int_en          : 1;
        uint8_t                 : 1;
        uint8_t i2c_en          : 1;
        uint8_t write_col_flag  : 1;
        uint8_t write_stop_bit  : 1;
        uint8_t write_start_bit : 1;
        uint8_t ack_en          : 1;
        uint8_t int_flag        : 1;
    } __packed;
} i2c_TWCR_t;

// TWI Status Register
typedef union __i2c_TWSR {
    uint8_t reg;
    struct {
        uint8_t prescaler   : 2;
        uint8_t             : 1;
        uint8_t status      : 5;
    } __packed;
} i2c_TWSR_t;

// TWI Data Register
typedef uint8_t i2c_TWDR_t;

// TWI Slave Address Register
typedef union __i2c_TWAR {
    uint8_t reg;
    struct {
        uint8_t addr                : 7;
        uint8_t gen_cal_recognision : 1;
    } __packed;
} i2c_TWAR_t;

// TWI Slave Address Mask
typedef union __i2c_TWAMR {
    uint8_t reg;
    struct {
        uint8_t         : 1;
        uint8_t mask    : 7;
    } __packed;
} i2c_TWAMR_t;


// linked list 
typedef struct __i2c_ll {
    uint8_t adder;
    i2c_rw_bit_t RW;
    uint8_t data_len;
    uint8_t* data;
    struct __i2c_ll* __next;
} i2c_job_t;

#endif /* I2C_INTERNAL_TYPES_H */
