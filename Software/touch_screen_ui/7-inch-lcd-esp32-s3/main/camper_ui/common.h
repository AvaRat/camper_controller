
/**
 * @file common.h
 *
 */

#ifndef COMMON_H
#define COMMON_H


#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"
#include "driver/ledc.h"
#include "esp_common.h"


static lv_style_t camper_style_title;
static const lv_font_t * font_large;
static const lv_font_t * font_normal;

#define BTN_MAX_TEXT_LEN 15

#define SCREENSAVER_TIMEOUT 1000000 // 5 seconds in ms
static bool screensaver_active = false;
static int old_brightness = 0;
#define SCREENSAVER_BRIGHTNESS 50

//static lv_color_t BUTTON_OUTLINE_COLOR = lv_color_black();


typedef struct camperstate_t{
    int house_battery_lvl;
    float house_battery_voltage;
    float house_battery_current;

    float start_battery_voltage;
    float start_battery_current;

    float pv_voltage;
    float pv_current;

    float alternator_voltage;
    float alternator_current;

    float dc_loads_voltage;
    float dc_loads_current;

    float inverter_voltage;
    float inverter_current;

    float grid_voltage;
    float grid_current;

    int32_t water_tank_lvl; //0-100 %
    int32_t temp_inside;
    int32_t temp_outside;
}CamperState_t;



typedef struct powerswitch_t{
    bool state;
    char btn_text[BTN_MAX_TEXT_LEN];
    const char btn_image_path[MAX_BTN_IMG_PATH];
    int max_current; //mA
    bool pwm_enabled;
    int pwm_percent; //0-100
}PowerSwitch_t;


CamperState_t CamperState = {
    55, 12, 10, //house batt
    12.5, 0,    //start batt
    33, 5,      //pv
    0, 0,       //alternator
    13.1, 5,     //dc loads
    11.5, 10,    //inverter
    0, 0,  //grid
    50, 17, 10 //water, T_in, T_out
};

static PowerSwitch_t MainSwitch = {1, "Mains", ICONS_MOUNT_POINT"pwr_64.png", 20, 0, 0};
static PowerSwitch_t LightSwitch = {1, "Lights", ICONS_MOUNT_POINT"lights_64.png", 5, 1, 100};
static PowerSwitch_t WaterPumpSwitch = {0, "Pump", ICONS_MOUNT_POINT"pump_64.png", 5, 0, 0};
static PowerSwitch_t InverterSwitch = {0, "Inverter", ICONS_MOUNT_POINT"inverter_64.png", 5, 0, 0};
static PowerSwitch_t HeaterSwitch = {0, "Heater", ICONS_MOUNT_POINT"heater_64.png", 5, 0, 0};
static PowerSwitch_t FridgeSwitch = {1, "Fridge", ICONS_MOUNT_POINT"fridge_64.png", -1, 0, 0};


//static PowerSwitch_t AudioSwitch = {0, "Inverter", &icon_audio_64, 1, 0, 0};



//PowerSwitch_t

lv_subject_t temp_out_subject, temp_in_subject, water_tank_lvl_subject, battery_lvl_subject;
lv_subject_t disp_brightness_subject;



static void init_subjects(){
    lv_subject_init_int(&temp_in_subject, CamperState.temp_inside);
    lv_subject_init_int(&temp_out_subject, CamperState.temp_outside);
    lv_subject_init_int(&water_tank_lvl_subject, CamperState.water_tank_lvl);
    lv_subject_init_int(&battery_lvl_subject, CamperState.house_battery_lvl);

    lv_subject_init_int(&disp_brightness_subject, 600);
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*COMMON_H*/
