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

 FUSES = {
     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
     .high = HFUSE_DEFAULT,
     .extended = EFUSE_DEFAULT
 };

LOCKBITS = LOCKBITS_DEFAULT;


#include <stdlib.h>
int main(void)
{
    sei();

    uint8_t data[16] = "test123\n";
    hal_uart_init(F_CPU, 9600, HAL_UART_CHAR_8BIT | HAL_UART_PARITY_DIS | HAL_UART_STOP_1BIT, 16u, 16u);
    uint16_t buf_cnt;

    int i = 0;
    hal_uart_sendBytes(data, 7);
    while(1){
        hal_uart_getRxBufferCount(&buf_cnt);
        if(buf_cnt > 0){
            hal_uart_readBytes(data, buf_cnt);
            hal_uart_sendBytes(data, buf_cnt);
        }
        delay_ms(50);
    }
}
