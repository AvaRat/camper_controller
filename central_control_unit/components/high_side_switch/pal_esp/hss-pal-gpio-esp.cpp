/**
 * @file        hss-pal-gpio-Esp.cpp
 * @brief       GPIO platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "hss-pal-gpio-esp.hpp"


using namespace idf;
using namespace hss;

/**
 * @brief    Constructor of the  GPIO class for Esp
 * @details  This function is setting the basics for a GPIO. It allows to set the pin number,
 *           mode of the pin and the logic level.
 * @param[in]   pin     Number of the desired pin
 * @param[in]   logic   Defines the logic level of the pin
 */
GPIOEsp::GPIOEsp(uint32_t pin, VLogic_t _logic): gpio(GPIO_Output(GPIONum(pin))), logic(_logic)
{
}

/**
 * @brief   Initialize the GPIO
 * @details This function is initializing the chosen pin.
 * @return  High-side switch error code
 */
Error_t GPIOEsp::init()
{
   // pinMode(this->pin, this->mode);
    return OK;
}

/**
 * @brief   Deinitialize the GPIO
 * @details This function is deinitializing the chosen pin.
 * @return  High-side switch error code
 */
Error_t GPIOEsp::deinit()
{
    return OK;
}

/**
 * @brief   [DEPRECIATED] Read GPIO logic level 
 * @details This function reads the logic level of the chosen pin and
 *          returns the logic level value.
 * @return  VLevel_t
 * @retval  0    GPIO_LOW
 * @retval  1    GPIO_HIGH
 */
GPIOEsp::VLevel_t GPIOEsp::read()
{
   // return (VLevel_t) digitalRead(this->pin);
   return GPIOEsp::GPIO_LOW;
}

/**
 * @brief   Set GPIO logic level
 * @details This functions sets the logic level of the chosen pin.
 * @param[in]   level   Desired logic level of the pin
 * @return High-side switch error code
 */
Error_t GPIOEsp::write(VLevel_t level)
{
    level ? gpio.set_high() : gpio.set_low();
    return OK;
}

/**
 * @brief   Enable the GPIO
 * @details This functions enable the chosen pin. Depending on the chosen logic of the pin
 *          it sets the right logic level of the pin.
 * @return  High-side switch error code
 */
Error_t GPIOEsp::enable()
{
    // if(this->logic == POSITIVE){
    //     digitalWrite(this->pin, GPIO_HIGH);
    // }
    // else if(this->logic == NEGATIVE){
    //     digitalWrite(this->pin, GPIO_LOW);;
    // }
    return OK;
}

/**
 * @brief   Disable the GPIO
 * @details This functions disables the chosen pin. Depending on the chosen logic of the pin
 *          it sets the right logic level of the pin.
 * @return  High-side switch error code
 */
Error_t GPIOEsp::disable()
{
    // if(this->logic == POSITIVE){
    //     digitalWrite(this->pin, GPIO_LOW);
    // }
    // else if(this->logic == NEGATIVE){
    //     digitalWrite(this->pin, GPIO_HIGH);
    // }
    return OK;
}





