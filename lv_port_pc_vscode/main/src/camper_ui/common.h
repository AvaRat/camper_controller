
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
    float temp_inside;
    float temp_outsise;
};


CamperState_t CamperState;



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*COMMON_H*/
