#ifndef ARDUINO_PRO_MINI_H
#define ARDUINO_PRO_MINI_H


#ifdef arduino_pro_mini_5v
    #define F_CPU 16000000UL
#endif

#ifdef arduino_pro_mini_3v3
    #define F_CPU 8000000UL
#endif

#ifndef F_CPU
    #error "define ARDUINO_BOARD and include arduino_boards.h instead of this file"
#endif  

#include "port.h"

// pin definitions
// analog pins
#define A0 port_C0 
#define A1 port_C1 
#define A2 port_C2 
#define A3 port_C3 
#define A4 port_C4 
#define A5 port_C5 
#define A6  // TODO: check what ports
#define A7

// digital pins
#define D0  port_D0
#define D1  port_D1
#define D2  port_D2
#define D3  port_D3
#define D4  port_D4
#define D5  port_D5
#define D6  port_D6
#define D7  port_D7
#define D8  port_B0
#define D9  port_B1
#define D10 port_B2
#define D11 port_B3
#define D12 port_B4
#define D13 port_B5



// buildins
#define LED_BUILTIN D13


#endif /* ARDUINO_PRO_MINI_H */
