#ifndef PORT_H
#define PORT_H

// PORT definitions


#include "avr/io.h"

// TYPES ===================================

/*
 * @brief enum for possible states of a pin
 * 
 */
typedef enum {
    LOW = 0,
    HIGH = 1
} pin_state_t;

/*
 * @brief enum for possible modes of a pin
 * 
 */
typedef enum {
    INPUT = 0x0,
    OUTPUT = 0x1,
    PULLDOWN = 0x2,
    PULLUP = 0x6,
    INPUT_PULLDOWN = INPUT | PULLDOWN,
    INPUT_PULLUP = INPUT | PULLUP,
} pin_mode_t;


typedef struct 
{
    uint8_t* data_reg;
    uint8_t* dir_reg;
    uint8_t* in_reg;
    uint8_t num;
} pin_t;

// pins
const pin_t d0 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 0u
};

const pin_t d1 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 1u
};

const pin_t d2 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 2u
};

const pin_t d3 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 3u
};

const pin_t d4 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 4u
};

const pin_t d5 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 5u
};

const pin_t d6 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 6u
};

const pin_t d7 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 7u
};

const pin_t d8 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 0u
};

const pin_t d9 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 1u
};

const pin_t d10 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 2u
};

const pin_t d11 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 3u
};

const pin_t d12 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 4u
};


const pin_t d13 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 5u
};

const pin_t a0 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 0u
};

const pin_t a1 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 1u
};

const pin_t a2 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 2u
};

const pin_t a3 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 3u
};

// FUNCTIONS ===================================


/*
 * @brief Set the mode of a pin
 * 
 * @param pin pin designation (TODO: pin designation not defined yet)
 * @param mode pinmode
 */
void set_pinMode(pin_t pin, pin_mode_t mode);


/*
 * @brief Get the mode of a pin
 * 
 * @param pin pin designation (TODO: pin designation not defined yet)
 * @return pin_mode_t 
 */
pin_mode_t get_pinMode(pin_t pin);


/*
 * @brief Set the state of a pin
 * 
 * @param pin pin designation (TODO: pin designation not defined yet)
 * @param state state to be set on an output pin
 */
void set_pinState(pin_t pin, pin_state_t state);


/*
 * @brief Get the state of a pin 
 * 
 * @param pin  pin designation (TODO: pin designation not defined yet)
 * @return pin_state_t state of selected pin
 */
pin_state_t get_pinState(pin_t pin);


#endif /* PORT_H */
