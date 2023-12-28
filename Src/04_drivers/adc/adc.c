#include "adc.h"
// #include "avr/io.h"

#define __at(x) __attribute__((address (x)))

// REGISTERS

// IMPORTANT: THIS WORKS only for atmega328p 

// ADMUX
typedef union {
    struct {
        uint8_t channel_sel : 4;
        uint8_t             : 1;
        uint8_t left_adj    : 1;
        uint8_t ref_volt    : 2;
    } __attribute__((packed));
    uint8_t reg;
} adc_ADMUX_t;

static volatile adc_ADMUX_t adc_ADMUX __at(0x7C);

// ADCSRA
typedef union {
    struct {
        uint8_t adc_prescaler   : 2;
        uint8_t adc_int_en      : 1;
        uint8_t adc_int_flag    : 1;
        uint8_t adc_auto_en     : 1;
        uint8_t adc_start       : 1;
        uint8_t adc_en          : 1;
    } __attribute__((packed));
    uint8_t reg;
} adc_ADCSRA_t;

static volatile adc_ADCSRA_t adc_ADCSRA __at(0x7C);

// ADCSRB
typedef union {
    struct {
        uint8_t auto_trigger_src    : 3;
        uint8_t                     : 3;
        uint8_t analog_compare_en   : 1;
        uint8_t                     : 1;
    } __attribute__((packed));
    uint8_t reg;
} adc_ADCSRB_t;

static volatile adc_ADCSRB_t adc_ADCSRB __at(0x7B);

// DIDR0
typedef union {
    struct {
        uint8_t adc0_disable    :1;
        uint8_t adc1_disable    :1;
        uint8_t adc2_disable    :1;
        uint8_t adc3_disable    :1;
        uint8_t adc4_disable    :1;
        uint8_t adc5_disable    :1;
        uint8_t                 :2;
    } __attribute__((packed));
    uint8_t reg;
} adc_DIDR0_t;

static volatile adc_DIDR0_t adc_DIDR0 __at(0x7E);

// ADCL ADCH
static volatile uint8_t adc_ADCL __at(0x78);
static volatile uint8_t adc_ADCH __at(0x79);

// private functions

// public functions

void adc_init(uint8_t ref_mux_ladj, uint8_t auto_int_pre, adc_auto_src_t auto_src)
{
    adc_ADMUX.reg = ref_mux_ladj;

    adc_ADCSRA.adc_auto_en = (auto_int_pre >> 5) & 0x1;
    adc_ADCSRA.adc_int_en = (auto_int_pre >> 3) & 0x1;
    adc_ADCSRA.adc_prescaler = auto_int_pre & 0x3;
    
    adc_ADCSRB.analog_compare_en = 0;
    adc_ADCSRB.auto_trigger_src = auto_src;
    
    return;
}

void adc_enable()
{
    adc_ADCSRA.adc_en = 1;
    return;
}

void adc_disable()
{
    adc_ADCSRA.adc_en = 0;
    return;
}

adc_err_t adc_setChannel(adc_mux_channel_t channel)
{
    adc_err_t ret_val = ADC_ERR_OK;

    if(adc_ADCSRA.adc_start){
        ret_val = ADC_ERR_BUSY;
        return ret_val;
    }

    adc_ADMUX.channel_sel = channel;
    return ret_val;
}

void adc_enableInt()
{
    adc_ADCSRA.adc_int_en = 1;
    return;
}

void adc_disableInt()
{
    adc_ADCSRA.adc_int_en = 0;
    return;
}

adc_err_t adc_startMeasurement()
{
    adc_err_t ret_val = ADC_ERR_OK;
    
    // Conversion is running
    if(adc_ADCSRA.adc_start){
        ret_val = ADC_ERR_BUSY;
        return ret_val;
    }
    
    adc_ADCSRA.adc_int_flag = 0;
    adc_ADCSRA.adc_start = 1;
    return ret_val;
}

adc_measurement_t adc_getMeasurement()
{
    adc_measurement_t ret_val = {
        .error = ADC_ERR_OK,
        .result = 0
    };

    if(!adc_ADCSRA.adc_int_flag){
        ret_val.error = ADC_ERR_BUSY;
        return ret_val;
    }
    
    ret_val.result = adc_ADCL;
    ret_val.result |= adc_ADCH << 8;
    
    return ret_val;
}

adc_measurement_t adc_getMeasurementBlock()
{
    adc_measurement_t ret_val = {
        .error = ADC_ERR_OK,
        .result = 0
    };

    while(!adc_ADCSRA.adc_int_flag);
    
    ret_val.result = adc_ADCL;
    ret_val.result |= adc_ADCH << 8;

    return ret_val;
}

