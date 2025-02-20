#pragma once

#include <stdio.h>
#include <iostream>
#include <array>

#include "gpio_pal.h"



class WaterSensor{
    public:
    int lvl;
    WaterSensor(std::array<GPIOPAL*, 4> gpios);
    WaterSensor(GPIOPAL *gpios);
    float read();
};