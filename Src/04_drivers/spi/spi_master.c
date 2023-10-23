#include "spi_master.h"

void spi_master_init(spi_common_int_en_t int_en, spi_master_data_order_t data_order, \
        spi_master_clock_idle_t clk_idle_polarity, spi_common_clk_phase_t clk_phase, spi_common_clk_div_t clk_div)
{
    uint8_t SPCR_new = 0;

    SPCR_new |= int_en << SPIE;
    SPCR_new |= 1u << SPE;
    SPCR_new |= data_order << DORD;
    SPCR_new |= 1u << MSTR;
    SPCR_new |= clk_idle_polarity << CPOL;
    SPCR_new |= clk_phase << CPHA;
    SPCR_new |= clk_div & 0x03;
    
    SPCR = SPCR_new;

    SPSR |= (clk_div >> 2) << SPI2X;
}

void spi_master_transmit(uint8_t data)
{
    SPDR = data;
}

uint8_t spi_master_read(void)
{
    return SPDR;
}

void spi_master_enable(spi_common_en_t enable)
{
    if(SPI_ENABLE == enable){
        SPCR |= 0x01u << SPE;
    }
    else if (SPI_DISABLE == enable)
    {
        SPCR &= ~(0x01u << SPE);
    }
}

spi_common_status_t spi_master_check_status(void)
{
    spi_common_status_t status;
    status.transfer_clomplete_flag  = (SPSR >> SPIF) & 0x01u;
    status.write_collision_flag     = (SPSR >> WCOL) & 0x01u;

    return status;
}
