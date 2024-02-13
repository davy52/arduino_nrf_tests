#ifndef SSD1306_REGS_H
#define SSD1306_REGS_H

#define SSD1306_ADDR        0x3C

#define SSD1306_COMMAND     0x00
#define SSD1306_DATA        0xC0
#define SSD1306_DATA_STREAM 0x40


// Fundamental Commands 
#define SSD1306_SET_CONTRAST_CONTROL    0x81
#define SSD1306_ENTIRE_DISPLAY_ON_RAM   0xA4
#define SSD1306_ENTIRE_DISPLAY_ON_NORAM 0xA5
#define SSD1306_SET_NORMAL_DISPLAY      0xA6
#define SSD1306_SET_INVERSE_DISPLAY     0xA7
#define SSD1306_SET_DISPLAY_OFF         0xAE
#define SSD1306_SET_DISPLAY_ON          0xAF


// Scrolling Commands
// Continious Horizontal Scroll
// 1st byte
#define SSD1306_CONTINIOUS_SCROLL_RIGHT 0x26
#define SSD1306_CONTINIOUS_SCROLL_LEFT  0x27
// 2nd byte
#define SSD1306_SCROLL_DUMMY        0x00
// 3rd byte
#define SSD1306_SCROLL_START        0x07    // AND with page num (0-7)
// 4th byte
#define SSD1306_SCROLL_INTERVAL_5   0x00
#define SSD1306_SCROLL_INTERVAL_64  0x1
#define SSD1306_SCROLL_INTERVAL_128 0x2
#define SSD1306_SCROLL_INTERVAL_256 0x3
#define SSD1306_SCROLL_INTERVAL_3   0x4
#define SSD1306_SCROLL_INTERVAL_4   0x5
#define SSD1306_SCROLL_INTERVAL_25  0x6
#define SSD1306_SCROLL_INTERVAL_2   0x7
// 5th byte
#define SSD1306_SCROLL_STOP         0x07    // AND with page num (0-7)
// 6th and 7th byte
#define SSD1306_SCROLL_6TH_BYTE     0x00
#define SSD1306_SCROLL_7TH_BYTE     0xFF

// Contrinious Vertical and Horizontal Scroll
// 1st byte
#define SSD1306_CONTINIOUS_VERT_RIGHT   0x29
#define SSD1306_CONTINIOUS_VERT_LEFT    0x2A
// 2nd byte 
// start as above
// 3rd byte
// interval as above
// 4th byte
// stop as above
// 5th byte
#define SD1306_SCROLL_VERT_OFFSET   0x3F    // AND with offset (1-63)

#define SSD1306_DEACTIVATE_SCROLL       0x2E

// Scroll type needs to be written before this command eg. [0x29 0x2F]
#define SSD1306_ACTIVATE_SCROLL         0x2F    

// Vertical Scroll Area
// 1st byte
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
// 2nd byte 
#define SSD1306_VERT_AREA_STATIC    0x3F    // AND with number of static rows
// 3rd byte 
#define SSD1306_VERT_AREA_DYNAMIC   0x7F    // AND with number of dynamic rows


// Addressing Setting Commands

// Set Lower Column Start Addres for page addressing mode
#define SSD1306_START_ADDR_LOW          0x00    // OR with lower nibble of start adder (0x0 - 0xF)

// Set Higher Column start Address
#define SSD1306_START_ADDR_HIGH         0x10    // OR with higher nibble of start adder (0x0 - 0xF)

// Set memory addressing mode
// 1st byte
#define SSD1306_SET_MEMORY_ADDR_MODE    0x20
// 2nd byte
#define SSD1306_MEMORY_MODE_HORI    0x00
#define SSD1306_MEMORY_MODE_VERTI   0x01
#define SSD1306_MEMORY_MODE_RESET   0x02

// Set column start and end address
// 1st byte
#define SSD1306_SET_COLUMN_ADDR     0x21
// 2nd byte
#define SSD1306_COLUMN_START        0x7F    // AND with col start addr (0-127)
// 3rd byte
#define SSD1306_COLUMN_END          0x7F    // AND with col end addr (0-127)

// set page start and end address
// 1st byte
#define SSD1306_SET_PAGE_ADDER          0x22
// 2nd byte
#define SSD1306_PAGE_START          0x07    // AND with page start adder (0-7)
// 3rd byte
#define SSD1306_PAGE_STOP           0x07    // AND with page end addr (0-7)

// set page start for page addressing mode
#define SSD1306_SET_PAGE_START_P_ADDR_MODE 0xB0 // OR with page adder (0-7)


// Harware configuration
#define SSD1306_DISPLAY_START_LINE       0x40    // OR with display line (0-63)

#define SSD1306_DISPLAY_SEGMENT_REMAP_0 0xA0    // col addr 0 is SEG0
#define SSD1306_DISPLAY_SEGMENT_REMAP_127 0xA1    // col addr 127 is SEG0

// set multiplex ratio
// 1st byte
#define SSD1306_SET_MULTIPLEX_RATIO     0xA8
// 2nd byte
#define SSD1306_MULTIPLEX_RATIO     0x3F    // AND with ratio (15-63)

// set com output scan direction
#define SSD1306_COM_OUT_SCAN_NORMAL     0xC0
#define SSD1306_COM_OUT_SCAN_REMAPPED   0xC8

// set display offset
// 1st byte
#define SSD1306_SET_DISP_OFFSET         0xD3
// 2nd byte
#define SSD1306_DISP_OFFSET     0x3F    // AND with vertical offset (0-63)
                                        
// set COM pins Hardware Config
// 1st byte
#define SSD1306_COM_PINS_HARD_CONFIG    0xDA
// 2nd byte
#define SSD1306_HARD_COM_SEQUENTIAL     0x02
#define SSD1306_HARD_COM_ALTERNATIVE    0x12
// or one of ^ with one of \/
#define SSD1306_HARD_COM_DISABLE_REMAP  0x00
#define SSD1306_HARD_COM_ENABLE_REMAP   0x20


// Timing and Drivind Scheme Setting Command Table

// Set Display Clock Divade ratio
// 1st byte
#define SSD1306_SET_COCK_DIV            0xD5
// 2nd byte 
#define SSD1306_CLOCK_DIV       0xFF    // AND with ratio - 1

// Set Pre-Charge Period
// 1st byte
#define SSD1306_SET_PRE_CHARGE_PERIOD   0xD9
// 2nd byte
#define SSD1306_PRE_CHARGE_PERIOD   0xFF    // AND with CLOCK

// Set VCOMH Deselect Level
// 1st byte
#define SSD1306_SET_VCOMH_DES_LEVEL     0xDB
// 2nd byte
#define SSD1306_VCOMH_DES_LEVEL_065 0x00    // 0.65 x Vcc
#define SSD1306_VCOMH_DES_LEVEL_077 0x20    // 0.77 x Vcc
#define SSD1306_VCOMH_DES_LEVEL_083 0x30    // 0.83 x Vcc

// NOP
#define SSD1306_NOP                     0xE3

// Charge Pump
// 1st byte
#define SSD1306_CHARGE_PUMP_SETTINGS    0x8D
// 2nd byte
#define SSD1306_CHARGE_PUMP_DISABLE 0x10
#define SSD1306_CHARGE_PUMP_ENABLE  0x14
// must wrtite display on after








#endif /* SSD1306_REGS_H */
