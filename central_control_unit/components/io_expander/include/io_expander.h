#include <iostream>
#include <stdio.h>
#include <memory>
#include <vector>

#include "esp_io_expander.h"
#include <gpio_cxx.hpp>
#include "i2c_cxx.hpp"

#include "gpio_pal.h"

using namespace std;
using namespace idf;


class IoExpander{
    public:
    IoExpander(I2CMaster *i2c_);

    void init();
    void reset();
    GPIOPAL* get_channel(uint32_t channel);
    
};