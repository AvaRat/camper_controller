
#include <cstdlib>
#include <thread>
#include "esp_log.h"
#include "gpio_cxx.hpp"
#include "esp_log.h"
#include "soc/soc_caps.h"

#include "power_switch/BTS_power_switch.hpp"
#include "control_panel/control_panel.hpp"

#include "camper_control/camper_control.hpp"
#include "gpio/gpio_extended.hpp"
#include "gpio/multiplexer.hpp"
#include "config.h"


using namespace idf;
using namespace std;

const static char *TAG = "main.cpp";


//A0: GND   A1: 3V3  A2: GND
// 0100010  0b0100010
#define TCA_I2C_ADDRESS  ESP_IO_EXPANDER_I2C_TCA9555_ADDRESS_010        /*!< Slave address of the MPU9250 sensor */


#ifdef __cplusplus
extern "C" {
#endif

static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {I2C_MODE_MASTER, 
        I2C_MASTER_SDA_IO,
        I2C_MASTER_SCL_IO,
        GPIO_PULLUP_ENABLE,
        true, //sda pullup enable
        true, // scl pullup enable
        };
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    i2c_port_t i2c_port = I2C_NUM_0;

    i2c_param_config(i2c_port, &conf);

    return i2c_driver_install(i2c_port, conf.mode, 0, 0, 0);
}
#ifdef __cplusplus
}
#endif



extern "C" {
    void app_main(void)
    {
        i2c_master_init();
        esp_io_expander_handle_t io_expander = NULL;
        ESP_ERROR_CHECK(esp_io_expander_new_i2c_tca95xx_16bit(I2C_NUM_0, TCA_I2C_ADDRESS, &io_expander));

        ExtendedGPIO_Output s0(&io_expander, IO_EXPANDER_PIN_NUM_0, "s0");
        ExtendedGPIO_Output s1(&io_expander, IO_EXPANDER_PIN_NUM_1, "s1");
        ExtendedGPIO_Output s2(&io_expander, IO_EXPANDER_PIN_NUM_2, "s2");
        ExtendedGPIO_Output s3(&io_expander, IO_EXPANDER_PIN_NUM_3, "s3");

        ExtendedGPIO_Output ch1(&io_expander, IO_EXPANDER_PIN_NUM_11, "s3");

        AnalogMultiplexer mux(ADC_UNIT_2, ADC_CHANNEL_3, &s0, &s1, &s2, &s3);
        WaterSensor water_sensor_1(&mux, {0,1,2});

        while(1){
            cout << "water lvl: " << water_sensor_1.get_current_level() <<"%" << endl;
           ch1.set_high();
            sleep(5);
            ch1.set_low();
            sleep(1);
            cout << "next" << endl;
        }
    }
}