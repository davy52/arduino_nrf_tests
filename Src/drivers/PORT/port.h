#ifndef PORT_H
#define PORT_H

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


// FUNCTIONS ===================================


/*
 * @brief Set the mode of a pin
 * 
 * @param pin pin designation (TODO: pin designation not defined yet)
 * @param mode pinmode
 */
void set_pinMode(int pin, pin_mode_t mode);


/*
 * @brief Get the mode of a pin
 * 
 * @param pin pin designation (TODO: pin designation not defined yet)
 * @return pin_mode_t 
 */
pin_mode_t get_pinMode(int pin);


/*
 * @brief Set the state of a pin
 * 
 * @param pin pin designation (TODO: pin designation not defined yet)
 * @param state state to be set on an output pin
 */
void set_pinState(int pin, pin_state_t state);


/*
 * @brief Get the state of a pin 
 * 
 * @param pin  pin designation (TODO: pin designation not defined yet)
 * @return pin_state_t state of selected pin
 */
pin_state_t get_pinState(int pin);


#endif /* PORT_H */
