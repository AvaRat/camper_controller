#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include <gpio_cxx.hpp>

#include "esp_log.h"
#include "ds18b20.h"
#include "onewire_bus.h"

static const char *TAG_h = "temp_sensor";

using namespace std;
using namespace idf;


class TempSensorDriver{

    public:
    TempSensorDriver(gpio_num_t one_wire_gpio)
    :   bus_config({.bus_gpio_num = int(one_wire_gpio),}), 
        rmt_config({.max_rx_bytes = 10,})
    {
        ESP_ERROR_CHECK(onewire_new_bus_rmt(&bus_config, &rmt_config, &bus));
        
        int ds18b20_device_num = 0;
        onewire_device_iter_handle_t iter = NULL;
        onewire_device_t next_onewire_device;
        esp_err_t search_result = ESP_OK;

        // create 1-wire device iterator, which is used for device search
        ESP_ERROR_CHECK(onewire_new_device_iter(bus, &iter));
        ESP_LOGI(TAG_h, "Device iterator created, start searching...");
        do {
            search_result = onewire_device_iter_get_next(iter, &next_onewire_device);
            if (search_result == ESP_OK) { // found a new device, let's check if we can upgrade it to a DS18B20
                ds18b20_device_handle_t handle;
                ds18b20_config_t ds_cfg = {};
                // check if the device is a DS18B20, if so, return the ds18b20 handle
                if (ds18b20_new_device(&next_onewire_device, &ds_cfg, &handle) == ESP_OK) {
                    ESP_LOGI(TAG_h, "Found a DS18B20[%d], address: %016llX", ds18b20_device_num, next_onewire_device.address);
                    sensors.push_back(handle);
                } else {
                    ESP_LOGI(TAG_h, "Found an unknown device, address: %016llX", next_onewire_device.address);
                }
            }
        } while (search_result != ESP_ERR_NOT_FOUND);
        ESP_ERROR_CHECK(onewire_del_device_iter(iter));
    }
    void  read(){
        vector<float> output_temps(sensors.size());
        float temperature;
        for(ds18b20_device_handle_t sensor : sensors){
            ESP_ERROR_CHECK(ds18b20_trigger_temperature_conversion(sensor));
            ESP_ERROR_CHECK(ds18b20_get_temperature(sensor, &temperature));
            ESP_LOGI(TAG_h, "temperature read from DS18B20: %.2fC", temperature);
            output_temps.push_back(temperature);
        }
    };

    private:
    onewire_bus_handle_t bus = NULL;
    onewire_bus_config_t bus_config;
    onewire_bus_rmt_config_t rmt_config;
    vector<ds18b20_device_handle_t> sensors;
};

