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

/*  FUSES = {
     .low = LFUSE_DEFAULT | FUSE_CKDIV8,
     .high = HFUSE_DEFAULT,
     .extended = EFUSE_DEFAULT
 }; */

// LOCKBITS = LOCKBITS_DEFAULT;


#include <stdlib.h>
int main(void)
{
    uint8_t states = 0;

    sei();

    port_set_pinMode(D3, PORT_OUTPUT);
    port_set_pinMode(D4, PORT_OUTPUT);
    port_set_pinMode(D5, PORT_OUTPUT);
    port_set_pinMode(D13, PORT_OUTPUT);
    
    while(1){

        port_set_pinState(D3, states & 1);
        port_set_pinState(D4, (states & 2) >> 1);
        port_set_pinState(D5, (states & 4) >> 2);
        port_toggle_pinState(D13);

        states++;
        delay_ms(1000);
    }
}
