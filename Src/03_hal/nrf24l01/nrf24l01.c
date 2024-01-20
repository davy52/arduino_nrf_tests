
#include "nrf24l01.h"
#include "nrf24l01_internal.h"

#include "stdint.h"

#include "spi_master.h"


static nrf_err_t nrf_writeReg(uint8_t command, uint8_t data[], uint8_t data_len)
{
    nrf_err_t ret_val = NRF_ERR_OK;
    uint8_t iter = 0;
    uint8_t status;

    status = spi_master_transmit(command);
    
    if(status & NRF_TX_FULL > 0){
        ret_val = NRF_ERR_TX_FIFO_FULL;
        return ret_val;
    }

    for(iter = 0; iter < data_len; iter++){
        spi_master_transmit(data[iter]);
    }
    
    return ret_val;
}

static nrf_err_t nrf_readReg(uint8_t command, uint8_t data[], uint8_t data_len)
{
    nrf_err_t ret_val = NRF_ERR_OK;
    uint8_t iter = 0;
    uint8_t status;

    status = spi_master_transmit(command);

    if(status & NRF_RX_EMPTY > 0){
        ret_val = NRF_RX_EMPTY;
        return ret_val;
    }
    
    for(iter = 0; iter < data_len; iter++){
        spi_master_transmit(data[iter]);
    }
    
    return ret_val;
}

static nrf_err_t nrf_hybrid(uint8_t command, uint8_t status, uint8_t data[], uint8_t data_len)
{
    nrf_err_t ret_val = NRF_ERR_OK;
    uint8_t iter = 0;

    status = spi_master_transmit(command);
    
    for(iter = 0; iter < data_len; iter++){
        data[iter] = spi_master_transmit(data[iter]);
    }
    
    return ret_val;
}

static uint8_t nrf_getStatus()
{
    return spi_master_transmit(NRF_COMM_NOP);
}

static inline void nrf_setupSPI()
{
    // TODO: div might be to small for logic analyzer 
    spi_master_init(SPI_INT_DISABLE, SPI_MSB_FIRST, SPI_CLK_IDLE_LOW, SPI_CLK_PHASE_TRAILING, SPI_DIV_16);
}


nrf_err_t nrf_init(nrf_config_t config)
{
    nrf_err_t ret_val = NRF_ERR_OK;
    uint8_t data[5];

    
    nrf_setupSPI();

    data[0] = nrf_getStatus();
    if(data[0] != 0x08){
        ret_val = NRF_ERR_NOT_OK;
        return ret_val;
    }

    data[0] = (1 << NRF_EN_CRC) | (config.crc_2bytes << NRF_CRCO) | (1 << NRF_PWR_UP);
    ret_val = (NRF_COMM_W_REGISTER | NRF_REG_CONFIG, data, 1);
    if(ret_val != NRF_ERR_OK){
        return ret_val;
    }
    
    data[0] = config.pipes_en;
    ret_val = nrf_writeReg(NRF_COMM_W_REGISTER | NRF_REG_CONFIG, data, 1);
    if(ret_val != NRF_ERR_OK){
        return ret_val;
    }
    
    

}

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




