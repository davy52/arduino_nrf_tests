
#include "bme280.h"
#include "bme280_registers.h"

#include "i2c_master.h"

#include <stdint.h>

// delay_ms
#include "debug.h"

//debug
#define BME_DEBUG 0
#if BME_DEBUG
#include "hal_uart.h"
#include <stdio.h>
#endif

typedef enum __bme280_mode_internal {
    BME280_MODE_INT_SLEEP   = 0x00,
    BME280_MODE_INT_FORCED  = 0x01,
    BME280_MODE_INT_FORCED2 = 0x02,
    BME280_MODE_INT_NORMAL  = 0x03,
} bme280_mode_internal_t;

volatile static int32_t t_fine = 0;

volatile static uint16_t dig_T1;
volatile static int16_t dig_T2;
volatile static int16_t dig_T3;

volatile static uint16_t dig_P1;
volatile static int16_t dig_P2;
volatile static int16_t dig_P3;
volatile static int16_t dig_P4;
volatile static int16_t dig_P5;
volatile static int16_t dig_P6;
volatile static int16_t dig_P7;
volatile static int16_t dig_P8;
volatile static int16_t dig_P9;

volatile static uint8_t dig_H1;
volatile static int16_t dig_H2;
volatile static uint8_t dig_H3;
volatile static int16_t dig_H4;
volatile static int16_t dig_H5;
volatile static int16_t dig_H6;

static bme280_settings_t bme280_settings;


static bme280_err_t bme280_write(uint8_t adder, uint8_t value)
{
    bme280_err_t ret_val = BME280_ERR_OK;

    uint8_t data[2] = {adder, value};
    i2c_job_t job = {
        .adder = bme280_settings.adder,
        .RW = I2C_RW_WRITE,
        .repeated_start = 0,
        .data = data,
        .data_len = 2,
        .status = I2C_ERR_BUSY
    };
    
    i2c_master_sendData(&job);
    
    delay_ms(2); // FIXME: WTF does not work without delay
    while(job.status == I2C_ERR_BUSY);
    
    if(job.status == I2C_ERR_NOT_OK){
        ret_val = BME280_ERR_NOT_OK;
        return ret_val;
    }

    return ret_val;
}

static bme280_err_t bme280_writeMultiple(uint8_t adder, uint8_t values[], uint8_t len)
{
    bme280_err_t ret_val = BME280_ERR_OK;

    uint8_t data[len * 2];

    for(int i = 0; i < len; i++){
        if(i % 2 == 0){
            data[i * 2] = adder + i;
        }
        else{
            data[(i * 2) + 1] = values[i];
        }
    }

    i2c_job_t job = {
        .adder = bme280_settings.adder,
        .RW = I2C_RW_WRITE,
        .repeated_start = 0,
        .data = data,
        .data_len = len * 2
    };

    i2c_master_sendData(&job);
    while(job.status == I2C_ERR_BUSY);

    if(job.status != I2C_ERR_OK){
        ret_val = BME280_ERR_NOT_OK;
        return ret_val;
    }
    
    return ret_val;
}

static bme280_err_t bme280_read(uint8_t adder, uint8_t len, uint8_t* data)
{
    bme280_err_t ret_val = BME280_ERR_OK;

    uint8_t wdata = adder;
    i2c_job_t job_read = {
        .adder = bme280_settings.adder,
        .RW = I2C_RW_READ,
        .repeated_start = 0,
        .data = data,
        .data_len = len
    };
    i2c_job_t job_write = {
        .adder = bme280_settings.adder,
        .RW = I2C_RW_WRITE,
        .repeated_start = 1,
        .data = &wdata,
        .data_len = 1
    };

    while(i2c_master_appendJob(&job_write) != I2C_ERR_OK);
    while(i2c_master_appendJob(&job_read) != I2C_ERR_OK);
    i2c_master_startTransaction();
    while(job_read.status == I2C_ERR_BUSY);

    if(job_read.status != I2C_ERR_OK || job_write.status != I2C_ERR_OK){
        ret_val = BME280_ERR_NOT_OK;
        return ret_val;
    }
    
    return ret_val;
}

// comprensation equations from documentation
// temperature has to always be measured, its needed for conpensation formulas
static int32_t bme280_comp_T(int32_t adc_T)
{
    int32_t var1, var2, T;

    var1 = (((adc_T >> 3) - ((int32_t)dig_T1 << 1)) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;
    
    T = ((t_fine * 5 + 128) >> 8);
    return T;
}

static uint32_t bme280_comp_P(int32_t adc_P)
{
    int64_t var1, var2, P;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8 ) + ((var1 * (int64_t)dig_P2) << 12);    
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

    if ( var1 == 0)
    {
        return 0;   // division by zero
    }
    
    P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)dig_P9) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * P) >> 19;
    P = ((P + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (uint32_t)P;
}

static uint32_t bme280_comp_H(int32_t adc_H)
{
    int32_t var1;

    var1 = (t_fine - ((int32_t)76800));
    var1 = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) + \
        ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t) dig_H6  )) >> 10) * \
        (((var1 * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + \
        ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
    
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);

    return (uint32_t)(var1 >> 12);
}

static bme280_err_t bme280_readCompValues()
{
    bme280_err_t ret_val = BME280_ERR_OK;
    uint16_t temp;

    uint8_t regs[24];
    ret_val = bme280_read(BME280_REG_CALIB00, BME280_REG_CALIB23 - BME280_REG_CALIB00 + 1, regs);

    dig_T1 = (uint16_t)(regs[0] | regs[1] << 8);
    temp = (int16_t)(regs[2] | regs[3] << 8);
    dig_T2 = *(int16_t*)&temp;
    temp = (int16_t)(regs[4] | regs[5] << 8);
    dig_T3 = *(int16_t*)&temp;

    dig_P1 = (uint16_t)(regs[6] | regs[7] << 8);
    temp = (int16_t)(regs[8] | regs[9] << 8);
    dig_P2 = *(int16_t*)&temp;
    temp = (int16_t)(regs[10] | regs[11] << 8);
    dig_P3 = *(int16_t*)&temp;
    temp = (int16_t)(regs[12] | regs[13] << 8);
    dig_P4 = *(int16_t*)&temp;
    temp = (int16_t)(regs[14] | regs[15] << 8);
    dig_P5 = *(int16_t*)&temp;
    temp = (int16_t)(regs[16] | regs[17] << 8);
    dig_P6 = *(int16_t*)&temp;
    temp = (int16_t)(regs[18] | regs[19] << 8);
    dig_P7 = *(int16_t*)&temp;
    temp = (int16_t)(regs[20] | regs[21] << 8);
    dig_P8 = *(int16_t*)&temp;
    temp = (int16_t)(regs[22] | regs[23] << 8);
    dig_P9 = *(int16_t*)&temp;

    ret_val =  bme280_read(BME280_REG_CALIB25, 1, regs);
    dig_H1 = (uint8_t)regs[0];
    
    ret_val = bme280_read(BME280_REG_CALIB26, BME280_REG_CALIB31 - BME280_REG_CALIB26 + 1, regs);
    temp = (int16_t)(regs[0] | regs[1] << 8);
    dig_H2 = *(int16_t*)&temp;
    dig_H3 = (uint8_t)(regs[2]);
    temp = (int16_t)(regs[3] << 4 | (regs[4] & 0x0F));
    dig_H4 = *(int16_t*)&temp;
    temp = (int16_t)(((regs[5] >> 4) & 0x0F) | (regs[5] << 4));
    dig_H5 = *(int16_t*)&temp;

#if BME_DEBUG
    int size;
    char str[20];

    while(hal_uart_sendBytes("digT1: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_T1, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);

    while(hal_uart_sendBytes("digT2: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_T2, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);

    while(hal_uart_sendBytes("digT3: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_T3, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP1: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P1, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP2: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P2, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP3: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P3, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP4: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P4, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP5: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P5, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP6: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P6, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP7: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P7, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP8: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P8, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digP9: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_P9, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digH1: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_H1, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digH2: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_H2, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digH3: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_H3, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digH4: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_H4, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digH5: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_H5, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    
    while(hal_uart_sendBytes("digH6: ", 7) != HAL_UART_ERR_OK);
    size = intToStr(dig_H6, str, 7);
    str[size + 1] = '\n';
    while(hal_uart_sendBytes(str, size+2) != HAL_UART_ERR_OK);
    delay_ms(500);
#endif

    return ret_val;
}

bme280_err_t bme280_init(bme280_settings_t settings)
{
    bme280_err_t ret_val = BME280_ERR_OK;
    bme280_settings = settings;
    uint8_t data;

    ret_val = bme280_readCompValues();
    if(ret_val != BME280_ERR_OK){
        return ret_val;
    }

    data = (settings.sample_delay << 5) | (settings.filter_setting << 2);
    ret_val = bme280_write(BME280_REG_CONFIG, data);
    if(ret_val != BME280_ERR_OK){
        return ret_val;
    }

    if(settings.sens_en & BME280_SENS_HUMIDITY != 0){
        data = settings.humidity_oversample;
    }
    else {
        data = 0;
    }
    ret_val = bme280_write(BME280_REG_CTRL_HUM, data);
    if(ret_val != BME280_ERR_OK){
        return ret_val;
    }
    
    data = 0;
    if(settings.sens_en & BME280_SENS_TEMP != 0){
        data |= settings.temp_oversample << 5;
    }
    else {
        // temperature needed for compensation so...
        data |= BME280_OVERSAMPLE_2 << 5;
    }
    if(settings.sens_en & BME280_SENS_PRESSURE != 0){
        data |= settings.pressure_oversample << 2;
    }
    
    if(settings.mode = BME280_MODE_CONINOUS){
        data |= BME280_MODE_INT_NORMAL;
    }
    else if(settings.mode = BME280_MODE_ON_DEMAND){
        data |= BME280_MODE_INT_SLEEP;
    }
    else {
        ret_val = BME280_ERR_NOT_OK;
        return ret_val;
    }
    
    ret_val = bme280_write(BME280_REG_CTRL_MEAS, data);
    if(ret_val != BME280_ERR_OK){
        return ret_val;
    }
    
    return ret_val;
}

bme280_err_t bme280_changeSettings(bme280_settings_t settings);

bme280_result_one_t bme280_readTemp();

bme280_result_one_t bme280_readPressure();

bme280_result_one_t bme280_readHumidity();

bme280_result_all_t bme280_readAll()
{
    bme280_result_all_t ret_val;
    ret_val.error = BME280_ERR_OK;
    uint32_t temporary;
    uint32_t temp;
    int32_t  press, humid;
    int32_t temp_comp;
    uint32_t press_comp, humid_comp;
    uint8_t data[8];
    
    if(bme280_settings.mode == BME280_MODE_ON_DEMAND){
        bme280_read(BME280_REG_STATUS, 1, data);
        if(data[0] & 0x04 != 0){
            ret_val.error = BME280_ERR_BUSY;
            return ret_val;
        }
        
    } 
    
    ret_val.error = bme280_read(BME280_REG_PRESS_MSB, BME280_REG_HUM_LSB - BME280_REG_PRESS_MSB + 1, data);
    if(ret_val.error != BME280_ERR_OK){
        return ret_val;
    }
    
    temporary = (uint32_t)(((uint32_t)(data[0]) << 12) | (uint32_t)(data[1] << 4) | (data[2] >> 4));
    press = *(int32_t*)&temporary;       //QUAKE?

    temp = (uint32_t)(((uint32_t)(data[3]) << 12) | (uint32_t)(data[4] << 4) | (data[5] >> 4));

    temporary = (uint32_t)((uint32_t)(data[6] << 8) | data[7]);
    humid = *(int32_t*)&temporary;
    
    #if BME_DEBUG
    blink_slow(1);
    #endif

    temp_comp = bme280_comp_T(temp);
    press_comp = bme280_comp_P(press);
    humid_comp = bme280_comp_H(humid);

    #if BME_DEBUG
    blink_slow(0);
    
    int size;
    char str[50];
    size = sprintf(str, "t: %d\n", temp_comp);
    while(hal_uart_sendBytes(str, size) == HAL_UART_ERR_BUFF_FULL);
    size = sprintf(str, "p: %u\n", press_comp);
    while(hal_uart_sendBytes(str, size) == HAL_UART_ERR_BUFF_FULL);
    size = sprintf(str, "h: %u\n", humid_comp);
    while(hal_uart_sendBytes(str, size) == HAL_UART_ERR_BUFF_FULL);
    delay_ms(50);
    #endif

    ret_val.temp = (float)(temp_comp/100.0f);
    ret_val.pressure = (float)(temp_comp/256.0f);
    ret_val.humidity = (float)(humid_comp/1024.0f);

    return ret_val;
}

bme280_err_t bme280_startMeasurement()
{
    bme280_err_t ret_val = BME280_ERR_OK;
    uint8_t data;
    
    if(bme280_settings.mode == BME280_MODE_CONINOUS){
        ret_val = BME280_ERR_NOT_OK;
        return ret_val;
    }

    bme280_read(BME280_REG_STATUS, 1, &data);
    if(data & 0x04 != 0){
        ret_val = BME280_ERR_BUSY;
        return ret_val;
    }

    bme280_read(BME280_REG_CTRL_MEAS, 1, &data);
    data = (data & ~0x03) | BME280_MODE_INT_FORCED;
    bme280_write(BME280_REG_CTRL_MEAS, data);

    return ret_val;
}

bme280_err_t bme280_changeMode(bme280_mode_t mode);

bme280_err_t bme280_reset()
{
    bme280_err_t ret_val = BME280_ERR_OK;

    ret_val = bme280_write(BME280_REG_RESET, BME280_RESET_CMD);
    
    return ret_val;
}