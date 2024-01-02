#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

typedef enum __hal_adc_err {
    HAL_ADC_ERR_OK = 0,
    HAL_ADC_ERR_BUSY,
    HAL_ADC_ACTION_NOT_ALLOWED,

    HAL_ADC_ERR_NOT_OK = 0xFF
} hal_adc_err_t;

typedef struct __hal_adc_measurement_float {
    hal_adc_err_t error;
    float result;
} hal_adc_read_result_float_t;

typedef struct __hal_adc_measurement_float {
    hal_adc_err_t error;
    uint16_t result;
} hal_adc_read_result_t;

typedef enum __hal_adc_auto_trigger {
    HAL_ADC_AUTO_TRIG_NONE,
    HAL_ADC_AUTO_TRIG_EXTERNAL,
    HAL_ADC_AUTO_TRIG_TIM0_COMP,
    HAL_ADC_AUTO_TRIG_TIM0_OVERFLOW,
    HAL_ADC_AUTO_TRIG_TIM1_COMP,
    HAL_ADC_AUTO_TRIG_TIM1_OVERFLOW,
    HAL_ADC_AUTO_TRIG_CAPTURE
} hal_adc_auto_trigger_t;

typedef enum __hal_adc_channel {
    HAL_ADC_CHANNEL0,
    HAL_ADC_CHANNEL1,
    HAL_ADC_CHANNEL2,
    HAL_ADC_CHANNEL3,
    HAL_ADC_CHANNEL4,
    HAL_ADC_CHANNEL5,
    HAL_ADC_CHANNEL6,
    HAL_ADC_CHANNEL7,

    HAL_ADC_TEMP
} hal_adc_channel_t;

typedef enum __hal_adc_vref {
    HAL_ADC_VREF_EXTERNAL,
    HAL_ADC_VREF_INTERNAL_VCC,
    HAL_ADC_INTERNAL_1v1
} hal_adc_vref_t;

typedef struct __hal_settings {
    hal_adc_channel_t channel;
    hal_adc_auto_trigger_t auto_trigger;
    hal_adc_vref_t voltage_reference;
    float vref;
    uint16_t buffer_size;

} hal_adc_settings_t;

hal_adc_err_t hal_adc_init(hal_adc_settings_t settings);

hal_adc_err_t hal_adc_changeSettings(hal_adc_settings_t settings);

hal_adc_err_t hal_adc_getSettings(hal_adc_settings_t* settings);

hal_adc_read_result_t hal_adc_readChannel(hal_adc_channel_t channel);

hal_adc_read_result_float_t hal_adc_readChannel_float(hal_adc_channel_t channel);

hal_adc_err_t hal_adc_startAutoMeasurements(void);

hal_adc_err_t hal_adc_stopAutoMeasurements(void);

hal_adc_err_t hal_adc_readAutoMeasurements(uint16_t data, uint16_t buf_size);

#endif /* HAL_ADC_H */
