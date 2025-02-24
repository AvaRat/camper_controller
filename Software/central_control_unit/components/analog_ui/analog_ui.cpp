#include "analog_ui.h"

static const char *TAG = "analog_ui";

AnalogUI::AnalogUI(AnalogUiConfig_t analog_ui_config_)
:      led_driver(LedStripDriver(analog_ui_config_.led_strip_gpio, 
    analog_ui_config_.water_lvl_leds.size() + analog_ui_config_.normal_leds.size())),
    update_frequency(analog_ui_config_.update_frequency),

    water_lvl_indicator(AnalogWaterLvlIndicator())
{
    //create interrupts for buttons
    //create led_strip 

    led_indicators = led_driver.get_all_indicators();
    for(auto i:analog_ui_config_.water_lvl_leds){
        water_lvl_indicator.add_indicator(led_indicators[i]);
    }
    //led_indicators[5]->flash(500);   
}



void AnalogUI::init_hardware(){
    led_driver.init_hardware();
    ESP_LOGI(TAG, "analog UI hardware initialized");
}

void AnalogUI::test_fun(){
    auto led5 = led_indicators.back();
    //ESP_LOGI(TAG, "end iterator: %d", led5);
    led5->set_color(Color::green());

    led5->flash(500);
}

void AnalogUI::loop(){

    while(1){
        update();
        this_thread::sleep_for(chrono::milliseconds(1000/update_frequency));
        //cout << ".\n";
        //led_driver.
    }
}

void AnalogUI::update(){
    // transmit to LEDs via led_driver
    // for(int i=100; i>10; i=i-10){
    //     water_lvl_indicator.update_lvl(i);
    //     led_driver.update_all_leds();
    //     this_thread::sleep_for(chrono::milliseconds(200));
    // }   
    led_driver.update_all_leds();
}

AnalogWaterLvlIndicator::AnalogWaterLvlIndicator():
    current_lvl(0)
{

}

void AnalogWaterLvlIndicator::assign_indicators(vector<LedIndicator*> indicators_){
    indicators = indicators_;
}

void AnalogWaterLvlIndicator::add_indicator(LedIndicator* led_){
    indicators.push_back(led_);
    led_->set_color(Color::blue());
    cout << "indicators added"<< endl;
}



void AnalogWaterLvlIndicator::update_lvl(uint16_t new_lvl){
    uint16_t limit = (uint16_t) ((new_lvl * indicators.size())/100);
    //cout << "limit: " << limit << " indicators size: " << indicators.size() << endl;
    for(uint16_t i=0; i<limit; i++)
    {
        indicators[i]->on();
    }
    for(uint16_t i=limit; i<indicators.size(); i++)
    {
        indicators[i]->off();
    }
    current_lvl = new_lvl;
}