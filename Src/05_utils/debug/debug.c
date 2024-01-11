#include "arduino_boards.h"

#include <util/delay.h>
#include "debug.h"
#ifndef LED_BUILTIN
#define LED_BUILTIN _BV(PB5)
#endif

void delay_ms(uint32_t ms){
    while(ms--){
        _delay_ms(1);
    }
}

void blink(uint8_t n)
{
    delay_ms(5);
    for(int i = 0; i < n; i++){
        port_toggle_pinState(LED_BUILTIN);
        delay_ms(1);
        port_toggle_pinState(LED_BUILTIN);
        delay_ms(1);
    }
}

void blink_dur(uint8_t n, uint32_t dur)
{
    delay_ms(5);
    for(uint32_t i = 0; i < n; i++){
        port_toggle_pinState(LED_BUILTIN);
        delay_ms(dur);
        port_toggle_pinState(LED_BUILTIN);
        delay_ms(1);
    }
    
}

void blink_reg(uint8_t reg)
{
    delay_ms(5);
    blink_dur(1, 2);
    for(int i =0; i < 8; i++){
        // PORTB &= ((reg >> i) & 1) ? 0 : LED_BUILTIN; // negative logic?
        port_set_pinState(LED_BUILTIN, (~(reg >> i) & 1));
        delay_ms(1);
        // PORTB |= (LED_BUILTIN);
        port_set_pinState(LED_BUILTIN, 1);
        delay_ms(1);
    }
    blink_dur(1, 2);
}

void blink_b32(uint32_t b32)
{
    delay_ms(5);
    port_set_pinState(LED_BUILTIN, 1);

    for(uint8_t i = 0; i < 32; i++){
        port_set_pinState(LED_BUILTIN, 0);
        delay_ms(((b32 >> (31 - i)) & 1) ? 2 : 1);
        port_set_pinState(LED_BUILTIN, 1);
        delay_ms(1);
    }
    delay_ms(2);
    port_set_pinState(LED_BUILTIN, 0);
}

void blink_slow(uint8_t state)
{
    port_set_pinState(LED_BUILTIN, state);
}