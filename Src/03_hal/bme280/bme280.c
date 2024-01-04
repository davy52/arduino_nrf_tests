#include "bme280.h"
#include <stdint.h>

typedef enum __bme280_mode_internal {
    BME280_MODE_INT_SLEEP   = 0x00,
    BME280_MODE_INT_FORCED  = 0x01,
    BME280_MODE_INT_FORCED2 = 0x02,
    BME280_MODE_INT_NORMAL  = 0x03,
} bme280_mode_internal_t;

static int32_t t_fine = 0;

static uint16_t dig_T1;
static int16_t dig_T2;
static int16_t dig_T3;

static uint16_t dig_P1;
static int16_t dig_P2;
static int16_t dig_P3;
static int16_t dig_P4;
static int16_t dig_P5;
static int16_t dig_P6;
static int16_t dig_P7;
static int16_t dig_P8;
static int16_t dig_P9;

static uint8_t dig_H1;
static int16_t dig_H2;
static uint8_t dig_H3;
static int16_t dig_H4;
static int16_t dig_H5;
static int16_t dig_H6;


// comprensation equations from documentation
// temperature has to always be measured, its needed for conpensation formulas
static int32_t bme280_comp_T(uint32_t adc_T)
{
    int32_t var1, var2, T;

    var1 = (((adc_T >> 3) - ((int32_t)dig_T1 << 1)) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;
    
    T = ((t_fine * 5 + 128) >> 8);
    return T;
}

static uint32_t bme280_comp_P(uint32_t adc_P)
{
    int64_t var1, var2, P;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 += ((var1 * (int64_t)dig_P5) << 17);
    var2 += (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8 ) + ((var1 * (int64_t)dig_P2) << 12);    
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

    if ( var1 == 0)
    {
        return 0;   // division by zero
    }
    
    P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)dig_P9) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * P) >> 19;
    P = ((P + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (uint32_t)P;
}

static uint32_t bme280_comp_H(uint32_t adc_H)
{
    int32_t var1;

    var1 = (t_fine - ((int32_t)76800));
    var1 = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) + \
        ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t) dig_H6  )) >> 10) * \
        (((var1 * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + \
        ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
    
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);

    return (uint32_t)(var1 >> 12);
}

static bme280_readCompValues();
