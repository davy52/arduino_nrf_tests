#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <stdint.h>
#include "avr/io.h"

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

/**
 * @brief 
 * @param data 
 */
void spi_master_transmit(uint8_t data);

/**
 * @brief read bytes transmitted by slave
 * 
 * @return uint8_t byte transmitted by slave
 */
uint8_t spi_master_read(void);

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
