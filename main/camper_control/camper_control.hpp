#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "esp_log.h"
#include "gpio_cxx.hpp"

#include "gpio/multiplexer.hpp"


using namespace std;
using namespace idf;

class CamperControl{
    public:
    CamperControl();

    private:
    map<string, uint32_t> prameters;
};

class WaterSensor{
    public:
    WaterSensor(AnalogMultiplexer *_mux, vector<uint32_t> _channels);
    float get_current_level();

    private:
    uint32_t last_reading = 0; //percent %
    uint32_t adc_threshold = 200; // raw adc read 
    uint32_t resolution; // 3 waters levels are available (FULL, )
    vector<uint32_t> analog_channels;
    AnalogMultiplexer *mux;

};

class TemperatureSensor{
    public:
    TemperatureSensor();
    float get_current_reading();
    
    private:
};


