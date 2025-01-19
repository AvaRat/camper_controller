/**
 * @file        hss-pal-timer-esp.hpp
 * @brief       Timer platform abstraction layer ESP class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_PAL_TIMER_Esp_HPP_
#define HSS_PAL_TIMER_Esp_HPP_


#include <thread>
#include <chrono>

#include "hss-pal-timer.hpp"

namespace hss
{

/**
 * @addtogroup arduEspPal
 * @{
 */

/**
 * @class TimerEsp
 * @brief Timer platform abstraction layer ArduEsp API
 */
class TimerEsp: virtual public TimerPAL
{
    public:

        TimerEsp();
        ~TimerEsp();
        Error_t init();
        Error_t deinit();
        Error_t start();
        Error_t elapsed(uint32_t &elapsed);
        Error_t stop();
        Error_t delayMilli(uint32_t timeout);
        Error_t delayMicro(uint32_t timeout);

    private:

        uint32_t startTime;
};

/** @} */

}

#endif /** HSS_PAL_TIMER_Esp_HPP_ **/