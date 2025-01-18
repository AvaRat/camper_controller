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


#include <ws.hpp>

#include "hss-esp.hpp"

static const char *TAG = "main";

using namespace std;
using namespace std::chrono;
using namespace idf;

using namespace hss;

const auto test_sleep_time = seconds{2};


//#include <driver/i2c_master.h>

#define I2C_MASTER_SCL_IO           SCL_GPIO(22)      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           SDA_GPIO(21)      /*!< GPIO number used for I2C master data  */

// void i2c_init()
// {
// i2c_master_bus_config_t i2c_mst_config = {
//     .clk_source = I2C_CLK_SRC_DEFAULT,
//     .i2c_port = TEST_I2C_PORT,
//     .scl_io_num = I2C_MASTER_SCL_IO,
//     .sda_io_num = I2C_MASTER_SDA_IO,
//     .glitch_ignore_cnt = 7,
//     .flags.enable_internal_pullup = true,
// };

// i2c_master_bus_handle_t bus_handle;
// ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

// i2c_device_config_t dev_cfg = {
//     .dev_addr_length = I2C_ADDR_BIT_LEN_7,
//     .device_address = 0x58,
//     .scl_speed_hz = 100000,
// };

// i2c_master_dev_handle_t dev_handle;
// ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
// }

void hss_test_thread(){

    shared_ptr<I2CMaster> master(new I2CMaster(I2CNumber::I2C0(),
                I2C_MASTER_SCL_IO,
                I2C_MASTER_SDA_IO,
                Frequency(400000)));
    esp_io_expander_handle_t io_expander = NULL;
    
    ESP_ERROR_CHECK(esp_io_expander_new_i2c_tca95xx_16bit(I2C_NUM_0, ESP_IO_EXPANDER_I2C_TCA9555_ADDRESS_010, &io_expander));
    ESP_ERROR_CHECK(esp_io_expander_set_dir(io_expander, IO_EXPANDER_PIN_NUM_15, IO_EXPANDER_OUTPUT));
    ESP_ERROR_CHECK(esp_io_expander_set_level(io_expander, IO_EXPANDER_PIN_NUM_15 , 1));

    bool state = 0;

    while(1)
    {
        ESP_LOGI(TAG, "toggling io-espander pin 17 ");
        esp_io_expander_set_level(io_expander, IO_EXPANDER_PIN_NUM_15 , state);
        state = !state;
        this_thread::sleep_for(test_sleep_time);


    }
}

void test_func()
{
            // creating master bus
    shared_ptr<I2CMaster> master(new I2CMaster(I2CNumber::I2C0(),
                I2C_MASTER_SCL_IO,
                I2C_MASTER_SDA_IO,
                Frequency(400000)));
    ESP_LOGI(TAG, "I2C initialized successfully");
    esp_io_expander_handle_t io_expander = NULL;
    esp_io_expander_new_i2c_tca95xx_16bit(I2C_NUM_0, ESP_IO_EXPANDER_I2C_TCA9539_ADDRESS_00, &io_expander);
    esp_io_expander_set_dir(io_expander, IO_EXPANDER_PIN_NUM_17, IO_EXPANDER_OUTPUT);
    esp_io_expander_set_level(io_expander, IO_EXPANDER_PIN_NUM_17 , 1);
    HssEsp hss1(0, 1, 2, &hss::BTS7006);
    hss1.enable();

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
        print_thread_info("This is the INHERITING thread with the same parameters as our parent, including name. ");
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
        print_thread_info("This thread (with the default name) may run on any core.");
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


    // Create a thread using default values that can run on any core
    auto cfg = esp_pthread_get_default_config();
    esp_pthread_set_cfg(&cfg);
    std::thread any_core(thread_func_any_core);

    // Create a thread on core 0 that spawns another thread, they will both have the same name etc.
    cfg = create_config("Thread 1", 0, 3 * 1024, 5);
    cfg.inherit_cfg = true;
    esp_pthread_set_cfg(&cfg);
    std::thread thread_1(spawn_another_thread);

    // Create a thread on core 1.
    cfg = create_config("HSS Thread 2", 1, 3 * 2048, 5);
    esp_pthread_set_cfg(&cfg);

    //test_func();
    std::thread thread_2(hss_test_thread);

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
