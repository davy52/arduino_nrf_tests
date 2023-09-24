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


#define ARDUINO_BOARD arduino_nano
#include "arduino_boards.h"

#include <util/delay.h>


int main(void)
{
    DDRB |= D13;

    while(1){
        PORTB ^= D13;
        _delay_ms(500U);
    }

    
    
}
