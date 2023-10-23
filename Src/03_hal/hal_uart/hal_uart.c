
#include "hal_uart.h"
#include "usart.h"
#include "ring_buffer.h"

#include "debug.h"
typedef enum
{
    HAL_UART_IDLE,
    HAL_UART_RUNNING
} hal_uart_state_t;

static ring_buffer_handle_t tx_buffer_handle;
static ring_buffer_handle_t rx_buffer_handle;


static hal_uart_state_t hal_uart_state;

static inline hal_uart_err_t hal_uart_write_byte()
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    uint8_t data = 0;

    if (RB_ERR_OK != rb_pop(tx_buffer_handle, &data)){
        hal_uart_state = HAL_UART_IDLE;
        usart_disableTxInt();
        ret_val = HAL_UART_ERR_NOT_OK;
        return ret_val;
    }
    
    if (USART_ERR_OK != usart_write(data)){
        ret_val = HAL_UART_ERR_NOT_OK;
        return ret_val;
    }
    hal_uart_state = HAL_UART_RUNNING;
    usart_enableTxInt();
    
    return ret_val; 
}

hal_uart_err_t hal_uart_init(uint32_t f_cpu, uint16_t baud_rate, uint8_t settings, uint16_t rx_buffer_size, uint16_t tx_buffer_size)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    uint8_t char_size = (uint8_t)(settings & (7<<3)) >> 3; 
    uint8_t stopbit_mode = (uint8_t)((settings & (1<<2)) >> 2);
    uint8_t parity = (uint8_t)(settings & 0x3); 

    uint8_t double_speed = (uint8_t)((settings & 0x80) >> 7);

    usart_settings_t usart_settings = {
        .char_size = 0,
        .clk_polarity = 0,
        .data_reg_empty_int_en = 0,
        .rx_complete_int_en = 1,
        .tx_complete_int_en = 0,
        .double_speed = 0,
        .mode = USART_MODE_ASYNC,
        .multi_proc_mode = 0,
        .parity = 0,
        .baud = 0,
        .receiver_en = 1,
        .transmitter_en = 1,
        .stopbit_mode = 0,
        .f_cpu = 0,
    };
    
    usart_settings.char_size = char_size;
    usart_settings.stopbit_mode = stopbit_mode;
    usart_settings.parity = parity;

    usart_settings.double_speed = double_speed;
    usart_settings.baud = baud_rate;
    usart_settings.f_cpu = f_cpu;

    
    if(RB_ERR_OK != rb_init(&tx_buffer_handle, tx_buffer_size)){
        ret_val = HAL_UART_ERR_NOT_OK;
    }

    if(RB_ERR_OK != rb_init(&rx_buffer_handle, rx_buffer_size)){
        ret_val = HAL_UART_ERR_NOT_OK;
    }
    
    hal_uart_state = HAL_UART_IDLE;

    if(USART_ERR_OK != usart_init(usart_settings)){
        ret_val = HAL_UART_ERR_NOT_OK;
    }

    return ret_val;
}

hal_uart_err_t hal_uart_sendByte(uint8_t data)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    if(RB_ERR_OK != rb_insert(tx_buffer_handle, data)){
        ret_val = HAL_UART_ERR_BUFF_FULL;
        return ret_val;
    } 
    
    if (HAL_UART_RUNNING != hal_uart_state){
        ret_val = hal_uart_write_byte();
        if (HAL_UART_ERR_OK != ret_val){
            return ret_val;
        }
    }

    return ret_val;
}


hal_uart_err_t hal_uart_sendBytes(uint8_t *data, uint16_t data_len)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;

    if(RB_ERR_OK != rb_insertMultiple(tx_buffer_handle, data, data_len)){
        ret_val = HAL_UART_ERR_BUFF_FULL;
        return ret_val;
    } 
    
    if (HAL_UART_RUNNING != hal_uart_state){
        ret_val = hal_uart_write_byte();
        if (HAL_UART_ERR_OK != ret_val){
            return ret_val;
        }
    }

    return ret_val;
}

hal_uart_err_t hal_uart_readByte(uint8_t *data)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;
    
    if(RB_ERR_OK != rb_pop(rx_buffer_handle, data)){
        ret_val = HAL_UART_ERR_BUFF_EMPTY;
        return ret_val;
    }

    return ret_val;
}

hal_uart_err_t hal_uart_readBytes(uint8_t *data, uint16_t data_len)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;
    rb_err_t rb_err;
    
    if (rb_spaceUsed(rx_buffer_handle) < data_len){
        ret_val = HAL_UART_ERR_BUFF_EMPTY;
        return ret_val;
    }
    
    rb_err = rb_popMultiple(rx_buffer_handle, data, data_len);
    if (RB_ERR_OK != rb_err){
        switch (rb_err)
        {
        case RB_ERR_EMPTY:
            ret_val = HAL_UART_ERR_BUFF_EMPTY;
            break;
        
        case RB_ERR_NOT_ENOUGH_DATA:
            ret_val = HAL_UART_ERR_NOT_ENOUGH_IN_BUFFER;
            break;
            
        default:
            ret_val = HAL_UART_ERR_NOT_OK;
            break;
        }
        return ret_val;
    }

    return ret_val;
}

hal_uart_err_t hal_uart_getRxBufferCount(uint16_t *rx_buffer_count)
{
    hal_uart_err_t ret_val = HAL_UART_ERR_OK;
    *rx_buffer_count = rb_spaceUsed(rx_buffer_handle);
    return ret_val;
}

hal_uart_err_t hal_uart_clearBuffers()
{
    usart_err_t ret_val = USART_ERR_OK;

    usart_disableRxInt();
    usart_disableTxInt();

    usart_clear_buff();
    rb_clear(rx_buffer_handle);
    rb_clear(tx_buffer_handle);

    usart_enableRxInt();
    usart_enableTxInt();

    return ret_val;
}

hal_uart_err_t hal_uart_response_init(uint8_t *format, uint8_t format_len, uint8_t *data, uint16_t *data_len)
{
    return 0;
}

hal_uart_err_t hal_uart_getErrs()
{
    return (hal_uart_err_t)(usart_check_err_int() << 7);
}

// usart_data_reg_empty_int()
//ISR(USART_UDRE_vect)
ISR(USART_TX_vect)
{

    uint8_t data = 0;
    if (RB_ERR_OK != rb_pop(tx_buffer_handle, &data)){
        usart_disableTxInt();
        hal_uart_state = HAL_UART_IDLE;
        return;
    }
    
    UCSR0A |= (1<<TXC0);
    usart_write_int(data);
    return;
}

// usart_rx_int()
ISR(USART_RX_vect)
{
    
    rb_insert(rx_buffer_handle, usart_read_int());
    UCSR0A |= (1<<TXC0);
    return;
}
