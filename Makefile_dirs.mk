# header files dir
INCLUDE_DIRS=

# add files here
INCLUDE_DIRS+=./Src/01_main

INCLUDE_DIRS+=./Src/02_apps/uart_control
INCLUDE_DIRS+=./Src/02_apps/nrf_repeater

INCLUDE_DIRS+=./Src/03_hal/nrf24l01
INCLUDE_DIRS+=./Src/03_hal/ssd1306
INCLUDE_DIRS+=./Src/03_hal/hal_uart
INCLUDE_DIRS+=./Src/03_hal/bme280

INCLUDE_DIRS+=./Src/04_drivers/port
INCLUDE_DIRS+=./Src/04_drivers/spi
INCLUDE_DIRS+=./Src/04_drivers/i2c
INCLUDE_DIRS+=./Src/04_drivers/usart
INCLUDE_DIRS+=./Src/04_drivers/adc

INCLUDE_DIRS+=./Src/05_utils/ring_buffer
INCLUDE_DIRS+=./Src/05_utils/debug

INCLUDE_DIRS+=./Src/99_boards

# DFP DIRS
INCLUDE_DIRS+=$(DFP)/include
INCLUDE_DIRS+=$(DFP)/avr/lib/gcc/avr/5.4.0/include/

