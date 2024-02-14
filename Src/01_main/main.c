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
#define I2C_USE_IRQ 1
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


typedef union {
    struct {
        volatile uint32_t lux;
        volatile uint32_t temp;
        volatile uint32_t pressure;
        volatile uint32_t humidity;
    }  __attribute((packed));
    uint8_t data[16];
}packet_t;

uint8_t payload[16];


 volatile packet_t packet = {0};

void pack(packet_t* packet, float lux, float temp, float pressure, float humidity)
{
    packet->lux = ((uint16_t)lux << 16) | (uint16_t)(lux * 100)%100;
    packet->temp = ((uint16_t)temp << 16) | (uint16_t)(temp * 100)%100;
    packet->pressure = ((uint16_t)pressure << 16) | (uint16_t)(pressure * 100)%100;
    packet->humidity = ((uint16_t)humidity << 16) | (uint16_t)(humidity * 100)%100;
}


int main(void)
{
    sei();

    hal_uart_init(F_CPU, 9600,   HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 40);

    i2c_error_t i2c_ret_val;
    i2c_ret_val = i2c_master_init(F_CPU, 20000);
    
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
    temt6000_init(ADC_CHANNEL_0, 3.3);
    
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
            continue;
        }


        bme_err = bme280_startMeasurement();

        do{
            result = bme280_readAll();
            delay_ms(10);
        } while(result.error == BME280_ERR_BUSY);

        temt_err = temt6000_getLux(&light);

        pack(&packet, light, result.temp, result.pressure, result.humidity);

        while(hal_uart_sendBytes(packet.data, sizeof(packet_t)) == HAL_UART_ERR_BUFF_FULL);

    }
}
