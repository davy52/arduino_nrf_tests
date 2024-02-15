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
#include "i2c_master.h"
#include "bme280.h"

#include "adc.h"
#include "temt6000.h"

#include "ssd1306.h"

#include <stdio.h>
#include "ssd1306_regs.h"

/*  FUSES = {
     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
     .high = HFUSE_DEFAULT,
     .extended = EFUSE_DEFAULT
 }; */

// LOCKBITS = LOCKBITS_DEFAULT;

#define WANT_DATA 0x25



typedef struct {
    volatile uint32_t lux;
    volatile uint32_t temp;
    volatile uint32_t pressure;
    volatile uint32_t humidity;
} packet_t __attribute((packed));

 volatile packet_t packet = {0};

void pack(packet_t* packet, float lux, float temp, float pressure, float humidity)
{
    packet->lux = (((uint32_t)lux) << 16) | (uint16_t)(lux * 100)%100;
    packet->temp = (((uint32_t)temp) << 16) | (uint16_t)(temp * 100)%100;
    packet->pressure = (((uint32_t)pressure) << 16) | (uint16_t)(pressure * 100)%100;
    packet->humidity = (((uint32_t)humidity) << 16) | (uint16_t)(humidity * 100)%100;
}


int main(void)
{
    sei();

            blink_pin(port_B5);
            delay_ms(50);
    hal_uart_init(F_CPU, 9600, HAL_UART_DOUBLE_SPEED | HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 20, 20);
            blink_pin(port_B5);
            delay_ms(50);

    i2c_error_t i2c_ret_val;
    i2c_ret_val = i2c_master_init(F_CPU, 20000);
            blink_pin(port_B5);
            delay_ms(50);
    
    bme280_settings_t bme_settings = {
        .adder = 0x76,
        .filter_setting = BME280_FILTER_DIS,
        .temp_oversample = BME280_OVERSAMPLE_16,
        .pressure_oversample = BME280_OVERSAMPLE_4,
        .humidity_oversample = BME280_OVERSAMPLE_4,
        .sample_delay = BME280_DELAY_MS_500,
        .sens_en = BME280_SENS_ALL,
        .mode = BME280_MODE_ON_DEMAND
    };
    bme280_init( bme_settings);
            blink_pin(port_B5);
            delay_ms(50);

    // port_set_pinMode(port_C0, PORT_INPUT);
    adc_init(ADC_REF_INTERNAL_VCC, ADC_PRESCALER_64, ADC_AUTO_FREE);
    adc_enable();
    // adc_disableInt();
    temt6000_init(ADC_CHANNEL_0, 3.3);
            blink_pin(port_B5);
            delay_ms(50);
    
    uint8_t data[80];
    uint8_t size = 0;
    uint8_t i = 1;
    uint8_t first = 1;
    bme280_err_t bme_err;
    bme280_result_all_t result;
    temt6000_err_t temt_err;
    float light;
    
    while(1){
        while(hal_uart_readByte(data) == HAL_UART_ERR_BUFF_EMPTY);
        if(data[0] != WANT_DATA){
            blink_pin(port_B5);
            delay_ms(50);
            continue;
        }


        bme_err = bme280_startMeasurement();

        do{
            result = bme280_readAll();
            delay_ms(10);
        } while(result.error == BME280_ERR_BUSY);
        delay_ms(2);
        temt_err = temt6000_getLux(&light);

        pack(&packet, light, result.temp, result.pressure, result.humidity);
        // packet.lux = 0x01020304;
        // packet.temp = 0x05060708;
        // packet.pressure = 0x090A0B0C;
        // packet.humidity = 0x0D0E0F10;

        //#define DEBUG
        #ifdef DEBUG
        size = sprintf(data, "Lux: %d.%d\n", (uint16_t)light, (uint16_t)(light * 100)%100);
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        size = sprintf(data, "Temp: %d.%d\n", (uint16_t)result.temp, (uint16_t)(result.temp * 100)%100);
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        size = sprintf(data, "Pressure: %d.%d\n", (uint16_t)result.pressure, (uint16_t)(result.pressure * 100)%100);
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        size = sprintf(data, "Humid: %d.%d\n", (uint16_t)result.humidity, (uint16_t)(result.humidity * 100)%100);
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        
        size = sprintf(data, "Lux: %d.%d\n", (packet.lux & 0xFFFF0000) >> 16, (packet.lux & 0xFFFFF));
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        size = sprintf(data, "Temp: %d.%d\n", (packet.temp & 0xFFFF0000) >> 16, (packet.temp & 0xFFFFF));
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        size = sprintf(data, "Pressure: %d.%d\n", (packet.pressure & 0xFFFF0000) >> 16, (packet.pressure & 0xFFFFF));
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        size = sprintf(data, "Humidity: %d.%d\n", (packet.humidity & 0xFFFF0000) >> 16, (packet.humidity & 0xFFFFF));
        while(hal_uart_sendBytes(data, size) == HAL_UART_ERR_BUFF_FULL);
        #endif


        while(hal_uart_sendBytes((uint8_t*)&packet, sizeof(packet_t)) == HAL_UART_ERR_BUFF_FULL);
    }
}
