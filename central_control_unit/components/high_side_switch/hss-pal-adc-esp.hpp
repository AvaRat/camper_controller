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

namespace hss
{

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

                    ADCEsp();
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