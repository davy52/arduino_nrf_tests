#ifndef BME280_REGISTERS_H
#define BME280_REGISTERS_H

// registers adc [R]
#define BME280_REG_HUM_LSB      0xFE
#define BME280_REG_HUM_MSB      0xFD
#define BME280_REG_TEMP_XLSB    0xFC
#define BME280_REG_TEMP_LSB     0xFB
#define BME280_REG_TEMP_MSB     0xFA
#define BME280_REG_PRESS_XLSB   0xF9
#define BME280_REG_PRESS_LSB    0xF8
#define BME280_REG_PRESS_MSB    0xF7

// registers config [R/W]
#define BME280_REG_CONFIG       0xF5
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CTRL_HUM     0xF2

// registers status [R]
#define BME280_REG_STATUS       0xF3
#define BME280_REG_ID           0xD0

// registers calibration [R]
#define BME280_REG_CALIB00      0x88
#define BME280_REG_CALIB23      0x9F
#define BME280_REG_CALIB25      0xA1

#define BME280_REG_CALIB26      0xE1
#define BME280_REG_CALIB31      0xE6
#define BME280_REG_CALIB41      0xF0

// register reset   [W]
#define BME280_REG_RESET        0xE0
#define BME280_RESET_CMD        0xB6



#endif /* BME280_REGISTERS_H */
