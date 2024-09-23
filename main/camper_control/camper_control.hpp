#include <iostream>
#include <map>
#include <string>
#include <memory>

#include "esp_log.h"
#include "gpio_cxx.hpp"
#include "esp_io_expander.h"


using namespace std;
using namespace idf;

class CamperControl{
    public:
    CamperControl();

    private:
    map<string, uint32_t> prameters;
};


class ExtendedGPIO_Output{
    public: 
    ExtendedGPIO_Output(uint32_t io_num, string name);
    ExtendedGPIO_Output(esp_io_expander_handle_t *expander_handle, esp_io_expander_pin_num_t expander_pin_num, string name);
    void set_high();
    void set_low();

    private:
    uint32_t num;
    string name;

    GPIO_Output *standard_gpio;
    esp_io_expander_handle_t *expander_handle;
};

class WaterSensor{
    public:
    WaterSensor();
    float get_current_level();

    private:
    uint32_t last_reading;
};

class TemperatureSensor{
    public:
    TemperatureSensor();
    float get_current_reading();
    
    private:
};


