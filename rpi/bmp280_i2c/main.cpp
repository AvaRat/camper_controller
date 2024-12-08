/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include "i2c_scan.h"
//#include "ina3221/ina3221.h"
#include "ina3221_cpp/INA3221.h"


 /* Example code to talk to a BMP280 temperature and pressure sensor

    NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
    GPIO (and therefore I2C) cannot be used at 5v.

    You will need to use a level shifter on the I2C lines if you want to run the
    board at 5v.

    Connections on Raspberry Pi Pico board, other boards may vary.

    GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on BMP280
    board
    GPIO PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7)) -> SCL on
    BMP280 board
    3.3v (pin 36) -> VCC on BMP280 board
    GND (pin 38)  -> GND on BMP280 board
 */

 // device has default bus address of 0x76
#define ADDR _u(0x77)

// hardware registers
#define REG_CONFIG _u(0xF5)
#define REG_CTRL_MEAS _u(0xF4)
#define REG_RESET _u(0xE0)

#define REG_TEMP_XLSB _u(0xFC)
#define REG_TEMP_LSB _u(0xFB)
#define REG_TEMP_MSB _u(0xFA)

#define REG_PRESSURE_XLSB _u(0xF9)
#define REG_PRESSURE_LSB _u(0xF8)
#define REG_PRESSURE_MSB _u(0xF7)

// calibration registers
#define REG_DIG_T1_LSB _u(0x88)
#define REG_DIG_T1_MSB _u(0x89)
#define REG_DIG_T2_LSB _u(0x8A)
#define REG_DIG_T2_MSB _u(0x8B)
#define REG_DIG_T3_LSB _u(0x8C)
#define REG_DIG_T3_MSB _u(0x8D)
#define REG_DIG_P1_LSB _u(0x8E)
#define REG_DIG_P1_MSB _u(0x8F)
#define REG_DIG_P2_LSB _u(0x90)
#define REG_DIG_P2_MSB _u(0x91)
#define REG_DIG_P3_LSB _u(0x92)
#define REG_DIG_P3_MSB _u(0x93)
#define REG_DIG_P4_LSB _u(0x94)
#define REG_DIG_P4_MSB _u(0x95)
#define REG_DIG_P5_LSB _u(0x96)
#define REG_DIG_P5_MSB _u(0x97)
#define REG_DIG_P6_LSB _u(0x98)
#define REG_DIG_P6_MSB _u(0x99)
#define REG_DIG_P7_LSB _u(0x9A)
#define REG_DIG_P7_MSB _u(0x9B)
#define REG_DIG_P8_LSB _u(0x9C)
#define REG_DIG_P8_MSB _u(0x9D)
#define REG_DIG_P9_LSB _u(0x9E)
#define REG_DIG_P9_MSB _u(0x9F)

// number of calibration registers to be read
#define NUM_CALIB_PARAMS 24

struct bmp280_calib_param {
    // temperature params
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    // pressure params
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
};

#ifdef i2c_default
void bmp280_init() {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // 500ms sampling time, x16 filter
    const uint8_t reg_config_val = ((0x04 << 5) | (0x05 << 2)) & 0xFC;

    // send register number followed by its corresponding value
    buf[0] = REG_CONFIG;
    buf[1] = reg_config_val;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // osrs_t x1, osrs_p x4, normal mode operation
    const uint8_t reg_ctrl_meas_val = (0x01 << 5) | (0x03 << 2) | (0x03);
    buf[0] = REG_CTRL_MEAS;
    buf[1] = reg_ctrl_meas_val;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void bmp280_read_raw(int32_t* temp, int32_t* pressure) {
    // BMP280 data registers are auto-incrementing and we have 3 temperature and
    // pressure registers each, so we start at 0xF7 and read 6 bytes to 0xFC
    // note: normal mode does not require further ctrl_meas and config register writes

    uint8_t buf[6];
    uint8_t reg = REG_PRESSURE_MSB;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 6, false);  // false - finished with bus

    // store the 20 bit read in a 32 bit signed integer for conversion
    *pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    *temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
}

void bmp280_reset() {
    // reset the device with the power-on-reset procedure
    uint8_t buf[2] = { REG_RESET, 0xB6 };
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

// intermediate function that calculates the fine resolution temperature
// used for both pressure and temperature conversions
int32_t bmp280_convert(int32_t temp, struct bmp280_calib_param* params) {
    // use the 32-bit fixed point compensation implementation given in the
    // datasheet
    
    int32_t var1, var2;
    var1 = ((((temp >> 3) - ((int32_t)params->dig_t1 << 1))) * ((int32_t)params->dig_t2)) >> 11;
    var2 = (((((temp >> 4) - ((int32_t)params->dig_t1)) * ((temp >> 4) - ((int32_t)params->dig_t1))) >> 12) * ((int32_t)params->dig_t3)) >> 14;
    return var1 + var2;
}

int32_t bmp280_convert_temp(int32_t temp, struct bmp280_calib_param* params) {
    // uses the BMP280 calibration parameters to compensate the temperature value read from its registers
    int32_t t_fine = bmp280_convert(temp, params);
    return (t_fine * 5 + 128) >> 8;
}

int32_t bmp280_convert_pressure(int32_t pressure, int32_t temp, struct bmp280_calib_param* params) {
    // uses the BMP280 calibration parameters to compensate the pressure value read from its registers

    int32_t t_fine = bmp280_convert(temp, params);

    int32_t var1, var2;
    uint32_t converted = 0.0;
    var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)params->dig_p6);
    var2 += ((var1 * ((int32_t)params->dig_p5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)params->dig_p4) << 16);
    var1 = (((params->dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)params->dig_p2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)params->dig_p1)) >> 15);
    if (var1 == 0) {
        return 0;  // avoid exception caused by division by zero
    }
    converted = (((uint32_t)(((int32_t)1048576) - pressure) - (var2 >> 12))) * 3125;
    if (converted < 0x80000000) {
        converted = (converted << 1) / ((uint32_t)var1);
    } else {
        converted = (converted / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)params->dig_p9) * ((int32_t)(((converted >> 3) * (converted >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(converted >> 2)) * ((int32_t)params->dig_p8)) >> 13;
    converted = (uint32_t)((int32_t)converted + ((var1 + var2 + params->dig_p7) >> 4));
    return converted;
}

void bmp280_get_calib_params(struct bmp280_calib_param* params) {
    // raw temp and pressure values need to be calibrated according to
    // parameters generated during the manufacturing of the sensor
    // there are 3 temperature params, and 9 pressure params, each with a LSB
    // and MSB register, so we read from 24 registers

    uint8_t buf[NUM_CALIB_PARAMS] = { 0 };
    uint8_t reg = REG_DIG_T1_LSB;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    // read in one go as register addresses auto-increment
    i2c_read_blocking(i2c_default, ADDR, buf, NUM_CALIB_PARAMS, false);  // false, we're done reading

    // store these in a struct for later use
    params->dig_t1 = (uint16_t)(buf[1] << 8) | buf[0];
    params->dig_t2 = (int16_t)(buf[3] << 8) | buf[2];
    params->dig_t3 = (int16_t)(buf[5] << 8) | buf[4];

    params->dig_p1 = (uint16_t)(buf[7] << 8) | buf[6];
    params->dig_p2 = (int16_t)(buf[9] << 8) | buf[8];
    params->dig_p3 = (int16_t)(buf[11] << 8) | buf[10];
    params->dig_p4 = (int16_t)(buf[13] << 8) | buf[12];
    params->dig_p5 = (int16_t)(buf[15] << 8) | buf[14];
    params->dig_p6 = (int16_t)(buf[17] << 8) | buf[16];
    params->dig_p7 = (int16_t)(buf[19] << 8) | buf[18];
    params->dig_p8 = (int16_t)(buf[21] << 8) | buf[20];
    params->dig_p9 = (int16_t)(buf[23] << 8) | buf[22];
}

#endif

void print_ascii(uint16_t value){
    char high_byte = (value >> 8) & 0xFF; // Extract high byte
    char low_byte = value & 0xFF;        // Extract low byte

    printf("%c%c\n", high_byte, low_byte);
}

void print_binary_16(uint16_t value) {
    printf("Binary representation: ");
    for (int i = 15; i >= 0; i--) { // Loop through each bit (16 bits for uint16_t)
        printf("%d", (value >> i) & 1); // Extract the i-th bit and print it
        if (i % 4 == 0 && i != 0) {    // Add a space every 4 bits for readability
            printf(" ");
        }
    }
    printf("\n");
}
void print_binary_8(uint8_t value) {
    printf("Binary representation: ");
    for (int i = 7; i >= 0; i--) { // Loop through each bit (16 bits for uint16_t)
        printf("%d", (value >> i) & 1); // Extract the i-th bit and print it
        if (i % 4 == 0 && i != 0) {    // Add a space every 4 bits for readability
            printf(" ");
        }
    }
    printf("\n");
}

/*

void read_bus_v(ina3221_t *dev){

    uint16_t ch1_bus, ch1_shunt;
    uint8_t reg = 0x02;

    uint16_t out;
    int status = 0;
    status = i2c_write_blocking(dev->params.i2c, dev->params.addr, &reg, 1, true);
 //   status = i2c_read_blocking(dev->params.i2c, dev->params.addr, &tmp, 1, true);
    status = i2c_read_blocking(dev->params.i2c, dev->params.addr, (uint8_t *)(&out), 2, false);
    ch1_bus = (out >> 8) | (out << 8);

    
    reg = 0x01;
    status = i2c_write_blocking(dev->params.i2c, dev->params.addr, &reg, 1, true);
 //   status = i2c_read_blocking(dev->params.i2c, dev->params.addr, &tmp, 1, true);
    status = i2c_read_blocking(dev->params.i2c, dev->params.addr, (uint8_t *)(&out), 2, false);
    
    ch1_shunt = (out >> 8) | (out << 8);

    printf("CH1 shunt %duV\tbus: %dmV\n", ch1_shunt, ch1_bus);

    ina3221_reset(dev);
}



void test_ina(ina3221_t *ina3221) {
    uint16_t cfg;
    printf("get_config status: %d\ncfg: ", ina3221_get_config(ina3221, &cfg));
    print_binary_16(cfg);

    ina3221_set_config(ina3221, 319);

    ina3221_set_enable_channel(ina3221, INA3221_ENABLE_CH1);

    printf("get_config status: %d\n\tcfgafter: ", ina3221_get_config(ina3221, &cfg));
    print_binary_16(cfg);

    ina3221_set_power_valid_lower_limit(ina3221, 9000);
    ina3221_set_power_valid_upper_limit(ina3221, 14000);
    int16_t bus_voltages[3];
    uint16_t flags;
    ina3221_read_bus_mv(ina3221, bus_voltages, &flags);

    printf("BUS V: %d, %d, %d\n", bus_voltages[0], bus_voltages[1], bus_voltages[2]);
    print_binary_16(cfg);

    bool warn, crit;
    ina3221_get_latch(ina3221, &warn, &crit);
    printf("latches: warn: %d,  crit: %d\n", warn, crit);


    read_bus_v(ina3221);

}
*/

using namespace std;

void ina_test(INA3221 *ina){
    
    uint16_t config = ina->getReg(INA3221_REG_CONF);
    print_binary_16(config);

    //ina->setChannelDisable(INA3221_CH3);
    printf("after ch3 disable\n");
    
    config = ina->getReg(INA3221_REG_CONF);
    print_binary_16(config);
}


#define INA_I2C_SDA_PIN 6
#define INA_I2C_SCL_PIN 7

int main() {
    stdio_init_all();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c / bmp280_i2c example requires a board with I2C pins
        puts("Default I2C pins were not defined");
    return 0;
#else
    // useful information for picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_program_description("BMP280 I2C example for the Raspberry Pi Pico"));

    printf("Hello, BMP280! Reading temperaure and pressure values from sensor...\n");

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);


    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
   // scan_i2c_bus(i2c_default, 5000);
    // configure BMP280
    //bmp280_init();

    //init i2c1 for INA
    i2c_inst_t *i2c2 = I2C_INSTANCE(1);
    i2c_init(i2c2, 100 * 1000);
    gpio_set_function(INA_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(INA_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(INA_I2C_SDA_PIN);
    gpio_pull_up(INA_I2C_SCL_PIN);


    scan_i2c_bus(i2c2, 1000);

    INA3221 ina = INA3221(INA3221_ADDR40_GND);
    ina.begin(i2c2);

    ina.setShuntRes(100, 3, 10);
    ina.setFilterRes(10, 10, 10);


    printf("MANUF ID: ");
    print_ascii(ina.getManufID());

    ina_test(&ina);
    

    // retrieve fixed compensation params
    struct bmp280_calib_param params;
    bmp280_get_calib_params(&params);

    int32_t raw_temperature;
    int32_t raw_pressure;
    int32_t bus_voltages[3];
    int32_t shunt_uv[3];
    uint16_t flags;

    sleep_ms(500); // sleep so that data polling and register update don't collide
    while (1) {


        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        printf("BUS: %.2fV, %.2fV, %.2fV \n", ina.getVoltage(INA3221_CH1), ina.getVoltage(INA3221_CH2), ina.getVoltage(INA3221_CH3));

        printf("SHUNT: %duV, %duV, %duV \n", ina.getShuntVoltage(INA3221_CH1), ina.getShuntVoltage(INA3221_CH2), ina.getShuntVoltage(INA3221_CH3));


        printf("Current: %.2f, %.2f, %.2f\n", ina.getCurrentCompensated(INA3221_CH1), ina.getCurrentCompensated(INA3221_CH2), ina.getCurrentCompensated(INA3221_CH3));
        sleep_ms(1000);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);
        printf("..................\n");
    //    std::cout << "ELO MORDO From C++" << std::endl;
    }
#endif
}
