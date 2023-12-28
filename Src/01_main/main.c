/*
 * @file main.c
 * @author  ()
 * @brief 
 * @version 0.1
 * @date 2023-09-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// #define arduino_pro_mini_3v3
#include "avr/interrupt.h"

#include "uart_control.h"

#include "spi_master.h"
#include "debug.h"
#include "arduino_boards.h"
#include "util/delay.h"
#include "hal_uart.h"

#include "adc.h"
#include <stdio.h>
/*  FUSES = {
     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
     .high = HFUSE_DEFAULT,
     .extended = EFUSE_DEFAULT
 }; */

// LOCKBITS = LOCKBITS_DEFAULT;


int main(void)
{
    uint8_t states = 0;

    adc_measurement_t adc_result;
    char str_buf[11];      // "ADC: %1.2f\n\0"

    sei();

    port_set_pinMode(D3, PORT_OUTPUT);
    port_set_pinMode(D4, PORT_OUTPUT);
    port_set_pinMode(D5, PORT_OUTPUT);
    port_set_pinMode(D13, PORT_OUTPUT);
    
    adc_init(ADC_REF_INTERNAL_VCC | ADC_CHANNEL_0, ADC_PRESCALER_128, ADC_AUTO_FREE);
    adc_enable();
    hal_uart_init(F_CPU, 9600, HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 16);

    adc_startMeasurement();
    while(1){
        // try to start conversion
        adc_result = adc_getMeasurementBlock();
        if(adc_result.error == ADC_ERR_OK){
            sprintf(str_buf, "ADC: %1.2f\n", (float)(adc_result.result * 5.0 / 0x400));
            hal_uart_sendBytes(str_buf, 10);
        }
        adc_startMeasurement();

        port_set_pinState(D3, states & 1);
        port_set_pinState(D4, (states & 2) >> 1);
        port_set_pinState(D5, (states & 4) >> 2);
        port_toggle_pinState(D13);

        states++;
        delay_ms(1000);
    }
}
