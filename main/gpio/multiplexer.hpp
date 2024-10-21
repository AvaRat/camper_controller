#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP


#include <iostream>
#include "esp_adc/adc_oneshot.h"

#include "gpio_extended.hpp"

using namespace idf;
using namespace std;



#define MULTIPLEXER_CHANNELS 16

class AnalogMultiplexer{
/*
Analog channel based on CD74 multiplexer with physical chip's analog channel connected to MUX common.
*/
    public:
    AnalogMultiplexer(adc_unit_t _adc_unit, adc_channel_t _adc_channel, ExtendedGPIO_Output *s0_pin, ExtendedGPIO_Output *s1_pin, ExtendedGPIO_Output *s2_pin, ExtendedGPIO_Output *s3_pin)
        :adc_1shot_channel(_adc_channel), S0_pin(s0_pin), S1_pin(s1_pin), S2_pin(s2_pin), S3_pin(s3_pin)
    {
        
        adc_oneshot_unit_init_cfg_t init_config1;
        init_config1.unit_id = _adc_unit;  
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc_1shot_handle));

        adc_oneshot_chan_cfg_t config;
        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        config.atten = ADC_ATTEN_DB_12;
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_1shot_handle, adc_1shot_channel, &config)); 
    };

    uint8_t enableChannel(uint32_t channel){
        if(channel >= MULTIPLEXER_CHANNELS){
            return -1; //throw some exception or use enum to check for correctness during compilation
        }
        const bitset<4> bits(channel);
        //        cout << "channel " << channel << " enabled" << endl << "setting bits: "<< bits << endl;
        S3_pin->set_output(bits[3]);
        S2_pin->set_output(bits[2]);
        S1_pin->set_output(bits[1]);
        S0_pin->set_output(bits[0]);
        return 0;
    };

    float enable_and_analog_read(uint32_t channel){
        this->enableChannel(channel);
        sleep(0.01);
        return this->get_avg_adc_value();
    }

    private:
    adc_oneshot_unit_handle_t adc_1shot_handle;
    adc_channel_t adc_1shot_channel;
    ExtendedGPIO_Output *S0_pin, *S1_pin, *S2_pin, *S3_pin;


    float get_avg_adc_value(){
        int raw_adc;
        int sum = 0;
        for(int i=0; i<10; i++){
            ESP_ERROR_CHECK(adc_oneshot_read(adc_1shot_handle, adc_1shot_channel, &raw_adc));
            sum += raw_adc;
            sleep(0.01);
        }

        float result = sum / 10;
        return result;
    };

};

#endif