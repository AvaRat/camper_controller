
#include <cstdlib>
#include <thread>
#include "esp_log.h"
#include "gpio_cxx.hpp"
#include "esp_log.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"

#include "power_switch/BTS_power_switch.hpp"
#include "control_panel/control_panel.hpp"

#include "gpio_extended.h"
#include "config.h"


static int adc_raw[2][10];



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

float get_avg_adc(adc_oneshot_unit_handle_t adc1_handle){
    int raw_adc;
    int sum = 0;
    for(int i=0; i<100; i++){
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL, &raw_adc));
        sum += raw_adc;
        sleep(0.01);
    }

    float result = sum / 100;
    return result;
};

extern "C" {
    void app_main(void)
    {
        BTS_Driver ch1_drv = BTS_Driver(14, 27);

        ControlPanel control_panel(ch1_drv);
        

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1;
    init_config1.unit_id = ADC_UNIT;
    init_config1.clk_src = ADC_RTC_CLK_SRC_DEFAULT; // use default clock
    
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    i2c_master_init();
    ExpanderTCA tca(I2C_NUM_0, TCA_I2C_ADDRESS);
    tca.configure_output_GPIOs({{5, 11}, {4,12}, {3, 13}, {2, 14}, {1, 15}});
    tca.set_low(1);
    tca.set_low(2);
    tca.set_low(3);
    tca.set_low(4);
    tca.set_low(5);
   // ExtendedOutputGPIOSpace gpio_space({22, 23, 50, 51, 52, 53}, &tca);
   // gpio_space.set_low({50, 51, 52, 53});

   // AnalogMultiplexer mux(&gpio_space, 50, 51, 52, 53);


    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;
    config.atten = ADC_ATTEN_DB_12;
    

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL, &config));
    int channel_num = 0;
    float awg_adc = 0;

        while(1){
          //  mux.enableChannel(channel_num++);
            if(channel_num == 5)
                channel_num = 0;
            sleep(0.1);
            awg_adc = get_avg_adc(adc1_handle);
            cout << "ADC AVG: " << awg_adc << endl;
            sleep(2);
        }
    }
}