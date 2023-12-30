#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef enum {
    ADC_ERR_OK = 0,
    ADC_ERR_BUSY,

    ADC_ERR_NOT_OK = 0x0F
} adc_err_t;

typedef enum {
    ADC_REF_EXTERNAL        = 0x0 << 6,
    ADC_REF_INTERNAL_VCC    = 0x1 << 6,
    ADC_REF_INTERNAL_1v1    = 0x3 << 6
} adc_ref_settings_t;

typedef enum {
    ADC_LADJ_OFF    = 0 << 5,
    ADC_LADJ_ON     = 1 << 5
} adc_left_adj_t;

typedef enum {
    ADC_CHANNEL_0       = 0x0,
    ADC_CHANNEL_1       = 0x1,
    ADC_CHANNEL_2       = 0x2,
    ADC_CHANNEL_3       = 0x3,
    ADC_CHANNEL_4       = 0x4,
    ADC_CHANNEL_5       = 0x5,
    ADC_CHANNEL_6       = 0x6,
    ADC_CHANNEL_7       = 0x7,

    ADC_CHANNEL_TEMP    = 0x8,

    ADC_CHANNEL_VBG     = 0xE,
    ADC_CHANNEL_GND     = 0xF
} adc_mux_channel_t;

typedef enum {
    ADC_PRESCALER_2 = 0x0,
    // ADC_PRESCALER_2 = 0x1, // WTF? waiting to see one of them not work
    ADC_PRESCALER_4     = 0x2,
    ADC_PRESCALER_8     = 0x3,
    ADC_PRESCALER_16    = 0x4,
    ADC_PRESCALER_32    = 0x5,
    ADC_PRESCALER_64    = 0x6,
    ADC_PRESCALER_128   = 0x7
} adc_prescaler_t;

typedef enum {
    ADC_AUTO_TRIG_EN    = 0x1 << 5,
    ADC_INTERRUPT_EN    = 0x1 << 3,
} adc_auto_int_t;

typedef enum {
    ADC_AUTO_FREE       = 0x0,
    ADC_AUTO_COMP       = 0x1,
    ADC_AUTO_EXT        = 0x2,
    ADC_AUTO_TIM0_A     = 0x3,  // Timer/Counter0 compare match A
    ADC_AUTO_TIM0_OVER  = 0x4,  // Timer/Counter0 overflow
    ADC_AUTO_TIM1_B     = 0x5,  // Timer/Counter1 compare match B
    ADC_AUTO_TIM1_OVER  = 0x6,  // Timer/Counter1 overflow
    ADC_AUTO_TIM1_CAP   = 0x7   // Timer/Counter1 capture event
} adc_auto_src_t;

typedef struct
{
    adc_err_t error : 6;     // 6 bits for error
    uint16_t result : 10;     // 10 bits for adc measurement

} __attribute__((packed)) adc_measurement_t;

/**
 * @brief adc option setting
 *      should be used when conversion is NOT running 
 *
 * @param ref_mux_ladj  reference voltage | channel | left adjust
 * @param auto_int_pre  auto trigger | interrupt en | prescaler
 * @param auto_src      auto trigger source
 */
void adc_init(uint8_t ref_mux_ladj, uint8_t auto_int_pre, adc_auto_src_t auto_src);

/**
 * @brief enable adc
 * 
 */
void adc_enable();

/**
 * @brief disable adc
 * 
 */
void adc_disable();

/**
 * @brief set channel for adc
 * 
 * @param channel channel to be set 
 * @return adc_err_t error code
 */
adc_err_t adc_setChannel(adc_mux_channel_t channel);

/**
 * @brief enable adc interrupt
 * 
 */
void adc_enableInt();

/**
 * @brief disable adc interrupt
 * 
 */
void adc_disableInt();

/**
 * @brief start adc measurement
 * 
 * @return adc_err_t error code
 */
adc_err_t adc_startMeasurement();

/**
 * @brief get measurement result
 *      will return error ADC_ERR_BUSY if mesurement not finished
 * 
 * @return adc_measurement_t error code and result
 */
adc_measurement_t adc_getMeasurement();

/**
 * @brief get measurment result, blocking function
 *      will wait and block cpu until mesurement is completed
 *      USE WITH CAUTION
 * 
 * @return adc_measurement_t error code and result
 */
adc_measurement_t adc_getMeasurementBlock();



#endif /* ADC_H */
