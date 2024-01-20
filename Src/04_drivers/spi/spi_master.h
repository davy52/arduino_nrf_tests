#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <stdint.h>

#include "spi_common.h"

/* types */
// ENUMS

typedef enum {
    SPI_LSB_FIRST = 0,
    SPI_MSB_FIRST = 1
} spi_master_data_order_t;

typedef enum {
    SPI_CLK_IDLE_LOW = 0,
    SPI_CLK_IDLE_HIGH = 1
} spi_master_clock_idle_t;


/* Public Functions */
#define spi_master_int() ISR(SPI_STC_vect)

/**
 * @brief SPI Master Init function. Must be called first before any other SPI function
 * 
 */
void spi_master_init(spi_common_int_en_t  int_en, spi_master_data_order_t data_order, \
        spi_master_clock_idle_t clk_idle_polarity,  spi_common_clk_phase_t clk_phase, spi_common_clk_div_t clk_div);

// TODO: ?compare spi config with arguments to check if reconfiguration is needed
//          dont know if its needed or if its faster to just change config every time you start transmitting and have 
//          multiple spi slaves that use different configurations
uint8_t spi_master_compareConfig();

/**
 * @brief send data and return what has been recieved
 * 
 * @param data  transmit byte
 * @return uint8_t recieve byte
 */
uint8_t spi_master_transmit(uint8_t data);


/**
 * @brief enable or disable SPI module
 * 
 * @param enable desired state 
 */
void spi_master_enable(spi_common_en_t enable);

/**
 * @brief check status bits of SPI module
 * 
 * @return spi_common_status_t structure with status bits
 */
spi_common_status_t spi_master_check_status(void);


#endif /* SPI_MASTER_H */
