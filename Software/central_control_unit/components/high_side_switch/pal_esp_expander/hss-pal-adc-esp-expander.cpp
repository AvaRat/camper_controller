/**
 * @file        hss-pal-adc-Esp.cpp
 * @brief       ADC platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

//#include "esp_adc/adc_oneshot.h"
#include "hss-pal-adc-esp-expander.hpp"


static const char *TAG = "hss-pal-adc-esp-expander";


using namespace std;
using namespace hss;



/**
 * @brief    Constructor of the ADC class for ADCEspExpander
 * @details  ADC should be already initialised before!!
 * @param[in]   _expander_handle 
 * @param[in]   _mux_config Config to enable proper ADC signal via multiplexer
 */
ADCEspExpander::ADCEspExpander(Cd74HCMux *_mux, uint8_t _mux_channel, adc_oneshot_unit_handle_t _adc_handle, adc_channel_t _adc_channel)
    : mux(_mux), mux_channel(_mux_channel), adc_handle(_adc_handle), adc_channel(_adc_channel)
{
    //TODO Check proper MUX initialization and each channel state if set as OUTPUT
    ESP_LOGI(TAG, "init ADC_ESP on mux channel %d", mux_channel);
}



/**
 * @brief Default constructor of the ArduEsp ADC
 *
 */
ADCEspExpander::~ADCEspExpander()
{

}

/**
 * @brief  ADC initialization
 * @return High-side switch error code
 */
Error_t ADCEspExpander::init()
{
    return OK;
}

/**
 * @brief ADC deinitialization
 * @return High-side switch error code
 */
Error_t ADCEspExpander::deinit()
{
    return OK;
}

/**
 * @brief ADC enable
 * @return High-side switch error code
 */
Error_t ADCEspExpander::enable()
{
    return OK;
}


/**
 * @brief ADC disable
 * @return High-side switch error code
 */
Error_t ADCEspExpander::disable()
{
    return OK;
}

/**
 * @brief Read the current ADC-Value
 * @return uint16_t ADC value
 */
uint16_t ADCEspExpander::ADCRead()
{
    int raw_adc_read;
    mux->enable_channel(mux_channel);
    this_thread::sleep_for(chrono::milliseconds(10));
    //Read ADC and return
    esp_err_t er = adc_oneshot_read(adc_handle, adc_channel, &raw_adc_read);
    if(er != ESP_OK)
    {
        ESP_LOGE(TAG, "adc read failed");
    }else{
            ESP_LOGI(TAG, "ADC READ succesfull value: %d", raw_adc_read);
    }
    return (uint16_t) raw_adc_read;
}


