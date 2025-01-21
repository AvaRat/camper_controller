/**
 * @file        hss-pal-timer-Esp.cpp
 * @brief       Timer platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "hss-pal-timer-esp.hpp"

using namespace std;
using namespace hss;

/**
 * @brief Constructor of the Timer class for arduEsp
 */
TimerEsp::TimerEsp()
{

}

/**
 * @brief Destructor of the Timer class for arduEsp
 *
 */
TimerEsp::~TimerEsp()
{

}

/**
 * @brief   Initialization of the Timer
 * @details This function is initializing the Timer and sets
 *          the elapsed time to zero.
 * @return High-side switch error code
 */
Error_t TimerEsp::init()
{
    startTime = 0;
    return OK;
}

/**
 * @brief   Deinitialize the Timer
 * @details This function deinitialize the Timer and also
 *          resets the elapsed time variable.
 * @return High-side switch error code
 */
Error_t TimerEsp::deinit()
{
    startTime = 0;
    return OK;
}

/**
 * @brief   Start the timer
 * @details This function is starting the timer.
 * @return High-side switch error code
 */
Error_t TimerEsp::start()
{
    startTime = 0;
    return OK;
}

/**
 * @brief   Calculate the elapsed time
 * @details This function is calculating the elapsed time since the
 *          start of the timer. The value stored in the given variable
 *          is in milliseconds.
 * @param[in]   &elapsed      Address of a value where the elapsed time should be stored
 * @return High-side switch error code
 */
Error_t TimerEsp::elapsed(uint32_t &elapsed)
{
    elapsed = 0;
    return OK;
}

/**
 * @brief   Stop the timer
 * @details This function stops the timer and resets the
 *          start time variable.
 * @return High-side switch error code
 */
Error_t TimerEsp::stop()
{
    startTime = 0;
    return OK;
}

/**
 * @brief   Time delay
 * @details This function is causing a desired delay of the application.
 *          The input value is given in milliseconds.
 * @param[in]   timeout                 Desired timeout in ms
 * @return High-side switch error code
 */
Error_t TimerEsp::delayMilli(uint32_t timeout)
{
   // delay(timeout);
    this_thread::sleep_for(chrono::milliseconds(timeout));
    return OK;
}

/**
 * @brief   Time delay
 * @details This function is causing a desired delay of the application.
 *          The input value is given in microseconds.
 * @param[in]   timeout                 Desired timeout in us
 * @return High-side switch error code
 */
Error_t TimerEsp::delayMicro(uint32_t timeout)
{
    //delayMicroseconds(timeout);
    this_thread::sleep_for(chrono::microseconds(timeout));
    return OK;
}

