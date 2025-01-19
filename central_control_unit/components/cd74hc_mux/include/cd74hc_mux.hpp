
#include <stdio.h>
#include <esp_io_expander.h>
#include <esp_log.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_pthread.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#ifndef CD74HC_MUX_HPP
#define CD74HC_MUX_HPP


class Cd74HCMux{
    public:
    // use expander for channel selection
    //Cd74HCMux();
     // use normal chip GPIOs for channel selection
    Cd74HCMux(esp_io_expander_handle_t _expander_handle, esp_io_expander_pin_num_t s0, esp_io_expander_pin_num_t s1, esp_io_expander_pin_num_t s2, esp_io_expander_pin_num_t s3);   
    ~Cd74HCMux();

    void enable();
    void enable_channel(uint8_t channel);

    private:
    esp_io_expander_handle_t expander_handle;
    uint8_t n_channels;
    //esp_io_expander_pin_num_t enable_pin;
    esp_io_expander_pin_num_t ch_select_pin_0;
    esp_io_expander_pin_num_t ch_select_pin_1;
    esp_io_expander_pin_num_t ch_select_pin_2;
    esp_io_expander_pin_num_t ch_select_pin_3;
};

#endif