#include "ssd1306.h"
#include "ssd1306_regs.h"
#include "i2c_master.h"
#include "font.h"

#include "hal_uart.h"

#include "debug.h"

static uint8_t ssd1306_line_num = 0;
static uint8_t ssd1306_cursor_pos = 0;

ssd1306_err_t ssd1306_send(uint8_t command_type, uint8_t command)
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;

    i2c_master_noirq_sendStart(((SSD1306_ADDR << 1) & 0xFE) | I2C_RW_WRITE);
    i2c_master_noirq_sendData(command_type);
    i2c_master_noirq_sendData(command);
    i2c_master_noirq_sendStop();
    delay_ms(1);

    return ret_val;
}

ssd1306_err_t ssd1306_sendStream(uint8_t command_type, uint8_t* stream, uint16_t stream_size)
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;
    uint32_t i = 0;
    uint8_t j = 0;

    while(i < stream_size){
        i2c_master_noirq_sendStart(((SSD1306_ADDR << 1) & 0xFE) | I2C_RW_WRITE);
        i2c_master_noirq_sendData(command_type);
        for(j = 0; j < 29 && i < stream_size; j++){
            i2c_master_noirq_sendData(stream[i]);
            i++;
        }
        i2c_master_noirq_sendStop();
        delay_ms(1);
    }

    return ret_val;
}

ssd1306_err_t ssd1306_init(uint8_t contrast)
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;
    
    /* documentation
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_DISPLAY_OFF);             // 0xAE
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_COCK_DIV);                // 0xD5
    ssd1306_send(SSD1306_COMMAND, SSD1306_CLOCK_DIV & 0x80);            // 0x80
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_MULTIPLEX_RATIO);         // 0xA8
    ssd1306_send(SSD1306_COMMAND, SSD1306_MULTIPLEX_RATIO & 0x3F);      // 0x3F
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_DISP_OFFSET);             // 0xD3
    ssd1306_send(SSD1306_COMMAND, 0x00);                                // 0x00
    ssd1306_send(SSD1306_COMMAND, SSD1306_DISPLAY_START_LINE | 0x0);    // 0x40
    ssd1306_send(SSD1306_COMMAND, SSD1306_DISPLAY_SEGMENT_REMAP_127);   // 0xA1
    ssd1306_send(SSD1306_COMMAND, SSD1306_COM_OUT_SCAN_REMAPPED);       // 0xC8
    ssd1306_send(SSD1306_COMMAND, SSD1306_COM_PINS_HARD_CONFIG);        // 0xDA
    ssd1306_send(SSD1306_COMMAND, SSD1306_HARD_COM_SEQUENTIAL | SSD1306_HARD_COM_DISABLE_REMAP);    // 0x02
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_CONTRAST_CONTROL);        // 0x81
    ssd1306_send(SSD1306_COMMAND, contrast);                            // 0xCF
    ssd1306_send(SSD1306_COMMAND, SSD1306_ENTIRE_DISPLAY_ON_RAM);       // 0xA4
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_NORMAL_DISPLAY);          // 0xA6
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_COCK_DIV);                // 0xD5
    ssd1306_send(SSD1306_COMMAND, SSD1306_CLOCK_DIV & 0x80);            // 0x80
    ssd1306_send(SSD1306_COMMAND, SSD1306_CHARGE_PUMP_SETTINGS);        // 0x8D
    ssd1306_send(SSD1306_COMMAND, SSD1306_CHARGE_PUMP_ENABLE);          // 0x14
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_DISPLAY_ON);              // 0xAF
                                                                        // */

    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_DISPLAY_OFF);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_MEMORY_ADDR_MODE);
    ssd1306_send(SSD1306_COMMAND, 0x00);//
    ssd1306_send(SSD1306_COMMAND, SSD1306_COM_OUT_SCAN_REMAPPED);
    ssd1306_send(SSD1306_COMMAND, SSD1306_DISPLAY_START_LINE | 0);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_CONTRAST_CONTROL);
    ssd1306_send(SSD1306_COMMAND, contrast);
    ssd1306_send(SSD1306_COMMAND, SSD1306_DISPLAY_SEGMENT_REMAP_127);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_NORMAL_DISPLAY);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_MULTIPLEX_RATIO);
    ssd1306_send(SSD1306_COMMAND, 63);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_DISP_OFFSET);
    ssd1306_send(SSD1306_COMMAND, 0);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_COCK_DIV);
    ssd1306_send(SSD1306_COMMAND, SSD1306_CLOCK_DIV & 0x80);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_PRE_CHARGE_PERIOD);
    ssd1306_send(SSD1306_COMMAND, 0x22);
    ssd1306_send(SSD1306_COMMAND, SSD1306_COM_PINS_HARD_CONFIG);
    ssd1306_send(SSD1306_COMMAND, SSD1306_HARD_COM_DISABLE_REMAP | SSD1306_HARD_COM_ALTERNATIVE);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_VCOMH_DES_LEVEL);
    ssd1306_send(SSD1306_COMMAND, SSD1306_VCOMH_DES_LEVEL_077);
    ssd1306_send(SSD1306_COMMAND, SSD1306_CHARGE_PUMP_SETTINGS);
    ssd1306_send(SSD1306_COMMAND, 0x14);

    ssd1306_send(SSD1306_COMMAND, SSD1306_ENTIRE_DISPLAY_ON_RAM);
    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_DISPLAY_ON);
    // ssd1306_send(SSD1306_COMMAND, SSD1306_DEACTIVATE_SCROLL);
    

    ssd1306_clear();

    return ret_val;
}

ssd1306_err_t ssd1306_clear()
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;

    ssd1306_setCursor(0, 0);

    uint8_t data[1024] = {0};
    ssd1306_sendStream(SSD1306_DATA_STREAM, data, 1024);

    ssd1306_setCursor(0, 0);

    return ret_val;
}

ssd1306_err_t ssd1306_setCursor(uint8_t line, uint8_t pos)
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;

    /* ssd1306_send(SSD1306_COMMAND, SSD1306_SET_COLUMN_ADDR);
    ssd1306_send(SSD1306_COMMAND, pos);
    ssd1306_send(SSD1306_COMMAND, 127);

    ssd1306_send(SSD1306_COMMAND, SSD1306_SET_PAGE_ADDER);
    ssd1306_send(SSD1306_COMMAND, line);
    ssd1306_send(SSD1306_COMMAND, 7);
     */

    i2c_master_noirq_sendStart(((SSD1306_ADDR << 1) & 0xFE) | I2C_RW_WRITE);
    i2c_master_noirq_sendData(SSD1306_COMMAND);
    i2c_master_noirq_sendData(SSD1306_SET_COLUMN_ADDR);
    i2c_master_noirq_sendData(pos);
    i2c_master_noirq_sendData(127);
    i2c_master_noirq_sendData(SSD1306_SET_PAGE_ADDER);
    i2c_master_noirq_sendData(line);
    i2c_master_noirq_sendData(7);
    i2c_master_noirq_sendStop();
    
    ssd1306_line_num = line;
    ssd1306_cursor_pos = pos;

    return ret_val;
}


ssd1306_err_t ssd1306_writeString(uint8_t *str)
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;
    uint8_t c;

    while(*str){
        if(ssd1306_cursor_pos + FONT_SIZE > 127 || *str == '\n'){
            ssd1306_nextLine();
        }
        c = *str;

        c -= 0x20;

        uint8_t character[FONT_SIZE];
        for(uint8_t i = 0; i < FONT_SIZE; i++){
            character[i] = font[c][i];
            ssd1306_cursor_pos++;
        }
        ssd1306_sendStream(SSD1306_DATA_STREAM, character, FONT_SIZE);
        str++;
    }

}

ssd1306_err_t ssd1306_nextLine()
{
    ssd1306_err_t ret_val = SSD1306_ERR_OK;

    ssd1306_line_num++;
    ssd1306_line_num = ssd1306_line_num % 7;

    ssd1306_setCursor(ssd1306_line_num, 0);

    return ret_val;
}