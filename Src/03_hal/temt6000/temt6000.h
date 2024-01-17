#ifndef TEMT6000_H
#define TEMT6000_H

#include "adc.h"
typedef enum __temt6000_err {
    TEMT6000_ERR_OK = 0,
    TEMT6000_ERR_BUSY,
    TEMT6000_ERR_NOT_OK
} temt6000_err_t;

void temt6000_init(adc_mux_channel_t channel, float vref);

temt6000_err_t temt6000_getLux(float* lux);

#endif /* TEMT6000_H */
