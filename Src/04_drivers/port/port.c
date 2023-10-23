#include "port.h"

void set_pinMode(pin_t pin, pin_mode_t mode)
{
    if(mode & INPUT){
        *(pin.dir_reg) &= ~(1<<pin.num);
    } else {
        *(pin.dir_reg) |= (1<<pin.num);
    }
    
    if(mode & PULLUP){
        *(pin.data_reg) |= (1<<pin.num);
    } else {
        *(pin.data_reg) &= ~(1<<pin.num);
    }
}

pin_mode_t get_pinMode(pin_t pin)
{
    if(*(pin.dir_reg) & (1<<pin.num)){
        return OUTPUT;
    }
    
    if(*(pin.data_reg) & (1<<pin.num)){
        return INPUT_PULLUP;
    } else {
        return INPUT;
    }
    
    return -1;
}

void set_pinState(pin_t pin, pin_state_t state)
{
    if(*(pin.dir_reg) & (1<<pin.num) == 0){
        return;     // you should use set_pinMode to change pullup
    }
    
    if(state == HIGH){
        *(pin.data_reg) |= (1<<pin.num);
    } else {
        *(pin.data_reg) &= ~(1<<pin.num);
    }

    return;
}

pin_state_t get_pinState(pin_t pin)
{
    return (*(pin.in_reg) & (1<<pin.num)) ? HIGH : LOW;
}
