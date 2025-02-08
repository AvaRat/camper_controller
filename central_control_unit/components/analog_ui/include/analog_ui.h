#ifndef ANALOG_UI_H_
#define ANALOG_UI_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "esp_log.h"


#include "led_indicators.h"

using namespace std;

class CamperUi
{
    public:
    virtual void update_ccu() = 0; //send commands to ccu
    virtual void read_ccu_state() = 0; //receive state from ccu
};

//TODO change to vectors
#define MAX_UI_NAME_LEN 32
#define N_BUTTONS 5

typedef struct {
    gpio_num_t gpio;
    char name[MAX_UI_NAME_LEN];
}ButtonConfig_t;

typedef vector<ButtonConfig_t>  AnalogUiButtonsConfig_t;

struct AnalogUiConfig_t{
    uint16_t update_frequency;
    gpio_num_t led_strip_gpio;
    AnalogUiButtonsConfig_t buttons_config;
    vector<uint16_t> water_lvl_leds;
    vector<uint16_t> normal_leds;
};



class AnalogWaterLvlIndicator{
    public:
    AnalogWaterLvlIndicator();

    void assign_indicators(vector<LedIndicator*> indicators);
    void add_indicator(LedIndicator*);
    void update_lvl(uint16_t new_lvl);
    
    private:
    uint16_t current_lvl = 0; // 0-100%
   // LedIndicator indicators[ANALOG_WATER_LVL_INDICATOR_SIZE];
    std::vector<LedIndicator*> indicators;
};

/*
* Update UI event-based
* or in fixed frequency intervals
*
*/
class AnalogUI: public CamperUi
{
    public:

    AnalogUI(AnalogUiConfig_t analog_ui_config_);

    void update_ccu(){
        cout << "update sent from ccu\n";
    }
    void read_ccu_state(){
        cout << "read state from ccu and update UI\n";
    }
    void init_hardware();

    void update();
    void loop();
    void test_fun();

    LedStripDriver led_driver;
    
    private:
    uint16_t update_frequency;
    
    vector<LedIndicator*> led_indicators;
    //NOT USED currently
    int buttons[N_BUTTONS] = {};
    AnalogWaterLvlIndicator water_lvl_indicator;
};



#endif