#include <iostream>
#include <cstdlib>
#include "iot_button.h"
#include "functional"


#include "../power_switch/BTS_power_switch.hpp"



class ControlPanel{
    public:
    
    // Static callback function to pass to the C API
    static void buttonEventHandler(void *button_handle, void *usr_data) {
        // Cast usr_data back to MyButtonHandler instance
        ControlPanel* handler = static_cast<ControlPanel*>(usr_data);
        cout << usr_data << endl;
        // Call the actual member function
        handler->onButtonEvent();
    };

    void onButtonEvent() {
        ESP_LOGI("ControlPanel/onButtonEvent", "Unknown Event");
        cout << "this class address: " << hex << this << endl;
        cout << ch1 << endl;
    };

    ControlPanel(BTS_Driver ch1_drv): ch1(ch1_drv){
        // create gpio button
        button_config_t gpio_btn_cfg = {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = 3000,
            .short_press_time = 100,
            .gpio_button_config = {
                .gpio_num = 13,
                .active_level = 0,
            }
        };

        button_handle_t gpio_btn_ch1 = iot_button_create(&gpio_btn_cfg);
        gpio_btn_cfg.gpio_button_config.gpio_num = 12;
        button_handle_t gpio_btn_ch1_diag_en = iot_button_create(&gpio_btn_cfg);

      //  typedef void (* button_cb_t)(void *button_handle, void *usr_data);



        iot_button_register_cb(gpio_btn_ch1, BUTTON_SINGLE_CLICK, buttonEventHandler,this);
        iot_button_register_cb(gpio_btn_ch1_diag_en, BUTTON_SINGLE_CLICK, buttonEventHandler,this);
    }




    private:
    BTS_Driver ch1;

    void ch1_toggle(void *button_handle, void *usr_data){
            ESP_LOGI("ControlPanel", "CH1 Toggle"); 
            ch1.toggle();
    };

    void ch1_diag_toggle(void *button_handle, void *usr_data){
            ESP_LOGI("ControlPanel", "CH1_DIAG_EN Toggle");
            ch1.diag_toggle();
    };

};

