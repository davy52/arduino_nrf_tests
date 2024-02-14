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

typedef struct {
    float lux;
    float temp;
    float pressure;
    float humidity;
} packet_t __attribute((packed));


packet_t packet = {0};


int main(void)
{
    sei();

    hal_uart_init(F_CPU, 57600, HAL_UART_DOUBLE_SPEED | HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 40);

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

        packet.lux = light;
        packet.temp = result.temp;
        packet.pressure = result.pressure;
        packet.humidity = result.humidity;

        while(hal_uart_sendBytes((uint8_t*)&packet, sizeof(packet_t)) == HAL_UART_ERR_BUFF_FULL);

    }
}
