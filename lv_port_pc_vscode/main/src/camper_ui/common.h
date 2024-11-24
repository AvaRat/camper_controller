
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


static lv_style_t camper_style_title;
static const lv_font_t * font_large;
static const lv_font_t * font_normal;

static const lv_color_t BUTTON_OUTLINE_COLOR = lv_color_black();


struct CamperState_t{
    float house_battery_lvl;
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
};

#define SWITCH_MAX_TEXT_LEN 15

struct PowerSwitch_t{
    bool state;
    char btn_text[SWITCH_MAX_TEXT_LEN];
    const lv_image_dsc_t *btn_image;
    int32_t max_current;
    bool pwm_enabled;
    int32_t pwm_percent;
};


CamperState_t CamperState = {
    100, 12, 10, //house batt
    12.5, 0,    //start batt
    33, 5,      //pv
    0, 0,       //alternator
    13.1, 5,     //dc loads
    11.5, 10,    //inverter
    0, 0,  //grid
    50, 17, 10 //water, T_in, T_out
};

LV_IMAGE_DECLARE(pwr_icon_64);
LV_IMAGE_DECLARE(icon_top_lights_64);
LV_IMAGE_DECLARE(icon_audio_64);
LV_IMAGE_DECLARE(icon_pump_64);
LV_IMAGE_DECLARE(icon_ac_power_64);
LV_IMAGE_DECLARE(temp_outside_campervan_64);
LV_IMAGE_DECLARE(temp_inside_campervan_64);
LV_IMAGE_DECLARE(heater_64);
LV_IMAGE_DECLARE(fridge_64);

static PowerSwitch_t MainSwitch = {1, "Mains", &pwr_icon_64, 20, 0, 0};
static PowerSwitch_t LightSwitch = {1, "Lights", &icon_top_lights_64, 5, 1, 100};
static PowerSwitch_t WaterPumpSwitch = {0, "Pump", &icon_pump_64, 5, 0, 0};
static PowerSwitch_t InverterSwitch = {0, "Inverter", &icon_ac_power_64, 5, 0, 0};
static PowerSwitch_t HeaterSwitch = {0, "Heater", &heater_64, 5, 0, 0};
static PowerSwitch_t FridgeSwitch = {1, "Fridge", &fridge_64, 5, 0, 0};


//static PowerSwitch_t AudioSwitch = {0, "Inverter", &icon_audio_64, 1, 0, 0};



//PowerSwitch_t

static lv_subject_t temp_out_subject, temp_in_subject, water_tank_lvl_subject;


void init_subjects(){
    lv_subject_init_int(&temp_in_subject, CamperState.temp_inside);
    lv_subject_init_int(&temp_out_subject, CamperState.temp_outside);
    lv_subject_init_int(&water_tank_lvl_subject, CamperState.water_tank_lvl);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*COMMON_H*/
