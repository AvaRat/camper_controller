/**
 * @file        hss-arduEsp.hpp
 * @brief       High side switch Esp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_ESP_HPP_
#define HSS_ESP_HPP_

#include "hss.hpp"
#include "hss-pal-gpio-esp.hpp"
#include "hss-pal-adc-esp.hpp"
#include "hss-pal-timer-esp.hpp"


namespace hss
{

/**
 * @class   HssEsp
 * @brief   High side switch Esp API
 */
class HssEsp : public Hss
{
    public:
        //HssEsp(uint8_t in, uint8_t is, BtxVariants_t * btxVariant);
        HssEsp(uint8_t den, uint8_t in, uint8_t is, BtxVariants_t * btxVariant);
        //HssEsp(uint8_t den, uint8_t in0, uint8_t in1, uint8_t dsel, uint8_t is, BtxVariants_t * btxVariant);
        ~HssEsp();
};

}

#endif /** HSS_ARDUEsp_HPP_ **/