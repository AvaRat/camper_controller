#include <iostream>
#include <stdlib.h>
#include "gpio_cxx.hpp"
#include <vector>
#include <bitset>


using namespace std;
using namespace idf;


typedef enum {
    NORMAL_OPERATION,
    OVERCURRENT,
    SHORT_CIRCUIT
};


class SmartPowerSwitch{
    public:
    SmartPowerSwitch();
    void set_high();
    void set_low();


    private:
    bool enabled;
    float nominal_load; // in amps
    bool diagnostics_enabled;

    uint16_t output_pin;
    uint16_t diagnostics_enable_pin;
    uint16_t analog_channel;
};