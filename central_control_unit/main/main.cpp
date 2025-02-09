/* pthread/std::thread example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_pthread.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include <esp_io_expander.h>
#include "esp_io_expander_tca95xx_16bit.h"
#include "esp_log.h"
#include <gpio_cxx.hpp>
#include "i2c_cxx.hpp"
#include "cd74hc_mux.hpp"

#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_timer_cxx.hpp"
#include <bits/stdc++.h>
#include "esp_timer_cxx.hpp"

#include "analog_ui.h"
#include "temp_sensor.h"
#include <ws.hpp>

//#include "hss-esp.hpp"
#include "hss-esp-expander.hpp"

static const char *TAG = "main";

using namespace std;
using namespace std::chrono;
using namespace idf;
using namespace idf::esp_timer;

using namespace hss;

const auto test_sleep_time = seconds{3};
const auto ms50 = milliseconds(50);
const auto sec2 = milliseconds(2000);


//#include <driver/i2c_master.h>

#define I2C_MASTER_SCL_IO           SCL_GPIO(22)      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           SDA_GPIO(21)      /*!< GPIO number used for I2C master data  */




void adc_init(adc_oneshot_unit_handle_t *handle, adc_channel_t adc_channel)
{
    // ADC2 CH2

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*handle, adc_channel, &config));
}

void test_mux()
{
    shared_ptr<I2CMaster> master(new I2CMaster(I2CNumber::I2C0(),
                I2C_MASTER_SCL_IO,
                I2C_MASTER_SDA_IO,
                Frequency(400000)));
    ESP_LOGI(TAG, "I2C initialized successfully");
    esp_io_expander_handle_t io_expander = NULL;
    esp_err_t status;
    
    status = esp_io_expander_new_i2c_tca95xx_16bit(I2C_NUM_0, ESP_IO_EXPANDER_I2C_TCA9555_ADDRESS_010, &io_expander);
    //esp_io_expander_reset(io_expander);
    if(status != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize tca expander");
        return;
    }

    Cd74HCMux mux( io_expander, IO_EXPANDER_PIN_NUM_0, IO_EXPANDER_PIN_NUM_1, IO_EXPANDER_PIN_NUM_2, IO_EXPANDER_PIN_NUM_3);   

    while(1)
    {
        for (uint8_t i = 0; i < 16; ++i) 
        {
            mux.enable_channel(i);      
            this_thread::sleep_for(sec2); 
        }
    }
}

void hss_test_thread(){

    shared_ptr<I2CMaster> master(new I2CMaster(I2CNumber::I2C0(),
                I2C_MASTER_SCL_IO,
                I2C_MASTER_SDA_IO,
                Frequency(400000)));
    esp_io_expander_handle_t io_expander = NULL;
    esp_err_t status;
    
    status = esp_io_expander_new_i2c_tca95xx_16bit(I2C_NUM_0, ESP_IO_EXPANDER_I2C_TCA9555_ADDRESS_010, &io_expander);
    //esp_io_expander_reset(io_expander);
    if(status != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize tca expander");
        return;
    }
     
    adc_oneshot_unit_handle_t adc_handle;
    adc_channel_t adc_channel = ADC_CHANNEL_2;
    adc_init(&adc_handle, adc_channel);

    Cd74HCMux mux( io_expander, IO_EXPANDER_PIN_NUM_0, IO_EXPANDER_PIN_NUM_1, IO_EXPANDER_PIN_NUM_2, IO_EXPANDER_PIN_NUM_3);   

    //TODO put this into #DEFINEs
    uint8_t ch1_diag_mux_pin = 8;
    uint8_t ch5_diag_mux_pin = 12;

    HssEspExpander hss1(io_expander, IO_EXPANDER_PIN_NUM_10, IO_EXPANDER_PIN_NUM_15, 
        &mux, ch1_diag_mux_pin, adc_handle, adc_channel, 
        &hss::BTS7006);
    HssEspExpander hss5(io_expander, IO_EXPANDER_PIN_NUM_6, IO_EXPANDER_PIN_NUM_11, 
        &mux, ch5_diag_mux_pin, adc_handle, adc_channel,
        &hss::BTS7006);


    hss1.init();
    hss5.init();
    hss1.enable();
    hss5.enable();

    hss1.enableDiag();
    hss5.enableDiag();

    hss1.disable();
    this_thread::sleep_for(chrono::milliseconds(500));  
    hss1.enable();

    int adc_raw = 0;
    int voltage = 0;
    bool state = 0;
    uint16_t r_sense = 1410;

    while(1)
    {
        float sense_current = hss5.readIs(r_sense);
        int diag_status = hss5.diagRead(sense_current);
        ESP_LOGI(TAG, "sensed current: %f", sense_current);
        ESP_LOGI(TAG, "HSS5 diag msg: %d", diag_status);

        hss5.print_diag(diag_status);
        this_thread::sleep_for(test_sleep_time);  


        sense_current = hss1.readIs(r_sense);
        diag_status = hss1.diagRead(sense_current);
        ESP_LOGI(TAG, "sensed current: %f", sense_current);
        ESP_LOGI(TAG, "HSS1 diag msg: %d", diag_status);

        hss1.print_diag(diag_status);
        this_thread::sleep_for(test_sleep_time);  

        // hss5.disable();
        // this_thread::sleep_for(chrono::milliseconds(500));  
        // hss5.enable();

    }
}


const auto sleep_time = seconds
{
    10
};



void print_thread_info(const char *extra = nullptr)
{
    std::stringstream ss;
    if (extra) {
        ss << extra;
    }
    ss << "Core id: " << xPortGetCoreID()
       << ", prio: " << uxTaskPriorityGet(nullptr)
       << ", minimum free stack: " << uxTaskGetStackHighWaterMark(nullptr) << " bytes.";
    ESP_LOGI(pcTaskGetName(nullptr), "%s", ss.str().c_str());
}

void thread_func_inherited()
{
    while (true) {
        //print_thread_info("This is the INHERITING thread with the same parameters as our parent, including name. ");
        std::this_thread::sleep_for(sleep_time);
    }
}

void spawn_another_thread()
{
    // Create a new thread, it will inherit our configuration
    std::thread inherits(thread_func_inherited);

    while (true) {
        print_thread_info();
        std::this_thread::sleep_for(sleep_time);
    }
}

void thread_func_any_core()
{
    while (true) {
        //print_thread_info("This thread (with the default name) may run on any core.");
        std::this_thread::sleep_for(sleep_time);
    }
}

esp_pthread_cfg_t create_config(const char *name, int core_id, int stack, int prio)
{
    auto cfg = esp_pthread_get_default_config();
    cfg.thread_name = name;
    cfg.pin_to_core = core_id;
    cfg.stack_size = stack;
    cfg.prio = prio;
    return cfg;
}

extern "C" void app_main(void)
{
    // this_thread::sleep_for(std::chrono::seconds(4));
    // TempSensorDriver temp_sensors(GPIO_NUM_0);

    // ESP_LOGI(TAG, "Setting up timer to read temperature update every 1000 ms\n");
    // function<void()> temp_update = [&]() { temp_sensors.read();};
    // ESPTimer timer(temp_update);
    // timer.start_periodic(chrono::milliseconds(5000));


    // Create a thread on core 0 that spawns another thread, they will both have the same name etc.
    auto cfg = create_config("Thread 1", 0, 3 * 1024, 5);
    cfg.inherit_cfg = true;
    esp_pthread_set_cfg(&cfg);
    std::thread thread_1(spawn_another_thread);


  // Analog UI
  ButtonConfig_t btn_heater = {GPIO_NUM_5, "heater"};
  ButtonConfig_t btn_mains = {GPIO_NUM_18, "mains"};
  ButtonConfig_t btn_water = {GPIO_NUM_19, "water"};
  AnalogUiButtonsConfig_t buttons_config = {btn_heater, btn_mains, btn_water};

  AnalogUiConfig_t analog_ui_config = {
      .update_frequency = 20, 
      .led_strip_gpio=GPIO_NUM_2,
      .buttons_config = buttons_config,
      .water_lvl_leds = {0,1,2,3},
      .normal_leds = vector<uint16_t>(4)
  };
  iota(analog_ui_config.normal_leds.begin(), analog_ui_config.normal_leds.end(), 4);
  /*
  TEST BEGIN
  */
  AnalogUI ui(analog_ui_config);

  ui.init_hardware();

  // Create a thread on core 1.
  // auto cfg = create_config("Led Strip Thread", 1, 3 * 1024, 5);
  // esp_pthread_set_cfg(&cfg);
  // std::thread thread_2(ui.loop);
  //ui.loop();

  ESP_LOGI(TAG, "Setting up timer to trigger UI update every 10ms\n");
  function<void()> ui_update = [&]() { ui.update();};
  idf::esp_timer::ESPTimer ui_timer(ui_update);
  ui_timer.start_periodic(chrono::milliseconds(10));

  //ui.test_fun();   
  for(auto led : ui.led_driver.leds){
    ESP_LOGI(TAG, "LED setup");
    led->set_brightness(10);
    led->set_color(Color::red());
    led->flash(2000);
    this_thread::sleep_for(std::chrono::milliseconds(100));
  }  

    // Let the main task do something too
    while (true) {
        std::stringstream ss;
        ss << "core id: " << xPortGetCoreID()
           << ", prio: " << uxTaskPriorityGet(nullptr)
           << ", minimum free stack: " << uxTaskGetStackHighWaterMark(nullptr) << " bytes.";
        ESP_LOGI(pcTaskGetName(nullptr), "%s", ss.str().c_str());
        std::this_thread::sleep_for(sleep_time);
    }
}
