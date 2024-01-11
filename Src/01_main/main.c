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
    hal_uart_init(F_CPU, 9600, HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 40, 16);

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
    


    sei();
    uint8_t adder = 0x76 << 1;
    uint8_t data1[] = {0};
    
    while(1){
        
        delay_ms(10);
        uint8_t data1[2] = {0xF4, 0x55};
        uint8_t data2[1] = {0xFE};
        uint8_t data3[2] = {0};
        i2c_job_t jobs[3] = {
            {
                .adder = adder,
                .RW = I2C_RW_WRITE,
                .repeated_start = 0,
                .data = data1,
                .data_len = 2
            },
            {
                .adder = adder,
                .RW = I2C_RW_WRITE,
                .repeated_start = 1,
                .data = data2,
                .data_len = 1
            },
            {
                .adder = adder,
                .RW = I2C_RW_READ,
                .repeated_start = 0,
                .data = data3,
                .data_len = 2
            }
        };

        for(int i = 0; i < 3; i++){
            blink(1);
            i2c_ret_val = i2c_master_appendJob(&(jobs[i]));
            if(i2c_ret_val == I2C_ERR_OK){
                char* msg = "a_ok\n";
                hal_uart_sendBytes(msg, 5);
            } else {
                char* msg = "a_nok\n";
                hal_uart_sendBytes(msg, 6);
            }
            blink(1);
        }
        i2c_master_startTransaction();

        delay_ms(500);

        while(jobs[1].status != I2C_ERR_OK);
        hal_uart_sendBytes(data3, 2);
    }
}
