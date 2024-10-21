#ifndef GPIO_EXTENDED_HPP
#define GPIO_EXTENDED_HPP

#include "esp_log.h"
#include "esp_io_expander_tca95xx_16bit.h"
#include "driver/i2c.h"
#include "config.h"
#include "gpio_cxx.hpp"
#include <vector>
#include <map>
#include <bitset>
#include <variant>

using namespace std;
using namespace idf;


class ExtendedGPIO_Output{
    public: 
    ExtendedGPIO_Output(uint32_t io_num, string name);
    ExtendedGPIO_Output(esp_io_expander_handle_t *expander_handle, esp_io_expander_pin_num_t expander_pin_num, string name);
    void set_high();
    void set_low();
    void set_output(bool output_value);

    private:
    uint32_t num;
    string name;

    GPIO_Output *standard_gpio;
    esp_io_expander_handle_t *expander_handle;
};


#endif
