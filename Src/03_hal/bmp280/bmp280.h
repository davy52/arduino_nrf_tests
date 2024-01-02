#ifndef BMP280_H
#define BMP280_H

typedef enum __bmp280_err {
    BMP280_ERR_OK      = 0x00,


    BMP280_ERR_NOT_OK  = 0xFF
} bmp280_err_t;

typedef enum __bmp280_mode {
    BMP280_MODE_SLEEP   = 0x00,
    BMP280_MODE_FORCED  = 0x01,
    BMP280_MODE_FORCED2 = 0x02,
    BMP280_MODE_NORMAL  = 0x03,
} bmp280_mode_t;

typedef enum __bmp280_sens_en {
    BMP280_SENS_TEMP        = 0x1,
    BMP280_SENS_PRESSURE    = 0x2,
    BMP280_SENS_BOTH        = BMP280_SENS_TEMP | BMP280_SENS_PRESSURE,
} bmp280_sens_en_t;

typedef enum __bmp280_oversample {
    BMP280_OVERSAMPLE_1     = 0x01,
    BMP280_OVERSAMPLE_2     = 0x02,
    BMP280_OVERSAMPLE_4     = 0x03,
    BMP280_OVERSAMPLE_8     = 0x04,
    BMP280_OVERSAMPLE_16    = 0x05
} bmp280_oversample_t;

typedef enum __bmp280_meas_delay {
    BMP280_DELAY_MS_0_5         = 0x00,
    BMP280_DELAY_MS_62_5        = 0x01,
    BMP280_DELAY_MS_125         = 0x02,
    BMP280_DELAY_MS_250         = 0x03,
    BMP280_DELAY_MS_500         = 0x04,
    BMP280_DELAY_MS_1000        = 0x05,
    BMP280_DELAY_MS_2000        = 0x06,
    BMP280_DELAY_MS_4000        = 0x07
} bmp280_meas_delay_t;

// from bme280 datasheet, in bmp280 register values not mentioned
typedef enum __bmp280_filter_settings {
    BMP280_FILTER_DIS       = 0x00,
    BMP280_FILTER_2         = 0x01,
    BMP280_FILTER_4         = 0x02,
    BMP280_FILTER_8         = 0x03,
    BMP280_FILTER_16        = 0x04
} bmp280_filter_settings_t;


typedef struct __bmp280_settings {
    bmp280_sens_en_t sens_en;
    bmp280_oversample_t temp_oversample;
    bmp280_oversample_t pressure_oversample;
    bmp280_mode_t mode;
    bmp280_meas_delay_t sample_delay;
    bmp280_filter_settings_t filter_setting;
} bmp280_settings_t;


bmp280_err_t bmp280_init();

bmp280_err_t bmp280_changeSettings();

bmp280_err_t bmp280_readTemp();

bmp280_err_t bmp280_readPressure();

bmp280_err_t bmp280_readHumidity();

bmp280_err_t bmp280_readAll();

bmp280_err_t bmp280_forceMeasure();

bmp280_err_t bmp280_sleep();

bmp280_err_t bmp280_wakeup();

bmp280_err_t bmp280_reset();

#endif /* BMP280_H */
