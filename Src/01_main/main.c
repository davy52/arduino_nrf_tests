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

    hal_uart_init(F_CPU, 9600,   HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 40);

        hal_uart_sendBytes("KURWA!\n", 7);
    i2c_error_t i2c_ret_val;
    i2c_ret_val = i2c_master_init(F_CPU, 20000);
        hal_uart_sendBytes("KURWA!\n", 7);
    ssd1306_init(0xCF);
        hal_uart_sendBytes("KURWA!\n", 7);
    
    uint8_t data[80];
    uint8_t size = 0;
    uint8_t i = 1;
    uint8_t first = 1;
    
    while(1){
        hal_uart_sendBytes("KURWA!\n", 7);
        delay_ms(50);
        if(first == 0){
            while(hal_uart_sendByte(WANT_DATA) != HAL_UART_ERR_OK);
            delay_ms(50);
            while(hal_uart_readBytes((uint8_t*)&packet, sizeof(packet_t)) == HAL_UART_ERR_BUFF_EMPTY);
        }
        else{
            first = 0;
        }

        ssd1306_setCursor(0, 0);
        size = sprintf(data, "Lux: %u.%u",
            (uint16_t)(packet.lux & 0xFF00) >> 16, (uint16_t)(packet.lux & 0xFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(2, 0);
        size = sprintf(data, "Temp: %u.%u",
            (uint16_t)(packet.temp & 0xFF00) >> 16, (uint16_t)(packet.temp & 0xFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(3, 0);
        size = sprintf(data, "Pressure: %u.%u",
            (uint16_t)(packet.pressure & 0xFF00) >> 16, (uint16_t)(packet.pressure & 0xFF));
        ssd1306_writeString(data);

        ssd1306_setCursor(4, 0);
        size = sprintf(data, "Humidity: %u.%u",
            (uint16_t)(packet.humidity & 0xFF00) >> 16, (uint16_t)(packet.humidity & 0xFF));
        ssd1306_writeString(data);

        delay_ms(2000);
    }
}
