
/**
 * @file water_tank.h
 *
 */

#ifndef WATER_TANK_H
#define WATER_TANK_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"

//#include "../common.h"

//TODO Water Tank details on long press like: max capacity, current water capacity (in liters), Settings (to set max capacity), water temperature, water usage history.

static lv_obj_t * bar;


static void water_tank_update()
{
    lv_bar_set_value(bar, CamperState.water_tank_lvl, LV_ANIM_ON);
}

void water_tank_indicator_create(lv_obj_t *parent)
{
//update layout first
    static lv_style_t style_bg;
    static lv_style_t style_indic;

    lv_style_init(&style_bg);
    lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
    lv_style_set_border_width(&style_bg, 2);
    lv_style_set_pad_all(&style_bg, 6); /*To make the indicator smaller*/
    lv_style_set_radius(&style_bg, 6);
    lv_style_set_anim_duration(&style_bg, 1000);

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_indic, 3);

    bar = lv_bar_create(parent);
    lv_obj_remove_style_all(bar);  /*To have a clean start*/
    lv_obj_add_style(bar, &style_bg, 0);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);

    lv_obj_set_size(bar, 100, 200);
    lv_obj_center(bar);
    lv_bar_set_value(bar, CamperState.water_tank_lvl, LV_ANIM_ON);

    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Water Tank");
    lv_obj_set_style_text_font(label, font_normal, LV_PART_MAIN);
    lv_obj_align_to(label, bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);


    LV_IMAGE_DECLARE(icon_water_64);
    lv_obj_t * water_icon = lv_image_create(parent);
    lv_image_set_src(water_icon, &icon_water_64);
    lv_obj_set_style_opa(water_icon, LV_OPA_80, LV_PART_MAIN);
    lv_obj_align_to(water_icon, bar, LV_ALIGN_CENTER, 0, 0);

}

void battery_indicator_create(lv_obj_t *parent)
{
//update layout first
    static lv_style_t style_bg;
    static lv_style_t style_indic;

    lv_style_init(&style_bg);
    lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_border_width(&style_bg, 2);
    lv_style_set_pad_all(&style_bg, 6); /*To make the indicator smaller*/
    lv_style_set_radius(&style_bg, 6);
    lv_style_set_anim_duration(&style_bg, 1000);

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_radius(&style_indic, 3);

    bar = lv_bar_create(parent);
    lv_obj_remove_style_all(bar);  /*To have a clean start*/
    lv_obj_add_style(bar, &style_bg, 0);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);

    lv_obj_set_size(bar, 100, 200);
    lv_obj_center(bar);
    lv_bar_set_value(bar, CamperState.house_battery_lvl, LV_ANIM_ON);

    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Battery");
    lv_obj_set_style_text_font(label, font_normal, LV_PART_MAIN);
    lv_obj_align_to(label, bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    LV_IMAGE_DECLARE(battery_64);
    lv_obj_t * battery_icon = lv_image_create(parent);
    lv_image_set_src(battery_icon, &battery_64);
    lv_obj_set_style_opa(battery_icon, LV_OPA_80, LV_PART_MAIN);
    lv_obj_align_to(battery_icon, bar, LV_ALIGN_CENTER, 0, 0);

}



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DASHBOARD_SCREEN*/
