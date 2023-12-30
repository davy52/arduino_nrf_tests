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
/*  FUSES = {
     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
     .high = HFUSE_DEFAULT,
     .extended = EFUSE_DEFAULT
 }; */

// LOCKBITS = LOCKBITS_DEFAULT;

void make_bin(uint8_t* buf, uint16_t value, uint8_t n)
{
    for(int i = 0; i < n; i++){
        buf[i] = ((value >> n - i - 1) & 1) ? '1' : '0';
    }
    buf[n] = '\n';
}

void printByte(uint8_t byte)
{
    uint8_t str_buf[9];
    make_bin(str_buf, byte, 8);
    hal_uart_sendBytes(str_buf, 9);
    delay_ms(1000);
    return;
}

int main(void)
{
    uint8_t states = 0;

    adc_measurement_t adc_result;
    uint8_t str_buf[11];      // "ADC: %1.2f\n\0"
    sei();

    port_set_pinMode(D3, PORT_OUTPUT);
    port_set_pinMode(D4, PORT_OUTPUT);
    port_set_pinMode(D5, PORT_OUTPUT);
    port_set_pinMode(D13, PORT_OUTPUT);

    

    hal_uart_init(F_CPU, 9600, HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 16);

    
       
    adc_init(ADC_REF_INTERNAL_VCC | ADC_CHANNEL_0, ADC_PRESCALER_64, ADC_AUTO_FREE);
    adc_enable();


    //adc_startMeasurement();
    do{
        char* temp = "UART INIT TEST\n";
        hal_uart_sendBytes(temp, 15);

    } while(0);
    delay_ms(500);
    printByte(ADMUX);
    printByte(ADCSRA);
    printByte(ADCSRB);
    printByte(DIDR0);
    while(1){
        
        do{
            adc_result = adc_getMeasurement();
            hal_uart_sendByte('B');
            delay_ms(50);
        }while(adc_result.error != ADC_ERR_OK);

        hal_uart_sendByte('a');
        if(adc_result.error == ADC_ERR_OK){
            make_bin(str_buf, adc_result.result, 10);
            hal_uart_sendBytes(str_buf, 11);
        }
        adc_startMeasurement();

        port_set_pinState(D3, states & 1);
        port_set_pinState(D4, (states & 2) >> 1);
        port_set_pinState(D5, (states & 4) >> 2);
        port_toggle_pinState(D13);

        states++;
        // delay_ms(1000);
    }
}
