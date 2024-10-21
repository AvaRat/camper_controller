#include "gpio/gpio_extended.hpp"



ExtendedGPIO_Output::ExtendedGPIO_Output(uint32_t io_num, string name)
    : num(io_num), name(name), expander_handle(nullptr)
{
    standard_gpio = new GPIO_Output(GPIONum(io_num));
};

ExtendedGPIO_Output::ExtendedGPIO_Output(esp_io_expander_handle_t *expander_handle, esp_io_expander_pin_num_t expander_pin, string name)
    : num(expander_pin), name(name), standard_gpio(nullptr), expander_handle(expander_handle)
{
    esp_io_expander_set_dir(*expander_handle,  expander_pin, IO_EXPANDER_OUTPUT);
};

void ExtendedGPIO_Output::set_high(){
    if(!standard_gpio){
        ESP_ERROR_CHECK(esp_io_expander_set_level(*expander_handle,  num, 1));
    }else
        standard_gpio->set_high();
}

void ExtendedGPIO_Output::set_low(){
    if(!standard_gpio){
        ESP_ERROR_CHECK(esp_io_expander_set_level(*expander_handle,  num, 0));
    }else
        standard_gpio->set_low();
}

void ExtendedGPIO_Output::set_output(bool output_value){
    if(output_value){
        this->set_high();
    }else
        this->set_low();
}

