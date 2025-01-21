/**
 * @file        hss-pal-adc-Esp.cpp
 * @brief       ADC platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

//#include "esp_adc/adc_oneshot.h"
#include "hss-pal-adc-esp.hpp"

using namespace hss;

/**
 * @brief Default constructor of the ADC class for Esp
 */
ADCEsp::ADCEsp(esp_io_expander_handle_t handle, mux_config_t conf): pin(0)
{
    conf.s0 = 90;
    uint8_t c = handle->config.io_count;
    mux_config_t co = conf;
    co.s0 = c;
    // adc_oneshot_unit_handle_t adc1_handle;
    // adc_oneshot_unit_init_cfg_t init_config1;
    // init_config1.unit_id = ADC_UNIT_1;
    // init_config1.clk_src = ADC_RTC_CLK_SRC_DEFAULT; // use default clock
    
    // ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    // //-------------ADC1 Config---------------//
    // adc_oneshot_chan_cfg_t config;
    // config.bitwidth = ADC_BITWIDTH_DEFAULT;
    // config.atten = ADC_ATTEN_DB_12;
}

/**
 * @brief    Constructor of the ADC class for arduEsp
 * @details  This constructor takes the number of the pin.
 * @param[in]   pin Number of the desired ADC pin
 */
ADCEsp::ADCEsp(uint8_t pin) : pin(pin)
{

}

/**
 * @brief Default constructor of the ArduEsp ADC
 *
 */
ADCEsp::~ADCEsp()
{

}

/**
 * @brief  ADC initialization
 * @return High-side switch error code
 */
Error_t ADCEsp::init()
{
    return OK;
}

/**
 * @brief ADC deinitialization
 * @return High-side switch error code
 */
Error_t ADCEsp::deinit()
{
    return OK;
}

/**
 * @brief ADC enable
 * @return High-side switch error code
 */
Error_t ADCEsp::enable()
{
    return OK;
}


/**
 * @brief ADC disable
 * @return High-side switch error code
 */
Error_t ADCEsp::disable()
{
    return OK;
}

/**
 * @brief Read the current ADC-Value
 * @return uint16_t ADC value
 */
uint16_t ADCEsp::ADCRead()
{
    uint16_t result = 0;

   // result = analogRead(pin);

    return result;
}

/**
 * @brief   Write on an analog pin
 * @details This functions writes the given value to the analog pin
 * @param[in] value Value to write
 * @return  High-side switch error code
 */
Error_t ADCEsp::ADCWrite(uint8_t value)
{
    //analogWrite(pin, value);

    return OK;
}

