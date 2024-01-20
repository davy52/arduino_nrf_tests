#ifndef NRF24L01_H
#define NRF24L01_H

#include "port.h"

typedef enum __nrf_err {
    NRF_ERR_OK,
    NRF_ERR_BUSY,
    NRF_ERR_TX_FIFO_FULL,
    NRF_ERR_TX_FIFO_EMPTY,
    NRF_ERR_RX_FIFO_FULL,
    NRF_ERR_RX_FIFO_EMPTY,

    NRF_ERR_NOT_OK
} nrf_err_t;

/**
 * @brief enable if there are spi slaves 
 *         that use diferent clock config
 *         
 *          when enabled before writing anything 
 *          spi will be configured again
 * 
 */
typedef enum __nrf_spi_multiconfig {
    NRF_SPIM_DIS = 0,
    NRF_SPIM_EN = 1
} nrf_spi_multiconfig_t;

typedef enum __nrf_data_rate {
    NRF_DRATE_250k      = 0x2,
    NRF_DRATE_1M        = 0x1,
    NRF_DRATE_2M        = 0x0
} nrf_data_rate_t;

typedef enum __nrf_power_control { 
    // values are negative
    NRF_POWER_0_DBM     = 0x3,
    NRF_POWER_6_DBM     = 0x2,
    NRF_POWER_12_DBM    = 0x1,
    NRF_POWER_18DBM     = 0x0
} nrf_power_control_t;

typedef enum __nrf_pipe {
    NRF_PIPE_P0,
    NRF_PIPE_P1,
    NRF_PIPE_P2,
    NRF_PIPE_P3,
    NRF_PIPE_P4,
    NRF_PIPE_P5
} nrf_pipe_t;

typedef enum __nrf_auto_ack {
    NRF_AUTO_ACK_EN,
    NRF_AUTO_ACK_DIS
} nrf_auto_ack_t;

typedef enum __nrf_addr_width {
    NRF_ADDR_WIDTH_3    = 0x1,
    NRF_ADDR_WIDTH_4    = 0x2,
    NRF_ADDR_WIDTH_5    = 0x3
} nrf_addr_width_t;

typedef enum __nrf_pipe_en {
    NRF_ADDR_PIPE_EN_P0 = 0x01,
    NRF_ADDR_PIPE_EN_P1 = 0x02,
    NRF_ADDR_PIPE_EN_P2 = 0x04,
    NRF_ADDR_PIPE_EN_P3 = 0x08,
    NRF_ADDR_PIPE_EN_P4 = 0x10,
    NRF_ADDR_PIPE_EN_P5 = 0x20
} nrf_pipe_en_t;

typedef uint8_t nrf_auto_rtx_delay_t;         // 250us + value * 250us [max value = 15]

typedef uint8_t nrf_rf_channel_t;       // 2400MHz + value * 1MHz [max value = 127]


typedef struct __nrf_config {
    port_pin_t nrf_ce_pin;
    port_pin_t nrf_csn_pin;
    nrf_data_rate_t data_rate;
    nrf_rf_channel_t rf_channel;
    nrf_power_control_t power;
    nrf_addr_width_t addr_width;
    nrf_auto_rtx_delay_t auto_rtx_delay;
    uint8_t dynamic_payload_len     :1;
    uint8_t en_payload_with_ack     :1;
    uint8_t crc_2bytes              :1;
    nrf_pipe_en_t pipes_en;

    nrf_spi_multiconfig_t nrf_is_multiconfig;


} nrf_config_t;


nrf_err_t nrf_init(nrf_config_t config);

nrf_err_t nrf_setupPipe(
    nrf_pipe_t pipe, 
    uint8_t *pipe_addr, 
    uint8_t pipe_addr_size, 
    nrf_auto_ack_t auto_ack, 
    uint8_t *pipe_ack, 
    uint8_t pipe_ack_len,
    uint8_t en_dynamic_payload_len
    );

nrf_err_t nrf_transmit(nrf_pipe_t pipe, uint8_t payload, uint8_t payload_len);

nrf_err_t nrf_startRX(nrf_pipe_t pipe);

nrf_err_t nrf_checkRx(uint8_t *rx_bytes);

nrf_err_t nrf_getPayload(nrf_pipe_t pipe, uint8_t payload, uint8_t payload_len);




#endif /* NRF24L01_H */
