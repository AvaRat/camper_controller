
#include <cstdlib>
#include <thread>
#include "esp_log.h"
#include "gpio_cxx.hpp"
#include "esp_log.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"

#include "power_switch/BTS_power_switch.hpp"
#include "control_panel/control_panel.hpp"


static int adc_raw[2][10];
static int voltage[2][10];



using namespace idf;
using namespace std;


const static char *TAG = "main.cpp";

extern "C" {
    void app_main(void)
    {
        BTS_Driver ch1_drv = BTS_Driver(2, 11, 10);

        ControlPanel control_panel(ch1_drv);
        

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1;
    init_config1.unit_id = ADC_UNIT_1;
    init_config1.clk_src = ADC_RTC_CLK_SRC_DEFAULT; // use default clock
    
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;
    config.atten = ADC_ATTEN_DB_12;
    

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_1, &config));

        while(1){
            ch1_drv.enable();
            sleep(1);
            ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_1, &adc_raw[0][0]));
            ESP_LOGI(TAG, "SHURT CIRCUIT: ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC_CHANNEL_1, adc_raw[0][0]);
            sleep(5);
            ch1_drv.disable();
            sleep(1);
            ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_1, &adc_raw[0][0]));
            ESP_LOGI(TAG, "CH1 OF: ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC_CHANNEL_1, adc_raw[0][0]);
        }
    }
}