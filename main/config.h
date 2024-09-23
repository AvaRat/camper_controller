#ifndef CONFIG_INCLUDE
#define CONFIG_INCLUDE

#define BTN1_PIN 23
#define BTN2_PIN 22

#define I2C_MASTER_SCL_IO           25      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           26      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */

#define ADC_UNIT ADC_UNIT_2
#define ADC_CHANNEL ADC_CHANNEL_3
#define MUX_S0 1 
#define MUX_S1 2
#define MUX_S2 3
#define MUX_S3 4


#endif