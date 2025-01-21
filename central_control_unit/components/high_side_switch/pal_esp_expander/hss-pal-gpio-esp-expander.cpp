/**
 * @file        hss-pal-gpio-Esp.cpp
 * @brief       GPIO platform abstraction layer ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "hss-pal-gpio-esp-expander.hpp"


using namespace hss;


static const char *TAG = "pal-gpio-esp-expander";

/**
 * @brief    Constructor of the  GPIO class for Esp
 * @details  This function is setting the basics for a GPIO. It allows to set the pin number,
 *           mode of the pin and the logic level.
 * @param[in]   pin     Number of the desired pin
 * @param[in]   logic   Defines the logic level of the pin
 */
EspExpanderGPIO::EspExpanderGPIO(
    esp_io_expander_handle_t _expander_handle, 
    esp_io_expander_pin_num_t _pin,
    esp_io_expander_dir_t _mode, 
    VLogic_t _logic)
:expander_handle(_expander_handle), pin(_pin), mode(_mode), logic(_logic)
{
    ESP_ERROR_CHECK(esp_io_expander_set_dir(expander_handle, pin, mode));
}

/**
 * @brief   Initialize the GPIO
 * @details This function is initializing the chosen pin.
 * @return  High-side switch error code
 */
Error_t EspExpanderGPIO::init()
{
   // pinMode(this->pin, this->mode);
    return OK;
}

/**
 * @brief   Deinitialize the GPIO
 * @details This function is deinitializing the chosen pin.
 * @return  High-side switch error code
 */
Error_t EspExpanderGPIO::deinit()
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
EspExpanderGPIO::VLevel_t EspExpanderGPIO::read()
{
   // return (VLevel_t) digitalRead(this->pin);
   uint32_t pin_lvl = 99;
   ESP_ERROR_CHECK(esp_io_expander_get_level(expander_handle, pin, &pin_lvl));
   if(pin_lvl==99)
   {
    ESP_LOGE(TAG, "CANNOT get expander pin state");
   }
   return VLevel_t(pin_lvl);
}

/**
 * @brief   Set GPIO logic level
 * @details This functions sets the logic level of the chosen pin.
 * @param[in]   level   Desired logic level of the pin
 * @return High-side switch error code
 */
Error_t EspExpanderGPIO::write(VLevel_t level)
{
    ESP_ERROR_CHECK(esp_io_expander_set_level(expander_handle, pin , level));
    return OK;
}

/**
 * @brief   Enable the GPIO
 * @details This functions enable the chosen pin. Depending on the chosen logic of the pin
 *          it sets the right logic level of the pin.
 * @return  High-side switch error code
 */
Error_t EspExpanderGPIO::enable()
{
    //ESP_LOGI(TAG, "enabling hss gpio : %d", pin);
    if(this->logic == EspExpanderGPIO::POSITIVE){
        esp_io_expander_set_level(expander_handle, pin , 1);
    }
    else if(this->logic == EspExpanderGPIO::NEGATIVE){
        esp_io_expander_set_level(expander_handle, pin , 0);
    }
    return OK;
}

/**
 * @brief   Disable the GPIO
 * @details This functions disables the chosen pin. Depending on the chosen logic of the pin
 *          it sets the right logic level of the pin.
 * @return  High-side switch error code
 */
Error_t EspExpanderGPIO::disable()
{
    if(this->logic == POSITIVE){
        esp_io_expander_set_level(expander_handle, pin , 0);
    }
    else if(this->logic == NEGATIVE){
        esp_io_expander_set_level(expander_handle, pin , 1);
    }
    return OK;
}





