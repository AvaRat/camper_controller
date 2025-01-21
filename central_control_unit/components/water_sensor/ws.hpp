#pragma once

#include <stdio.h>
#include <iostream>

struct WS_Struct
{
    float           iisEn;          /**< Leakage current when channel is enabled */
    float           iisO;           /**< Sense offset */
};

extern WS_Struct ws_s;

class WaterSensor{
    public:
    int lvl;
    WaterSensor(WS_Struct ws);
    float read();
};