
#ifndef BTS_PWR_SWITCH
#define BTS_PWR_SWITCH

#include <iostream>
#include <cstdlib>
#include "esp_log.h"
#include "gpio_cxx.hpp"




using namespace idf;
using namespace std;


class BTS_Driver{
    public:
    BTS_Driver(uint8_t _diagnostics_enable_pin, uint8_t _output_enable_pin);
    void enable();
    void disable();
    void toggle();
    void diag_toggle();
    friend ostream& operator<<(ostream& os, const BTS_Driver& obj);
    

    private:
    GPIO_Output diagnostics_enable_gpio;
    GPIO_Output output_enable_gpio;
    bool ch_enable;
    bool diag_enable;
};


#endif