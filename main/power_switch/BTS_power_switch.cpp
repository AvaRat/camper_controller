#include "BTS_power_switch.hpp"


BTS_Driver::BTS_Driver(uint8_t _diagnostics_pin, uint8_t _diagnostics_enable_pin, uint8_t _output_enable_pin):
diagnostics_pin(_diagnostics_pin), diagnostics_enable_gpio(GPIONum(_diagnostics_enable_pin)), output_enable_gpio(GPIONum(_output_enable_pin))
{
    ch_enable = 0;
    diag_enable = 1;
    output_enable_gpio.set_low();
    diagnostics_enable_gpio.set_high();
}

void BTS_Driver::toggle() {
    if(ch_enable){
        this->enable();
    }else
        this->disable();
    ch_enable = !ch_enable;
}

void BTS_Driver::diag_toggle(){
    if(diag_enable){
        diagnostics_enable_gpio.set_high();
    }else
        diagnostics_enable_gpio.set_low();
    diag_enable = !diag_enable;
}

void BTS_Driver::enable(){
    output_enable_gpio.set_high();
}

void BTS_Driver::disable(){
    output_enable_gpio.set_low();
}

ostream& operator<<(ostream& os, const BTS_Driver& obj){
    os << ((obj.ch_enable == 1) ? "CH ON" : "CH OFF");
    os << endl << ((obj.diag_enable) ? "diag ON" : "diag OFF");
    //os << obj.
    return os;

}