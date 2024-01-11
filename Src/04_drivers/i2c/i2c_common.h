#ifndef I2C_COMMON_H
#define I2C_COMMON_H

typedef enum {
    I2C_ERR_OK = 0x01,
    I2C_ERR_BUSY,
    I2C_ERR_BUFF_TOO_SMALL,
    I2C_ERR_ARR_TOO_SMALL,
    I2C_ERR_NO_READ,            // there was no reading before this

    I2C_ERR_NOT_OK = 0xFF
} i2c_error_t;

typedef enum __i2c_rw_bit {
    I2C_RW_WRITE = 0x00,
    I2C_RW_READ = 0x01
} i2c_rw_bit_t;



typedef volatile struct __i2c_ll {
    i2c_error_t status;         // read only
    uint8_t adder;
    i2c_rw_bit_t RW;
    uint8_t repeated_start;
    uint8_t data_len;
    uint8_t* data;
    struct __i2c_ll* __next;    // dont touch that
} i2c_job_t;




#define SCL A5
#define SDA A4


//TODO SCL_Freq = F_CPU/(16 + 2(TWBR) * Prescaler)



#endif /* I2C_COMMON_H */
