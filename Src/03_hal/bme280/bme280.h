#ifndef BME280_H
#define BME280_H

#include <stdio.h>

typedef enum __bme280_err {
    BME280_ERR_OK      = 0x00,
    BME280_ERR_BUSY,
    BME280_BAD_ID,


    BME280_ERR_NOT_OK  = 0xFF
} bme280_err_t;

typedef struct __bme280_result_one {
    bme280_err_t error;
    float result;
} bme280_result_one_t;

typedef struct __bme280_result_all
{
    bme280_err_t error;
    float temp;
    float pressure;
    float humidity;
} bme280_result_all_t;


typedef enum __bme280_mode {
    BME280_MODE_ON_DEMAND,
    BME280_MODE_CONINOUS
} bme280_mode_t;

typedef enum __bme280_sens_en {
    BME280_SENS_TEMP        = 0x1,
    BME280_SENS_PRESSURE    = 0x2,
    BME280_SENS_HUMIDITY    = 0x4,

    BME280_SENS_ALL        = BME280_SENS_TEMP | BME280_SENS_PRESSURE | BME280_SENS_HUMIDITY,
} bme280_sens_en_t;

typedef enum __bme280_oversample {
    BME280_OVERSAMPLE_1     = 0x01,
    BME280_OVERSAMPLE_2     = 0x02,
    BME280_OVERSAMPLE_4     = 0x03,
    BME280_OVERSAMPLE_8     = 0x04,
    BME280_OVERSAMPLE_16    = 0x05
} bme280_oversample_t;

typedef enum __bme280_meas_delay {
    BME280_DELAY_MS_0_5         = 0x00,
    BME280_DELAY_MS_62_5        = 0x01,
    BME280_DELAY_MS_125         = 0x02,
    BME280_DELAY_MS_250         = 0x03,
    BME280_DELAY_MS_500         = 0x04,
    BME280_DELAY_MS_1000        = 0x05,
    BME280_DELAY_MS_2000        = 0x06,
    BME280_DELAY_MS_4000        = 0x07
} bme280_meas_delay_t;

// from bme280 datasheet, in bme280 register values not mentioned
typedef enum __bme280_filter_settings {
    BME280_FILTER_DIS       = 0x00,
    BME280_FILTER_2         = 0x01,
    BME280_FILTER_4         = 0x02,
    BME280_FILTER_8         = 0x03,
    BME280_FILTER_16        = 0x04
} bme280_filter_settings_t;



typedef struct __bme280_settings {
    uint8_t adder;
    bme280_sens_en_t sens_en;
    bme280_oversample_t temp_oversample;
    bme280_oversample_t pressure_oversample;
    bme280_oversample_t humidity_oversample;
    bme280_mode_t mode;
    bme280_meas_delay_t sample_delay;
    bme280_filter_settings_t filter_setting;

} bme280_settings_t;


/**
 * @brief Initialize module with settings
 *  should be used only once after every power on / reset
 * 
 * @param settings 
 * @return bme280_err_t 
 */
bme280_err_t bme280_init(bme280_settings_t settings);

/**
 * @brief Change settings of the module 
 *  should be used only if bme280_init() was called already
 * 
 * @param settings 
 * @return bme280_err_t 
 */
bme280_err_t bme280_changeSettings(bme280_settings_t settings);

/**
 * @brief Read current temp
 * 
 * @return bme2280_result_one_t struct with err code and result
 */
bme280_result_one_t bme280_readTemp();

/**
 * @brief Read current Pressure
 * 
 * @return bme280_result_one_t struct with err code and result
 */
bme280_result_one_t bme280_readPressure();

/**
 * @brief Read current Humidity
 * 
 * @return bme280_result_one_t struct with err code and result
 */
bme280_result_one_t bme280_readHumidity();

/**
 * @brief Read all sensors 
 *      works even if some are disabled (result is always 0)
 * 
 * @return bme280_result_all_t struct with err code and all results
 */
bme280_result_all_t bme280_readAll();

/**
 * @brief force measurement on sensor 
 *      use only if mode set to BME280_MODE_ON_DEMAND
 * 
 * @return bme280_err_t 
 */
bme280_err_t bme280_startMeasurement();

/**
 * @brief Chamge mode between energy efficient on demand and continous
 * 
 * @param mode mode to be set
 * @return bme280_err_t 
 */
bme280_err_t bme280_changeMode(bme280_mode_t mode);

/**
 * @brief Soft reset sensor
 * 
 * @return bme280_err_t 
 */
bme280_err_t bme280_reset();

#endif /* BME280_H */
