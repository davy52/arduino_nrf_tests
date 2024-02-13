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

    hal_uart_init(F_CPU, 57600, HAL_UART_DOUBLE_SPEED | HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16, 40);

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
    delay_ms(50);
    ssd1306_init(0xCF);
    //hal_uart_sendBytes("ssd init\n", 9);
    
    uint8_t data[20];
    uint8_t size = 0;
    uint8_t i = 1;
    
    while(1){
    //    ssd1306_clear();
        ssd1306_setCursor(0, 0);
        size = sprintf(data, "Proba: %d\n\0", i++);
        hal_uart_sendBytes(data, size);
        ssd1306_writeString(data);
        ssd1306_setCursor(4, 90);
        ssd1306_writeString("KURWA!!!!");
        ssd1306_setCursor(7, 0);
        ssd1306_writeString("___________");

        delay_ms(2000);
    }
}
