/**
 * @file        hss-pal-gpio-Esp.hpp
 * @brief       GPIO platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_PAL_GPIO_Esp_HPP_
#define HSS_PAL_GPIO_Esp_HPP_


#include <cstdlib>
#include <thread>
#include "gpio_cxx.hpp"

#include "hss-pal-gpio.hpp"


/**
 * @class  GPIOEsp
 * @brief  GPIO platform abstraction layer Esp API
 */
class GPIOEsp: virtual public hss::GPIOPAL
{
private:
    idf::GPIO_Output gpio;
    hss::GPIOPAL::VLogic_t logic;

public:

    static constexpr uint8_t  unusedPin = 0xFFU;    /**< Unused pin */

    GPIOEsp(uint32_t pin, hss::GPIOPAL::VLogic_t logic);
    ~GPIOEsp();
    hss::Error_t     init();
    hss::Error_t     deinit();
    VLevel_t    read();
    hss::Error_t     write(VLevel_t level);
    hss::Error_t     enable();
    hss::Error_t     disable();
};


#endif /** HSS_PAL_GPIO_Esp_HPP_ **/
