
#include "usart.h"

#include <stdint-gcc.h>

#include "debug.h"
typedef union
{
    struct //TODO data_overrun and parity err handler
    {
        uint8_t multi_proc_mode : 1;        // multi prcessor mode enable
        uint8_t double_speed : 1;           // double transmit speed set 
        uint8_t parity_error : 1;           // receive buffer parrity error
        uint8_t data_overrun : 1;           // rcv buffer full and new frame start detected
        uint8_t frame_error : 1;            // error in recieved frame
        uint8_t data_reg_empty : 1;         // transmit buffer ready to recieve new data (can gen int)
        uint8_t tr_complete : 1;            // whole tramsit buffer shifted out
        uint8_t rcv_complete : 1;           // there is data in rcv buffer
    } __attribute__((packed)) bits;

    uint8_t reg;

} usart_status_a_t;

typedef union 
{
    struct 
    {
        uint8_t tx_8bit : 1;                // tx 9th bit if using 9 bit char size
        uint8_t rx_8bit : 1;                // rx 9th bit if using 9 bit char size
        uint8_t char_size_2 : 1;            // char size sel bit
        uint8_t trm_en : 1;                 // transmitter enable
        uint8_t rcv_en : 1;                 // receiver enable
        uint8_t data_reg_empty_int_en : 1;  // data register empty interrupt enable
        uint8_t tx_c_int_en : 1;            // tx complete interrupt enable
        uint8_t rx_c_int_en : 1;            // rx complete interrupt enable
    } __attribute__((packed)) bits;

    uint8_t reg;

} usart_status_b_t;

typedef union 
{
    struct
    {
        uint8_t clk_polarity : 1;           // clock polarity in sync mode [ 0 - rising, 1 - falling ]
        uint8_t char_size_0 : 1;            // char size sel bit
        uint8_t char_size_1 : 1;            // char size sel bit
        uint8_t stopbit_sel : 1;            // selectr num of stop bits [ 0 - 1-bit, 1 - 2-bit ]
        uint8_t parity_0 : 1;               // parity mode bit
        uint8_t parity_1 : 1;               // parity mode bit
        uint8_t mode_0 : 1;                 // usart mode bit
        uint8_t mode_1 : 1;                 // usart mode bit
    } __attribute__((packed)) bits;
    
    uint8_t reg;
} usart_status_c_t;

/* USART MODE
    mode_1  mode_0          selected MODE
    0       0               async USART
    0       1               sync USART
    1       0               (reserved)
    1       1               Master SPI
*/

/* PARITY MODE
    parity_1    parity_0        selected mode
    0           0               disabled
    0           1               (reserved)
    1           0               en, even parity
    1           1               en, odd parity
*/

/* CHAR SIZE 
    char_size_2 char_size_1 char_size_0     CHARACTER SIZE
    0           0           0               5-bit
    0           0           1               6-bit
    0           1           0               7-bit
    0           1           1               8-bit
    1           1           1               9-bit
*/

typedef union 
{
    struct
    {
        uint8_t baud_h : 4;                   // baud rate
        uint8_t _reserved : 4;
    }__attribute__((packed)) bits;

    uint8_t reg;
} usart_baud_h_t;


typedef union 
{
    struct 
    {
        uint8_t baud_l;
    } __attribute__((packed)) bits;
    
    uint8_t reg;
} usart_baud_l_t;


/* BAUD RATE    for 8MHz F_CPU
    double_speed    baud    err [%] |   double_speed    baud    err [%] BAUD RATE
    0               207      0.2    |   1               416     -0.1    2400    
    0               103      0.2    |   1               207      0.2    4800
    0               51       0.2    |   1               103      0.2    9600
    0               25       0.2    |   1               51       0.2    19200
    0               12       0.2    |   1               25       0.2    38400  
    0               8       -3.5    |   1               15       2.1    57600
    0               3        8.5    |   1               8       -3.5    115200
    REST IN DOCUMENTATION
*/

uint32_t f_cpu;

usart_err_t usart_init(usart_settings_t settings)
{
    usart_err_t ret_val = USART_ERR_OK;
    uint16_t prescaler = 0;
    f_cpu = settings.f_cpu;
    
    if(USART_MODE_SYNC == settings.mode){
        prescaler = (settings.f_cpu / (2 * settings.baud)) - 1;
    } 
    else if(USART_MODE_ASYNC == settings.mode){
        if(settings.double_speed){
            prescaler = (settings.f_cpu / (8 * settings.baud)) - 1;
        }
        else {
            prescaler = (settings.f_cpu / (16 * settings.baud)) - 1;
        }
    }
    

    usart_status_a_t status_a = {
        .bits = {
            .rcv_complete = 0,
            .tr_complete = 0,
            .data_reg_empty = 0,
            .frame_error = 0,
            .data_overrun = 0,
            .parity_error = 0,
            .double_speed = settings.double_speed,
            .multi_proc_mode = settings.multi_proc_mode
        }
    };
    
    usart_status_b_t status_b = {
        .bits = {
            .tx_c_int_en = settings.tx_complete_int_en,
            .rx_c_int_en = settings.rx_complete_int_en,
            .data_reg_empty_int_en = settings.data_reg_empty_int_en,
            .rcv_en = settings.receiver_en,
            .trm_en = settings.transmitter_en,
            .char_size_2 = (settings.char_size & 0x4u) >> 2,
            .rx_8bit = 0,
            .tx_8bit = 0
        }
    };

    usart_status_c_t status_c = {
        .bits = {
            .mode_1 = (settings.mode & 0x2u) >> 1u, 
            .mode_0 = (settings.mode & 0x1u),
            .parity_1 = (settings.parity & 0x2u) >> 1u,
            .parity_0 = (settings.parity & 0x1u),
            .stopbit_sel = settings.stopbit_mode,
            .char_size_1 = (settings.char_size & 0x2U) >> 1,
            .char_size_0 = (settings.char_size & 0x1U),
            .clk_polarity = ((settings.mode == USART_MODE_SYNC || settings.mode == USART_MODE_MASTER_SPI) ? settings.clk_polarity : 0)
        }
    };

    usart_baud_h_t baud_h = {
        .bits = {
            ._reserved = 0,
            .baud_h = ((prescaler & 0xFF00) >> 8) & 0xF
        }
    };

    usart_baud_l_t baud_l = {
        .bits = {
            .baud_l = (uint8_t)(prescaler & 0x00FF)
        }
    };
    
    UBRR0H = baud_h.reg;
    UBRR0L = baud_l.reg;
    
    
    UCSR0A = status_a.reg;
    UCSR0B = status_b.reg;
    UCSR0C = status_c.reg;

    // DDRD |= (1<<PD1);
    // DDRD &= ~(1<<PD0);
    // PORTD &= ~((1<<PD0) | (1<<PD1));
    return ret_val;
}

usart_err_t usart_get_settings(usart_settings_t *settings)
{
    usart_err_t ret_val = USART_ERR_OK;
    uint16_t prescaler = 0;
    
    usart_status_a_t status_a = {
        .reg = UCSR0A
    };
    usart_status_b_t status_b = {
        .reg = UCSR0B
    };
    usart_status_c_t status_c = {
        .reg = UCSR0C
    };
    
    usart_settings_t temp_settings = {
        .double_speed = status_a.bits.double_speed,
        .multi_proc_mode = status_a.bits.multi_proc_mode,

        .tx_complete_int_en = status_b.bits.tx_c_int_en,
        .rx_complete_int_en = status_b.bits.rx_c_int_en,
        .data_reg_empty_int_en = status_b.bits.data_reg_empty_int_en,
        .receiver_en = status_b.bits.rcv_en,
        .transmitter_en = status_b.bits.trm_en,
        .char_size = (status_b.bits.char_size_2 << 2) | (status_c.bits.char_size_1 << 1) | (status_c.bits.char_size_0),
        .mode = (status_c.bits.mode_1 << 1) | (status_c.bits.mode_0),
        .parity = (status_c.bits.parity_1 << 1) | (status_c.bits.parity_0),
        .stopbit_mode = status_c.bits.stopbit_sel,
        .clk_polarity = status_c.bits.clk_polarity,
        .baud = 0,
        .f_cpu = f_cpu
    };

    // calc selected baud rate
    prescaler = (UBRR0H << 8) | UBRR0L;

    if(USART_MODE_SYNC == temp_settings.mode){
        temp_settings.baud = f_cpu / (2 * (prescaler + 1));
    } 
    else if(USART_MODE_ASYNC == temp_settings.mode){
        if(temp_settings.double_speed){
            temp_settings.baud = f_cpu / (8 * (prescaler + 1));
        }
        else {
            temp_settings.baud = f_cpu / (16 * (prescaler + 1));
        }
    }

    *settings = temp_settings;
    return ret_val;
}

usart_err_t usart_write(uint8_t data)
{
    usart_err_t ret_val = USART_ERR_OK;
    
    // check if recieve complete and int is disabled 
    // if((UCSR0A & (1<<UDRE0)) && !(UCSR0B & (1<<TXCIE0))){
    if((UCSR0A & (1<<UDRE0))){
        UDR0 = data;
    } else {
        ret_val = USART_ERR_NOT_OK;
    }

    return ret_val;
}

inline void usart_write_int(uint8_t data)
{
   UDR0 = data; 
}

usart_err_t usart_read(uint8_t *data)
{
    usart_err_t ret_val = USART_ERR_OK;
    
    *data = UDR0;

    return ret_val;
}

inline uint8_t usart_read_int()
{
    return UDR0;
}

inline usart_err_int_t usart_check_err_int()
{
    return (UCSR0A & ((1<<FE0)|(1<<DOR0)|(1<<UPE0)) >> 2);
}

void usart_clear_buff()
{
    uint8_t data;
    data = UDR0;
    data = UDR0;
    data = UDR0;
    
    UCSR0A &= ~(0b11111100);
}

void usart_enableTxInt()
{
    UCSR0B |= (1<<TXCIE0);
}

void usart_disableTxInt()
{
    UCSR0B &= ~(1<<TXCIE0);
}

void usart_enableRxInt()
{
    UCSR0B |= (1<<RXCIE0);
}

void usart_disableRxInt()
{
    UCSR0B &= ~(1<<RXCIE0);
}

void usart_enableDregInt()
{
    UCSR0B |= (1<<UDRIE0);
}

void usart_disableDregInt()
{
    UCSR0B &= ~(1<<UDRIE0);
}
