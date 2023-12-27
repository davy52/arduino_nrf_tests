#ifndef PORT_H
#define PORT_H

#include <stdint.h>

// TODO: add safety checks 
// dont let state be changed when pin is set to input
// same with toggling
// same with entire ports

/**
 * @brief enum for possible states of a pin
 * 
 */
typedef enum {
    PORT_LOW = 0,
    PORT_HIGH = 1
} port_pin_state_t;

/**
 * @brief enum for possible modes of a pin
 * 
 */
typedef enum {
    PORT_INPUT = 0x0,
    PORT_OUTPUT = 0x1,
    PORT_PULLDOWN = 0x2,
    PORT_PULLUP = 0x6,
    PORT_INPUT_PULLDOWN = PORT_INPUT | PORT_PULLDOWN,
    PORT_INPUT_PULLUP = PORT_INPUT | PORT_PULLUP,
} port_pin_mode_t;

typedef struct 
{
    volatile uint8_t* data_reg;
    volatile uint8_t* dir_reg;
    volatile uint8_t* in_reg;
    uint8_t num;
} port_pin_t;

typedef port_pin_t* port_port_t;

typedef uint8_t port_portState_t;

// ports
extern const port_port_t port_A[8];
extern const port_port_t port_B[8];
extern const port_port_t port_C[8];
extern const port_port_t port_D[8];

// pins
extern const port_pin_t port_A1;
extern const port_pin_t port_A2;
extern const port_pin_t port_A3;
extern const port_pin_t port_A4;
extern const port_pin_t port_A5;
extern const port_pin_t port_A6;
extern const port_pin_t port_A7;

extern const port_pin_t port_B0; 
extern const port_pin_t port_B1; 
extern const port_pin_t port_B2; 
extern const port_pin_t port_B3; 
extern const port_pin_t port_B4; 
extern const port_pin_t port_B5; 
extern const port_pin_t port_B6; 
extern const port_pin_t port_B7; 

extern const port_pin_t port_C0; 
extern const port_pin_t port_C1; 
extern const port_pin_t port_C2; 
extern const port_pin_t port_C3; 
extern const port_pin_t port_C4; 
extern const port_pin_t port_C5; 
extern const port_pin_t port_C6; 
extern const port_pin_t port_C7; 

extern const port_pin_t port_D0; 
extern const port_pin_t port_D1; 
extern const port_pin_t port_D2; 
extern const port_pin_t port_D3; 
extern const port_pin_t port_D4; 
extern const port_pin_t port_D5; 
extern const port_pin_t port_D6; 
extern const port_pin_t port_D7; 



// FUNCTIONS ===================================


/**
 * @brief Set the mode of a pin
 * 
 * @param pin pin designation 
 * @param mode pinmode
 */
void port_set_pinMode(port_pin_t pin, port_pin_mode_t mode);


/**
 * @brief Get the mode of a pin
 * 
 * @param pin pin designation 
 * @return port_pin_mode_t 
 */
port_pin_mode_t port_get_pinMode(port_pin_t pin);


/**
 * @brief Set the state of a pin
 * 
 * @param pin pin designation 
 * @param state state to be set on an output pin
 */
void port_set_pinState(port_pin_t pin, port_pin_state_t state);


/**
 * @brief Get the state of a pin 
 * 
 * @param pin  pin designation 
 * @return port_pin_state_t state of selected pin
 */
port_pin_state_t port_get_pinState(port_pin_t pin);

/**
 * @brief Toggle state of a pin
 * 
 * @param pin pin which state is to be toggled
 */
void port_toggle_pinState(port_pin_t pin);

/**
 * @brief Set state of entire port
 * 
 * @param port port which state is to be set
 */
void port_set_portState(port_port_t port, port_portState_t port_state);

/**
 * @brief Get state of entire port
 * 
 * @param port port which state is to be returned
 * @return port_portState_t port state 
 */
port_portState_t port_get_portState(port_port_t port);

/**
 * @brief Toggle port states
 * 
 * @param port port which state is to be toggled
 */
void port_toggle_portState(port_port_t port, uint8_t toggleMask);

#endif /* PORT_H */
