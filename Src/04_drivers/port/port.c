#include "port.h"

#include "avr/io.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
// port B
const port_pin_t port_B0 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 0u
};

const port_pin_t port_B1 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 1u
};

const port_pin_t port_B2 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 2u
};

const port_pin_t port_B3 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 3u
};

const port_pin_t port_B4 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 4u
};


const port_pin_t port_B5 = {
    .data_reg   = &PORTB,
    .dir_reg    = &DDRB,
    .in_reg     = &PINB,
    .num        = 5u
};

// port C
const port_pin_t port_C0 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 0u
};

const port_pin_t port_C1 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 1u
};

const port_pin_t port_C2 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 2u
};

const port_pin_t port_C3 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 3u
};

const port_pin_t port_C4 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 4u
};

const port_pin_t port_C5 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 5u
};

const port_pin_t port_C6 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 6u
};

const port_pin_t port_C7 = {
    .data_reg   = &PORTC,
    .dir_reg    = &DDRC,
    .in_reg     = &PINC,
    .num        = 7u
};

// port D
const port_pin_t port_D0 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 0u
};

const port_pin_t port_D1 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 1u
};

const port_pin_t port_D2 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 2u
};

const port_pin_t port_D3 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 3u
};

const port_pin_t port_D4 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 4u
};

const port_pin_t port_D5 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 5u
};

const port_pin_t port_D6 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 6u
};

const port_pin_t port_D7 = {
    .data_reg   = &PORTD,
    .dir_reg    = &DDRD,
    .in_reg     = &PIND,
    .num        = 7u
};
#pragma GCC diagnostic pop

const port_port_t port_B[] = {
    &port_B0,
    &port_B1,
    &port_B2,
    &port_B3,
    &port_B4,
    &port_B5,
    0,          // FIXME: what if those regs dont exist?
    0
};

const port_port_t port_C[] = {
    &port_C0,
    &port_C1,
    &port_C2,
    &port_C3,
    &port_C4,
    &port_C5,
    &port_C6,
    &port_C7
};

const port_port_t port_D[] = {
    &port_D0,
    &port_D1,
    &port_D2,
    &port_D3,
    &port_D4,
    &port_D5,
    &port_D6,
    &port_D7
};


void port_set_pinMode(port_pin_t pin, port_pin_mode_t mode)
{
    if(mode & PORT_INPUT){
        *(pin.dir_reg) &= ~(1<<pin.num);
    } else {
        *(pin.dir_reg) |= (1<<pin.num);
    }
    
    if(mode & PORT_PULLUP){
        *(pin.data_reg) |= (1<<pin.num);
    } else {
        *(pin.data_reg) &= ~(1<<pin.num);
    }
}

port_pin_mode_t port_get_pinMode(port_pin_t pin)
{
    if(*(pin.dir_reg) & (1<<pin.num)){
        return PORT_OUTPUT;
    }
    
    if(*(pin.data_reg) & (1<<pin.num)){
        return PORT_INPUT_PULLUP;
    } else {
        return PORT_INPUT;
    }
    
    return -1;
}

void port_set_pinState(port_pin_t pin, port_pin_state_t state)
{
    if((*(pin.dir_reg) & (1<<pin.num)) == 0){
        return;     // you should use port_set_pinMode to change pullup
    }
    
    if(state == PORT_HIGH){
        *(pin.data_reg) |= (1<<pin.num);
    } else {
        *(pin.data_reg) &= ~(1<<pin.num);
    }

    return;
}

port_pin_state_t port_get_pinState(port_pin_t pin)
{
    return (*(pin.in_reg) & (1<<pin.num)) ? PORT_HIGH : PORT_LOW;
}

void port_toggle_pinState(port_pin_t pin)
{
    *(pin.in_reg) |= (1 << pin.num);
}

void port_set_portState(port_port_t port, port_portState_t port_state)
{
    *(port[0].data_reg) = port_state;
}

port_portState_t port_get_portState(port_port_t port)
{
    return (port_portState_t)*(port[0].in_reg);
}

void port_toggle_portState(port_port_t port, uint8_t toggleMask)
{
    *(port[0].in_reg) = toggleMask;
}