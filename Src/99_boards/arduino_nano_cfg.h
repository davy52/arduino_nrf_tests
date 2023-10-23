#ifndef ARDUINO_NANO_CFG_H
#define ARDUINO_NANO_CFG_H

#define F_CPU 16000000UL
//#include <xc.h>
#include "avr/io.h"

// pin definitions
// analog pins
#define PORT_ANALOG PORTC
#define A0 PC0 
#define A1 PC1 
#define A2 PC2 
#define A3 PC3 
#define A4 PC4 
#define A5 PC5 
#define A6 PC6 
#define A7 PC7 

// digital pins
#define D1 
#define D2 _BV(PD2)
#define D3 _BV(PD3)
#define D4 _BV(PD4)
#define D5 _BV(PD5)
#define D6 _BV(PD6)
#define D7 _BV(PD7)
#define D8 _BV(PB0)
#define D9 _BV(PB1)
#define D10 _BV(PB2)
#define D11 _BV(PB3)
#define D12 _BV(PB4)
#define D13 _BV(PB5)


// buildins
#define LED_BUILTIN D13
#endif /* ARDUINO_NANO_CFG_H */
