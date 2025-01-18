#include "ws.hpp"

WaterSensor::WaterSensor(WS_Struct ws):lvl(0){
    std::cout << ws.iisEn << std::endl;
}

float WaterSensor::read(){
    return 0.67;
}

WS_Struct ws_s = {0.56, 9.123};