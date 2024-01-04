#ifndef I2C_COMMON_H
#define I2C_COMMON_H

typedef enum {
    I2C_ERR_OK = 0x01,
    I2C_ERR_BUSY,

    I2C_ERR_NOT_OK = 0xFF
} i2c_error_t;



#define SCL A5
#define SDA A4


//TODO SCL_Freq = F_CPU/(16 + 2(TWBR) * Prescaler)



#endif /* I2C_COMMON_H */
