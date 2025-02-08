#ifndef LED_INDICATORS_H_
#define LED_INDICATORS_H_

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include <gpio_cxx.hpp>

#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_strip.h"
#include "esp_timer_cxx.hpp"


using namespace std;
using namespace idf;

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 

struct Color
{
    static constexpr Color red() { return Color(255, 0, 0); }
    static constexpr Color green() { return Color(0, 255, 0); }
    static constexpr Color blue() { return Color(0, 0, 255); }
    static constexpr Color black() { return Color(0, 0, 0); }
    static constexpr Color white() { return Color(255, 255, 255); }

    constexpr Color(uint8_t r, uint8_t g, uint8_t b)
        : r(r), g(g), b(b)
    {}

     uint8_t r;
     uint8_t g;
     uint8_t b;
};


typedef vector<uint16_t> LedIndicatorConfig_t;

class LedIndicator{
    public:
    // (index, red, green, blue)
    typedef std::function<void(uint8_t, uint8_t, uint8_t, uint8_t)> led_driver_call_t;
    LedIndicator(led_driver_call_t driver_call);
    LedIndicator(uint32_t index, uint16_t default_brightness, led_driver_call_t driver_call);
    ~LedIndicator();

    void on();
    void off();
    void toggle();
    void flash(uint16_t period_ms);
    void set_color(Color new_color);
    void set_brightness(uint16_t brightness);


    private:
    uint32_t pixel_index;
    Color color;
    uint16_t brightness;
    led_driver_call_t led_driver_call;
    idf::esp_timer::ESPTimer *flash_timer;

    bool is_on;

};

class LedStripDriver{
    public:
    LedStripDriver(gpio_num_t gpio, uint16_t n_leds);
    ~LedStripDriver();
    
    void init_hardware();
    void update_all_leds();
    vector<LedIndicator*> get_all_indicators();

    std::vector<LedIndicator*> leds;

    private:
    void set_pixel(uint32_t index, uint32_t r, uint32_t g, uint32_t b);
    uint16_t strip_length;
    
    led_strip_handle_t strip_handle;
    //uint16_t update_frequency = 100; //times per seccond

    led_strip_rmt_config_t rmt_config;
    led_strip_config_t strip_config;
};



#endif