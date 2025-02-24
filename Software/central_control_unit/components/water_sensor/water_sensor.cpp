#include "water_sensor.h"

WaterSensor::WaterSensor(std::array<GPIOPAL*, 4> gpios){
    std::cout <<std::endl;
} 

WaterSensor::WaterSensor(GPIOPAL *gpio) {

}

float WaterSensor::read(){
    return 0.5;
}
