/**
 * @file        hss-pal-adc-Esp.hpp
 * @brief       ADC platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_PAL_ADC_Esp_Expander_HPP
#define HSS_PAL_ADC_Esp_Expander_HPP

#include <thread>
#include <chrono>
#include <esp_log.h>

#include "hss-pal-adc.hpp"
#include "cd74hc_mux.hpp"
#include <esp_io_expander.h>
#include "esp_adc/adc_oneshot.h"

namespace hss
{

typedef struct
{
    uint8_t s0;
    uint8_t s1;
    uint8_t s2;
    uint8_t s3;
}mux_config_t;

/**
 * @addtogroup EspPal
 * @{
 */

/**
 * @class   ADCEsp
 * @brief   ADC platform abstraction layer ArduEsp API
 */
class ADCEspExpander : virtual public ADCPAL
{
    private:
        Cd74HCMux *mux;
        uint8_t mux_channel;
        adc_oneshot_unit_handle_t adc_handle;
        adc_channel_t adc_channel;


    public:

        ADCEspExpander(Cd74HCMux *_mux, uint8_t _mux_channel, adc_oneshot_unit_handle_t _adc_handle, adc_channel_t _adc_channel);
        ~ADCEspExpander();
        Error_t     init();
        Error_t     deinit();
        Error_t     enable();
        Error_t     disable();
        uint16_t    ADCRead();
};

/** @} */

}

#endif /** HSS_PAL_ADC_Esp_HPP **/