
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

//static lv_color_t BUTTON_OUTLINE_COLOR = lv_color_black();


typedef struct camperstate_t{
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
}CamperState_t;

#define SWITCH_MAX_TEXT_LEN 15

typedef struct powerswitch_t{
    bool state;
    char btn_text[SWITCH_MAX_TEXT_LEN];
    const lv_image_dsc_t *btn_image;
    int max_current;
    bool pwm_enabled;
    int pwm_percent;
}PowerSwitch_t;


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
static lv_subject_t disp_brightness;


/*
ESP hardware stuff
*/
static void screen_brightness_observer_cb(lv_observer_t *observer, lv_subject_t *subject){
    int brightness = lv_subject_get_int(subject);
    //ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, brightness, 10);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    //ledc_set_duty_and_update(LEDC_MODE, LEDC_CHANNEL, brightness, 1023);
}


void init_subjects(){
    lv_subject_init_int(&temp_in_subject, CamperState.temp_inside);
    lv_subject_init_int(&temp_out_subject, CamperState.temp_outside);
    lv_subject_init_int(&water_tank_lvl_subject, CamperState.water_tank_lvl);
    lv_subject_init_int(&disp_brightness, 600);
    lv_subject_add_observer(&disp_brightness, screen_brightness_observer_cb, NULL);


}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*COMMON_H*/
