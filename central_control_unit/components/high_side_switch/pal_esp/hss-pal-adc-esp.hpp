/**
 * @file        hss-pal-adc-Esp.hpp
 * @brief       ADC platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_PAL_ADC_Esp_HPP
#define HSS_PAL_ADC_Esp_HPP

#include "hss-pal-adc.hpp"
#include <esp_io_expander.h>

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
class ADCEsp : virtual public ADCPAL
{
    private:

        uint8_t     pin;   /**< ADC Pin number */

    public:

                    ADCEsp(esp_io_expander_handle_t handle, mux_config_t conf);
                    ADCEsp(uint8_t pin);
                    ~ADCEsp();
        Error_t     init();
        Error_t     deinit();
        Error_t     enable();
        Error_t     disable();
        uint16_t    ADCRead();
        Error_t     ADCWrite(uint8_t value);
};

/** @} */

}

#endif /** HSS_PAL_ADC_Esp_HPP **/