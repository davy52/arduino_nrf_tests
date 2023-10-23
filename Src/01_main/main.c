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

#include "hal_uart.h"
#include "debug.h"
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

    //init uart control
    u_control_init();

    while(1){
        // run uart control
        u_control_run();
        
        // delay_ms(200);
    }
}
