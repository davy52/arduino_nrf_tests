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


 volatile packet_t packet = {
    .lux = 0xFFFF,
    .temp = 0xFFFF,
    .pressure = 0xFFFF,
    .humidity = 0xFFFF
 };

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

    hal_uart_init(F_CPU, 9600,  HAL_UART_DOUBLE_SPEED | HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 20, 20);
    hal_uart_disableIrq();
    hal_uart_err_t hal_err;

    i2c_error_t i2c_ret_val;
    i2c_ret_val = i2c_master_init(F_CPU, 20000);
    
    uint8_t data[80];
    uint8_t size = 0;
    uint8_t i = 1;
    uint8_t first = 1;
    
    ssd1306_init(0xCF);
    
    while(1){

            
        ssd1306_clear();
        ssd1306_setCursor(0, 0);
        size = sprintf(data, "Lux: %u.%u",
            (uint16_t)(packet.lux & 0xFFFF0000) >> 16, (uint16_t)(packet.lux & 0xFFFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(2, 0);
        size = sprintf(data, "Temp: %u.%u",
            (uint16_t)(packet.temp & 0xFFFF0000) >> 16, (uint16_t)(packet.temp & 0xFFFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(3, 0);
        size = sprintf(data, "Pressure: %u.%u",
            (uint16_t)(packet.pressure & 0xFFFF0000) >> 16, (uint16_t)(packet.pressure & 0xFFFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(4, 0);
        size = sprintf(data, "Humidity: %u.%u",
            (uint16_t)(packet.humidity & 0xFFFF0000) >> 16, (uint16_t)(packet.humidity & 0xFFFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(6, 0);
        size = sprintf(data, "count: %d", i);
        ssd1306_writeString(data);
        
        hal_uart_writeByteNoIrq(WANT_DATA);
        delay_ms(5);
        for(uint8_t b = 0; b < 16; b++){
            hal_uart_readByteNoIrq(((uint8_t*)&packet) + ( b));
        }
        i++;

        delay_ms(2000);
    }
}
