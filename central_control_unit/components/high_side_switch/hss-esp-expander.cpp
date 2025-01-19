/**
 * @file        hss-arduEsp.cpp
 * @brief       High side switch ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */
#include "hss-esp-expander.hpp"

static const char *TAG = "hss-esp-expander";


using namespace hss;

/**
 * @brief   Constructor of the ArduEsp High-Side-Switch class
 * @details This functions is the default constructor of the ArduEsp High-Side-Switch class.
 *          There are different variants of the High-Side-Switch, therefore you have to chose one.
 * @param[in]   expander_handle_   handle to expander 
 * @param[in]   den           Pin number of the diagnostic enable pin
 * @param[in]   in            Pin number of the input pin
 * @param[in]   is            Pin number of the sense current output -> routed via MUX
 * @param[in]   btxVariant    Shield Variant type
 * Cd74HCMux *_mux, uint8_t _mux_channel);
 */
//esp_io_expander_handle_t _expander_handle, esp_io_expander_pin_num_t _pin, hss::GPIOPAL::VLogic_t _logic);
HssEspExpander::HssEspExpander(esp_io_expander_handle_t _expander_handle, esp_io_expander_pin_num_t den, esp_io_expander_pin_num_t in, 
    Cd74HCMux *_mux, uint8_t _mux_channel, adc_oneshot_unit_handle_t _adc_handle, adc_channel_t _adc_channel,
    BtxVariants_t *variant)
: Hss(new EspExpanderGPIO(_expander_handle, den, IO_EXPANDER_OUTPUT, EspExpanderGPIO::POSITIVE), 
    new EspExpanderGPIO(_expander_handle, in, IO_EXPANDER_OUTPUT, EspExpanderGPIO::POSITIVE), 
    new ADCEspExpander(_mux, _mux_channel, _adc_handle, _adc_channel), 
    new TimerEsp(),  
    variant)
{}

// HssEspExpander::HssEspExpander(esp_io_expander_handle_t handle, mux_config_t conf)
// : Hss(new GPIOEsp(0, GPIOEsp::POSITIVE), new GPIOEsp(1, GPIOEsp::POSITIVE), new ADCEsp(handle, conf), new TimerEsp(), &hss::BTS7006)
// {}

/**
 * @brief Destructor of the ArduEsp High-Side-Switch
 *
 */
HssEspExpander::~HssEspExpander()
{

//     delete den;
//     delete in0;
//     delete in1;
//     delete dsel;
//     delete is;
//     delete timer;
}


void HssEspExpander::print_diag(int diag_status)
{
    // enum DiagStatus_t{
    //     DIAG_READ_ERROR     = -1,   /**< Read Error */
    //     NOT_ENABLED         = -2,   /**< Diagnosis not enabled */
    //     NORMAL              = 0,    /**< Switch works correctly */
    //     FAULT               = 1,    /**< Switch is in fault condition (Is_fault at IS pin), which can mean "Short to GND/VS", "Overtemperature" or "Overload" */
    //     FAULT_OL_IC         = 2,    /**< Switch is is either in Open Load (whit enable channel) or inverse current is flowing */
    //     SHORT_TO_GND_OR_OT  = 3,    /**< Short to the ground or Overtemperature detected*/
    //     SHORT_TO_VSS        = 5,    /**< Short to the supply voltage */
    //     OPEN_LOAD           = 6,    /**< Open load detected */
    // };
    char msg[25] = {};
    switch(diag_status)
    {
        case -1:
            strncpy(msg, "DIAG_READ_ERROR", 20);
            break;
        case -2:
            strncpy(msg, "NOT_ENABLED", 20);
            break; 
        case 0:
            strncpy(msg, "NORMAL", 20);
            break; 
        case 1:
            strncpy(msg, "FAULT", 20);
            break; 
        case 2:
            strncpy(msg, "FAULT_OL_IC", 20);
            break; 
        case 3:
            strncpy(msg, "SHORT_TO_GND_OR_OT", 20);
            break; 

        case 5:
            strncpy(msg, "SHORT_TO_VSS", 20);
            break; 

        case 6:
            strncpy(msg, "OPEN_LOAD", 20);
            break;
        default:
            strncpy(msg, "unknown ERROR CODE", 20);  
    }
    std::cout << msg << std::endl;
    ESP_LOGI(TAG, "%s", msg);
}
