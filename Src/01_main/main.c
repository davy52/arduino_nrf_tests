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

// FUSES = {
//     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
//     .high = HFUSE_DEFAULT,
//     .extended = EFUSE_DEFAULT
// };

// LOCKBITS = LOCKBITS_DEFAULT;


#include <stdlib.h>
int main(void)
{
    sei();
    spi_master_init(SPI_INT_DISABLE, SPI_MSB_FIRST, SPI_CLK_IDLE_LOW, SPI_CLK_PHASE_LEADING, SPI_DIV_128);
    log_uart_init();

    uint8_t data[5] = {0x55 , 0, 0, 0, 0};

    while(1){
        log_uart(data, 1);
        spi_master_transmit(0X55);
        while(!(spi_master_check_status()).transfer_clomplete_flag);
        
        delay_ms(200);
    }
}
