#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint-gcc.h>

typedef enum
{
    HAL_UART_ERR_OK                     = 0x00,
    HAL_UART_ERR_NOT_OK                 = 0x01,
    HAL_UART_ERR_BUFF_FULL              = 0x02,
    HAL_UART_ERR_BUFF_EMPTY             = 0x03,
    HAL_UART_ERR_NOT_ENOUGH_IN_BUFFER   = 0x04,

    HAL_UART_ERR_PARITY                 = 0x08,
    HAL_UART_ERR_DOVER                  = 0x10,
    HAL_UART_ERR_FRAME                  = 0x20
} hal_uart_err_t;

typedef enum
{
    HAL_UART_PARITY_DIS = 0u,
    HAL_UART_PARITY_EVEN = 2u,
    HAL_UART_PARITY_ODD = 3u,
} hal_uart_parity_t;

typedef enum
{
    HAL_UART_STOP_1BIT = 0u << 2,
    HAL_UART_STOP_2BIT = 1u << 2,
} hal_uart_stop_bit_t;

typedef enum
{
    HAL_UART_CHAR_5BIT = 0u << 3,
    HAL_UART_CHAR_6BIT = 1u << 3,
    HAL_UART_CHAR_7BIT = 2u << 3,
    HAL_UART_CHAR_8BIT = 3u << 3,
    HAL_UART_CHAR_9BIT = 9u << 3
} hal_uart_char_size_t;

typedef enum
{
    HAL_UART_DOUBLE_SPEED = 1 << 7
} hal_uart_double_speed_t;

// typedef enum
// {
// #if F_CPU == 8000000ul
//     HAL_UART_BAUD_2400_DOUBLE = (416u | 0x8000u),
//     HAL_UART_BAUD_4800_DOUBLE = (207u | 0x8000u),
//     HAL_UART_BAUD_9600_DOUBLE = (103u | 0x8000u),
//     HAL_UART_BAUD_19200_DOUBLE = (51u | 0x8000u),
//     HAL_UART_BAUD_38400_DOUBLE = (25u | 0x8000u),
//     HAL_UART_BAUD_57600_DOUBLE = (15u | 0x8000u),
//     HAL_UART_BAUD_115200_DOUBLE = (8u | 0x8000u),

//     HAL_UART_BAUD_2400 = 207u,
//     HAL_UART_BAUD_4800 = 103u,
//     HAL_UART_BAUD_9600 = 51u,
//     HAL_UART_BAUD_19200 = 25u,
//     HAL_UART_BAUD_38400 = 12u,
//     HAL_UART_BAUD_57600 = 8u,
//     HAL_UART_BAUD_115200 = 3u,
// #endif
// } hal_uart_baud_t;



hal_uart_err_t hal_uart_init(uint32_t f_cpu, uint16_t baud_rate, uint8_t settings, uint16_t rx_buffer_size, uint16_t tx_buffer_size);

hal_uart_err_t hal_uart_sendByte(uint8_t data);

hal_uart_err_t hal_uart_sendByte9(uint16_t data);

hal_uart_err_t hal_uart_sendBytes(uint8_t *data, uint16_t data_len);

hal_uart_err_t hal_uart_sendBytes9(uint16_t *data, uint16_t data_len);

hal_uart_err_t hal_uart_readByte(uint8_t *data);

hal_uart_err_t hal_uart_readByte9(uint16_t *data);

hal_uart_err_t hal_uart_readBytes(uint8_t *data, uint16_t data_len);

hal_uart_err_t hal_uart_readBytes9(uint16_t *data, uint16_t data_len);

hal_uart_err_t hal_uart_getRxBufferCount(uint16_t* rx_buffer_count);

hal_uart_err_t hal_uart_clearBuffers();

/**
 * @brief function for creation of response vectors (automatic response to received packets)
 * @warning works only for one-byte size frames
 * 
 * @param format array of frames to respond to (cannot be changed without re-calling this function)
 * @param format_len number of different frames to respond to
 * @param data double array pointer (array (size format_len) of arrays with data)
 * @param data_len array (size format_len) with data arrays size
 * @return hal_uart_err_t 
 */
hal_uart_err_t hal_uart_response_init(uint8_t *format, uint8_t format_len, uint8_t *data, uint16_t *data_len);

hal_uart_err_t hal_uart_getErrs();


#endif /* HAL_UART_H */
