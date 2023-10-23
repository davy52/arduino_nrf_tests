#ifndef USART_H
#define USART_H

/**
 * @file usart.h
 * @author davy52 (dkliminskip@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-16
 * 
 * @copyright Copyright (c) 2023
 * 
 * @attention for now only works with usart0 controller, SPI_MASTER might not work TODO:
 */

#include <stdint-gcc.h>
#include "avr/interrupt.h"

// #define usart_rx_int()                  ISR(USART_RX_vect)

// #define usart_tx_int() 
//     ISR(USART_TX_vect)

// #define usart_data_reg_empty_int() 
//     ISR(USART_RX_vect)

typedef enum 
{
    USART_MODE_ASYNC = 0u,
    USART_MODE_SYNC = 1u,
    USART_MODE_MASTER_SPI = 3u
} usart_mode_t;

typedef enum
{
    USART_PARITY_DIS = 0u,
    USART_PARITY_EVEN = 2u,
    USART_PARITY_ODD = 3u
} usart_parity_t;

typedef enum
{
    USART_STOP_1BIT = 0u,
    USART_STOP_2BIT = 1u
} usart_stopbit_mode_t;

typedef enum
{
    USART_CHAR_5BIT = 0u,
    USART_CHAR_6BIT = 1u,
    USART_CHAR_7BIT = 2u,
    USART_CHAR_8BIT = 3u,
    USART_CHAR_9BIT = 9u
} usart_char_size_t;

typedef enum
{
    USART_CLK_POLARITY_RISING = 0u,
    USART_CLK_POLARITY_FALLING = 1u,
} usart_clk_polarity_t;

typedef enum 
{
    USART_ERR_OK,
    USART_ERR_NOT_OK,
    USART_ERR_BAS_SETTINGS,
    USART_ERR_FRAME,
    USART_ERR_PARITY,
    USART_ERR_DATA_OVERRUN
} usart_err_t;

typedef enum
{
    USART_ERR_INT_FRAME     = 0x04,
    USART_ERR_INT_DOVER     = 0x02,
    USART_ERR_INT_PARITY    = 0x01
} usart_err_int_t;

typedef struct
{
    uint8_t double_speed : 1;
    uint8_t multi_proc_mode : 1;
    uint8_t rx_complete_int_en : 1;
    uint8_t tx_complete_int_en : 1;
    uint8_t data_reg_empty_int_en : 1;
    uint8_t receiver_en : 1;
    uint8_t transmitter_en : 1;
    usart_clk_polarity_t clk_polarity;
    usart_char_size_t char_size;
    usart_mode_t mode;
    usart_parity_t parity;
    usart_stopbit_mode_t stopbit_mode;
    uint32_t baud;
    uint32_t f_cpu;         // TODO: make it possible to use F_CPU macro instead 
} usart_settings_t;

/* BAUD RATE    for 8MHz F_CPU
    double_speed    prescaler   err [%] |   double_speed    prescaler   err [%]     BAUD RATE
    0               207         0.2     |   1               416         -0.1        2400    
    0               103         0.2     |   1               207          0.2        4800
    0               51          0.2     |   1               103          0.2        9600
    0               25          0.2     |   1               51           0.2        19200
    0               12          0.2     |   1               25           0.2        38400  
    0               8          -3.5     |   1               15           2.1        57600
    0               3           8.5     |   1               8           -3.5        115200
    REST IN DOCUMENTATION
*/

/**
 * @brief usart0 initialisation function. To be called before using usart module
 * 
 * @param settings settings for init
 * @return usart_err_t 
 */
usart_err_t usart_init(usart_settings_t settings);

/**
 * @brief usart0 initialisation readout function.
 * 
 * @param settings pointer to structure for settings
 * @return usart_err_t 
 */
usart_err_t usart_get_settings(usart_settings_t *settings);

/**
 * @brief function to write data with usart module
 * @warning Do not use inside interrupts
 * 
 * @param data data to be written 
 * @return usart_err_t 
 */
usart_err_t usart_write(uint8_t data);

/**
 * @brief function to write data with usart module. Safe to use inside interrupt
 * 
 */
 void usart_write_int(uint8_t data);

// TODO:
usart_err_t usart_write9(uint16_t data);

/**
 * @brief function to read received data on usart module
 * @warning Do not use inside interrupts
 * 
 * @param data pointer to read data into
 * @return usart_err_t 
 */
usart_err_t usart_read(uint8_t *data);

/**
 * @brief function to read received data on usart module. Safe to use inside interrupt
 * 
 */
uint8_t usart_read_int();

usart_err_int_t usart_check_err_int();

// TODO:
usart_err_t usart_read9(uint16_t *data);

void usart_clear_buff();

void usart_enableTxInt();

void usart_disableTxInt();

void usart_enableRxInt();

void usart_disableRxInt();

void usart_enableDregInt();

void usart_disableDregInt();


#endif /* USART_H */
