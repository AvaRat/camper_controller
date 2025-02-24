/**
 * @file        hss-arduEsp.cpp
 * @brief       High side switch ArduEsp class
 * @copyright   Copyright (c) 2021 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */
#include "hss-esp.hpp"

using namespace hss;

/**
 * @brief   Constructor of the ArduEsp High-Side-Switch class
 * @details This functions is the default constructor of the ArduEsp High-Side-Switch class.
 *          There are different variants of the High-Side-Switch, therefore you have to chose one.
 * @param[in]   den           Pin number of the diagnostic enable pin
 * @param[in]   in            Pin number of the input pin
 * @param[in]   is            Pin number of the sense current output
 * @param[in]   btxVariant    Shield Variant type
 */
HssEsp::HssEsp(uint8_t den, uint8_t in, uint8_t is, BtxVariants_t * btxVariant)
: Hss(new GPIOEsp(den, GPIOEsp::POSITIVE),
      new GPIOEsp(in, GPIOEsp::POSITIVE),
      new ADCEsp(is),
      new TimerEsp(),
      btxVariant)
{}




/**
 * @brief Destructor of the ArduEsp High-Side-Switch
 *
 */
HssEsp::~HssEsp()
{

//     delete den;
//     delete in0;
//     delete in1;
//     delete dsel;
//     delete is;
//     delete timer;
}