#include "camper_control.hpp"


using namespace std;
using namespace idf;


WaterSensor::WaterSensor(AnalogMultiplexer *_mux, vector<uint32_t> _channels)
    : analog_channels(_channels), mux(_mux) {
        resolution = _channels.size();
    }

float WaterSensor::get_current_level(){
/* simple mechanism to count percentage of pins under water.
    1 pin in water -> lowest water lvl
    2 pins in water -> seccond lovest lvl
    all pins in water -> water tank is "full"
*/
    uint32_t pins_under_water = 0;
    for(uint32_t channel: analog_channels)
    {
      //  cout << "channel " << channel << " enable" << endl;
        float raw_adc = mux->enable_and_analog_read(channel);
        if(raw_adc >= adc_threshold){
            pins_under_water++;
        }
    }
    this->last_reading = (uint32_t) 100 * ((float)pins_under_water / (float)resolution); 
    return this->last_reading;
}

