/**
 * @file        hss-pal-gpio-Esp.hpp
 * @brief       GPIO platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_PAL_GPIO_Esp_Expander_HPP
#define HSS_PAL_GPIO_Esp_Expander_HPP


#include <cstdlib>
#include <thread>

#include <esp_io_expander.h>
#include <esp_log.h>

#include "hss-pal-gpio.hpp"


/**
 * @class  GPIOEsp
 * @brief  GPIO platform abstraction layer Esp API
 */
class EspExpanderGPIO: virtual public hss::GPIOPAL
{
private:
    esp_io_expander_handle_t expander_handle;
    esp_io_expander_pin_num_t pin;
    esp_io_expander_dir_t mode;
    VLogic_t logic;

public:

    static constexpr uint8_t  unusedPin = 0xFFU;    /**< Unused pin */

    EspExpanderGPIO(esp_io_expander_handle_t _expander_handle, esp_io_expander_pin_num_t _pin, esp_io_expander_dir_t mode, hss::GPIOPAL::VLogic_t _logic);
    ~EspExpanderGPIO();
    hss::Error_t     init();
    hss::Error_t     deinit();
    VLevel_t    read();
    hss::Error_t     write(VLevel_t level);
    hss::Error_t     enable();
    hss::Error_t     disable();
};


#endif /** HSS_PAL_GPIO_Esp_HPP_ **/
