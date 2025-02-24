/**
 * @file        hss-arduEsp.hpp
 * @brief       High side switch Esp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef HSS_ESP_EXPANDER_HPP
#define HSS_ESP_EXPANDER_HPP

#include "esp_log.h"

#include "string.h"
#include <esp_io_expander.h>
#include "hss.hpp"
//#include "hss-pal-gpio-esp.hpp"
#include "hss-pal-timer-esp.hpp"

#include "hss-pal-adc-esp-expander.hpp"
#include <esp_io_expander.h>
#include "hss-pal-gpio-esp-expander.hpp"
#include "cd74hc_mux.hpp"



namespace hss
{

/**
 * @class   HssEsp
 * @brief   High side switch Esp API
 */
class HssEspExpander : public Hss
{
    public:
        //HssEspExpander(esp_io_expander_handle_t handle, mux_config_t conf);
        HssEspExpander(esp_io_expander_handle_t expander_handle_, esp_io_expander_pin_num_t den, esp_io_expander_pin_num_t in, 
        Cd74HCMux *_mux, uint8_t _mux_channel, adc_oneshot_unit_handle_t _adc_handle, adc_channel_t _adc_channel,
        BtxVariants_t *variant);
        //HssEsp(uint8_t den, uint8_t in0, uint8_t in1, uint8_t dsel, uint8_t is, BtxVariants_t * btxVariant);
        ~HssEspExpander();
        void print_diag(int diag_status);
};

}

#endif /** HSS_ARDUEsp_HPP_ **/