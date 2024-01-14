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
    sei();

    hal_uart_init(F_CPU, 9600, HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 40);

    i2c_error_t i2c_ret_val;
    i2c_ret_val = i2c_master_init(F_CPU, 20000);
    delay_ms(50);
    if(i2c_ret_val != I2C_ERR_OK){
        char* msg = "i2c not ok\n";
        hal_uart_sendBytes(msg, 11);
    } else {
        char* msg = "i2c is ok :D\n";
        hal_uart_sendBytes(msg, 13);
    }
    
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
    
    while(bme280_init(bme_settings) != BME280_ERR_OK){
        blink_pin(port_D4);
        delay_ms(500);
    }
    
    blink(1);


    bme280_result_all_t result;
    bme280_err_t bme_err;
    
    while(1){

        bme_err = bme280_startMeasurement();

        do{
            result = bme280_readAll();
            delay_ms(10);
        } while(result.error == BME280_ERR_BUSY);

        while(1);

        delay_ms(1000);
    }
}
