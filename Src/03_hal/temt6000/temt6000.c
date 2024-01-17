#include "temt6000.h"
#include "adc.h"

static adc_mux_channel_t temt6000_channel;
static float temt6000_voltage;

void temt6000_init(adc_mux_channel_t channel, float vref)
{
    temt6000_channel = channel;
    temt6000_voltage = vref;
    return;
}

temt6000_err_t temt6000_getLux(float* lux)
{
    temt6000_err_t ret_val = TEMT6000_ERR_OK;
    adc_err_t adc_ret;
    adc_measurement_t adc_value;
    float temp_lux;

    adc_ret = adc_setChannel(temt6000_channel);
    if(adc_ret != ADC_ERR_NOT_OK){
        
    }

    adc_ret = adc_startMeasurement();
    if(adc_ret == ADC_ERR_NOT_OK){
        ret_val = TEMT6000_ERR_NOT_OK;
        return ret_val;
    } 
    else if(adc_ret == ADC_ERR_BUSY){
        ret_val = TEMT6000_ERR_BUSY;
        return ret_val;
    }

    do{
        adc_value = adc_getMeasurement();
    } while(adc_value.error == ADC_ERR_BUSY);
    
    if(adc_value.error == ADC_ERR_NOT_OK){
        ret_val = TEMT6000_ERR_NOT_OK;
        return ret_val;
    }
    
    temp_lux = adc_value.result * temt6000_channel / 1024.0;
    temp_lux = temp_lux * 2.0 / 100;
    
    *lux = temp_lux;
    return ret_val;
}