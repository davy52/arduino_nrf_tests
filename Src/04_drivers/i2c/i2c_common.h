#ifndef I2C_COMMON_H
#define I2C_COMMON_H

typedef enum {
    I2C_OK,
    I2C_NOT_OK,
    I2C_BUSY
} i2c_error_t;



#define SCL A5
#define SDA A4

//TODO SCL_Freq = F_CPU/(16 + 2(TWBR) * Prescaler)



#endif /* I2C_COMMON_H */
