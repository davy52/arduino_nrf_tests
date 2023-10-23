#ifndef SPI_COMMON_H
#define SPI_COMMON_H

#include <stdint.h>

/* types */
// ENUMS

typedef enum {
    CLK_PHASE_LEADING = 0,
    CLK_PHASE_TRAILING = 1
} spi_common_clk_phase_t;

typedef enum {
    SPI_DIV_2 = 0x04U,
    SPI_DIV_4 = 0x00U,
    SPI_DIV_8 = 0x05U,
    SPI_DIV_16 = 0x01U,
    SPI_DIV_32 = 0x06U,
    SPI_DIV_64 = 0x02U,
//  SPI_DIV_64 = 0x07U,         // 2 possible combinations
    SPI_DIV_128 = 0x03U
} spi_common_clk_div_t;

typedef enum {
    SPI_INT_ENABLE = 0x01U,
    SPI_INT_DISABLE = 0x00U
} spi_common_int_en_t;

typedef enum {
    SPI_ENABLE = 0x01U,
    SPI_DISABLE = 0x00U
} spi_common_en_t;

typedef struct {
    uint8_t transfer_clomplete_flag : 1;
    uint8_t write_collision_flag : 1;
} spi_common_status_t;

typedef enum {
    E_OK = 0,
    E_SET_ALREADY = 1
} spi_common_error_t;

#endif /* SPI_COMMON_H */
