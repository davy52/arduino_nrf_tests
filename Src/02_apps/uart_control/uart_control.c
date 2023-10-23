#include "uart_control.h"

#include <stdint-gcc.h>

#include "hal_uart.h"
#include "arduino_boards.h"

#include "debug.h"

typedef enum
{
    STATE_IDLE              = 0x00u,
    STATE_GET_COMMAND       = 0x01u,
    STATE_GET_PAYLOAD       = 0x02u, 
    STATE_PARSE_PAYLOAD     = 0x03u,
    STATE_ACK               = 0x04u,
    
    STATE_ERR               = 0x80u

} u_control_state_t;

typedef enum
{
    COMMAND_NONE        = 0u,
    COMMAND_SET_PIN_DIR = 0b10000001u,
    COMMAND_CLR_PIN_DIR = 0b10000011u,
    COMMAND_GET_PIN_DIR = 0b10000000u,
    COMMAND_SET_PORT    = 0b01000001u,
    COMMAND_CLR_PORT    = 0b01000011u,
    COMMAND_GET_PORT    = 0b01000000u,
    COMMAND_TOGGLE_PORT = 0b00100000u
} u_control_command_t;

typedef enum
{
    P_SIZE_SET_PIN_DIR  = 2,
    P_SIZE_GET_PIN_DIR  = 1,
    P_SIZE_SET_PORT     = 2, 
    P_SIZE_GET_PORT     = 1,
    P_SIZE_TOGGLE_PIN   = 2
} u_control_payload_size_t;

typedef enum
{
    TRANSLATE_PORTB = 0x01,
    TRANSLATE_PORTC = 0x02,
    TRANSLATE_PORTD = 0x04

} u_control_PORT_tranlate_t;

static u_control_state_t state;
static u_control_command_t command;
static uint8_t payload_buffer[8];
static uint8_t ack_buffer[8];
static uint8_t ack_len;

static inline void u_control_getCommand(uint16_t buffer_count)
{
    if (1 > buffer_count){
        state = STATE_GET_COMMAND;
        return;
    }

    if(HAL_UART_ERR_OK != hal_uart_readByte((uint8_t*)&command)){
        return;
    }

    ack_buffer[0] = command;
    state = STATE_GET_PAYLOAD;
    ack_len = 0;
    return;
}

static inline void u_control_getPayload(uint16_t buffer_count)
{
    switch (command)
    {
    case COMMAND_GET_PIN_DIR:
            if(HAL_UART_ERR_OK != hal_uart_readBytes(payload_buffer, P_SIZE_GET_PIN_DIR)){
                return;
            }
        break;
    
    case COMMAND_SET_PIN_DIR:
    case COMMAND_CLR_PIN_DIR:
            if(HAL_UART_ERR_OK != hal_uart_readBytes(payload_buffer, P_SIZE_SET_PIN_DIR)){
                return;
            }
        break;

    case COMMAND_GET_PORT:
            if(HAL_UART_ERR_OK != hal_uart_readBytes(payload_buffer, P_SIZE_GET_PORT)){
                return;
            }
        break;
    
    case COMMAND_SET_PORT:
    case COMMAND_CLR_PORT:
            if(HAL_UART_ERR_OK != hal_uart_readBytes(payload_buffer, P_SIZE_SET_PORT)){
                return;
            }
        break;
    
    case COMMAND_TOGGLE_PORT:
            if(HAL_UART_ERR_OK != hal_uart_readBytes(payload_buffer, P_SIZE_TOGGLE_PIN)){
                return;
            }
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_PARSE_PAYLOAD;
    return;
}

static inline void u_comand_getPinDir()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        ack_buffer[1] = DDRB;
        break;
    
    case TRANSLATE_PORTC:
        ack_buffer[1] = DDRC;
        break;
    
    case TRANSLATE_PORTD:
        ack_buffer[1] = DDRD;
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 2;
    return;
}

static inline void u_comand_setPinDir()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        DDRB |= payload_buffer[1];
        break;
    
    case TRANSLATE_PORTC:
        DDRC |= payload_buffer[1];
        break;
    
    case TRANSLATE_PORTD:
        DDRD |= payload_buffer[1];
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 1;
    return;
}

static inline void u_comand_clrPinDir()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        DDRB &= ~(payload_buffer[1]);
        break;
    
    case TRANSLATE_PORTC:
        DDRC &= ~(payload_buffer[1]);
        break;
    
    case TRANSLATE_PORTD:
        DDRD &= ~(payload_buffer[1]);
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 1;
    return;
}

static inline void u_comand_getPort()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        ack_buffer[1] = PINB;
        break;
    
    case TRANSLATE_PORTC:
        ack_buffer[1] = PINC;
        break;
    
    case TRANSLATE_PORTD:
        ack_buffer[1] = PIND;
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 2;
    return;
}

static inline void u_comand_setPort()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        PORTB |= payload_buffer[1];
        break;
    
    case TRANSLATE_PORTC:
        PORTC |= payload_buffer[1];
        break;
    
    case TRANSLATE_PORTD:
        PORTD |= payload_buffer[1];
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 1;
    return;
}

static inline void u_comand_clrPort()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        PORTB &= ~(payload_buffer[1]);
        break;
    
    case TRANSLATE_PORTC:
        PORTC &= ~(payload_buffer[1]);
        break;
    
    case TRANSLATE_PORTD:
        PORTD &= ~(payload_buffer[1]);
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 1;
    return;
}

static inline void u_comand_togglePort()
{
    switch (payload_buffer[0])
    {
    case TRANSLATE_PORTB:
        PINB |= payload_buffer[1];
        break;
    
    case TRANSLATE_PORTC:
        PINC |= payload_buffer[1];
        break;
    
    case TRANSLATE_PORTD:
        PIND |= payload_buffer[1];
        break;
    
    default:
        state = STATE_ERR;
        return;
        break;
    }

    state = STATE_ACK;
    ack_len = 1;
    return;
}

static inline void u_control_parsePayload()
{
    switch (command)
    {
    case COMMAND_GET_PIN_DIR:
        u_comand_getPinDir();
        break;
    
    case COMMAND_SET_PIN_DIR:
        u_comand_setPinDir();
        break;
    
    case COMMAND_CLR_PIN_DIR:
        u_comand_clrPinDir();
        break;
    
    case COMMAND_GET_PORT:
        u_comand_getPort();
        break;
    
    case COMMAND_SET_PORT:
        u_comand_setPort();
        break;
    
    case COMMAND_CLR_PORT:
        u_comand_clrPort();
        break;
    
    case COMMAND_TOGGLE_PORT:
        u_comand_togglePort();
        break;
    
    default:                // shouldn't happen
        state = STATE_ERR;
        return;
        break;
    }
    

    return;
}

static inline void u_control_ack()
{

    if(HAL_UART_ERR_OK != hal_uart_sendBytes(ack_buffer, ack_len)){
        return;
    }
    
    state = STATE_IDLE;
    ack_len = 0;
    return;
}

static inline void u_contol_err()
{
    uint8_t data[3] = {0xff, command, hal_uart_getErrs() >> 7};
    if(HAL_UART_ERR_OK != hal_uart_sendBytes(data, 3)){
        return;
    }
    
    state = STATE_IDLE;
    ack_len = 0;
    return;
}

void u_control_init()
{
    while(HAL_UART_ERR_OK != hal_uart_init(F_CPU, 9600u, HAL_UART_CHAR_8BIT | HAL_UART_PARITY_EVEN | HAL_UART_STOP_1BIT, 16u, 16u));

    state = STATE_IDLE;
    command = COMMAND_NONE;

    hal_uart_clearBuffers();

    return;
}

void u_control_run()
{
    uint16_t buffer_count;
    hal_uart_getRxBufferCount(&buffer_count);
    //hal_uart_sendByte(state | 0x80);

    switch (state)
    {
    case STATE_IDLE:
    case STATE_GET_COMMAND:
        u_control_getCommand(buffer_count);
        break;

    case STATE_GET_PAYLOAD:
        u_control_getPayload(buffer_count);
        break;

    case STATE_PARSE_PAYLOAD:
        u_control_parsePayload();
        break;

    case STATE_ACK:
        u_control_ack();
        break;

    case STATE_ERR:
        u_contol_err();
        break;
    
    default:
        // should not happen
        break;
    }
}
