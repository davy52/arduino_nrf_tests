#ifndef DEBUG_H
#define DEBUG_H

#include "port.h"

void delay_ms(uint32_t ms);


void blink(uint8_t n);

void blink_pin(port_pin_t pin);

void blink_dur(uint8_t n, uint32_t dur);

void blink_reg(uint8_t reg);

void blink_b32(uint32_t b32);

void blink_slow(uint8_t state);

void itobin(int32_t value, uint8_t data[]);



#endif /* DEBUG_H */
