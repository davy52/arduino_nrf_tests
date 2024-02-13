#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

typedef enum __ssd1306_err {
    SSD1306_ERR_OK,
    SSD1306_ERR_BUSY,

    SSD1306_ERR_NOT_OK
} ssd1306_err_t;

ssd1306_err_t ssd1306_init(uint8_t contrast);

ssd1306_err_t ssd1306_clear();

ssd1306_err_t ssd1306_setCursor(uint8_t line, uint8_t pos);

ssd1306_err_t ssd1306_writeString(uint8_t *str);

ssd1306_err_t ssd1306_nextLine();


#endif /* SSD1306_H */
