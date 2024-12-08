/**
 * @file dashboard.h
 *
 */

#ifndef DASHBOARD_SCREEN_H
#define DASHBOARD_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"

#include "common.h"
#include "widgets/lvl_indicators.h"
#include "widgets/image_buttons.h"
#include "widgets/temp_indicator.h"

#define BTN_SIZE 150


void create_dashboard_screen(lv_obj_t *parent)
{
    lv_obj_set_style_border_width(parent, 0, 0);
    lv_obj_set_style_outline_width(parent, 0, 0);

    static int32_t main_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    static int32_t main_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};


    /*Create a container with grid*/
    lv_obj_t * grid_cont = lv_obj_create(parent);
    lv_obj_set_grid_dsc_array(grid_cont, main_col_dsc, main_row_dsc);
    lv_obj_set_size(grid_cont, lv_pct(100), lv_pct(100));
    lv_obj_center(grid_cont);

    lv_obj_t * label;
    lv_obj_t * obj;

    /*Cell to 0;0 and align to to the start (left/top) horizontally and vertically too*/
    obj = lv_obj_create(grid_cont);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, 150, 100);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    temp_indicator_create(obj, &temp_in_subject, &temp_inside_campervan_64);

    /*Cell to 1;0 and align to to the start (left) horizontally and center vertically too */

    obj = lv_obj_create(grid_cont);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, 150, 100);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 1, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    temp_indicator_create(obj, &temp_out_subject, &temp_outside_campervan_64);
    /*
    LV_IMAGE_DECLARE(temp_inside_campervan_64);
    img_obj = lv_image_create(obj);
    lv_image_set_src(img_obj, &temp_inside_campervan_64);

    label = lv_label_create(obj);
    lv_label_set_text(label, "20 C");
    lv_obj_set_style_pad_left(label, 10, LV_PART_MAIN);
    lv_obj_align_to(label, img_obj, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    */


    //Power Switches grid
    lv_obj_t * btn_grid_cont = lv_obj_create(grid_cont);
    lv_obj_set_grid_cell(btn_grid_cont, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 3);
    lv_obj_center(btn_grid_cont);

    static int32_t btn_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t btn_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_remove_flag(btn_grid_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(btn_grid_cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(btn_grid_cont, btn_col_dsc, btn_row_dsc);

    lv_obj_t *lights_btn = lv_obj_create(btn_grid_cont);
    lv_obj_set_size(lights_btn, BTN_SIZE, BTN_SIZE);
    lv_obj_remove_flag(lights_btn, LV_OBJ_FLAG_SCROLLABLE);
    create_icon_button(lights_btn, obj, &LightSwitch);
    lv_obj_set_grid_cell(lights_btn, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_t *pump_btn = lv_obj_create(btn_grid_cont);
    lv_obj_set_size(pump_btn, BTN_SIZE, BTN_SIZE);
    lv_obj_remove_flag(pump_btn, LV_OBJ_FLAG_SCROLLABLE);
    create_icon_button(pump_btn, obj, &WaterPumpSwitch);
    lv_obj_set_grid_cell(pump_btn, LV_GRID_ALIGN_CENTER, 1, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_t *inverter_btn = lv_obj_create(btn_grid_cont);
    lv_obj_set_size(inverter_btn, BTN_SIZE, BTN_SIZE);
    lv_obj_remove_flag(inverter_btn, LV_OBJ_FLAG_SCROLLABLE);
    create_icon_button(inverter_btn, obj, &InverterSwitch);
    lv_obj_set_grid_cell(inverter_btn, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);

    lv_obj_t *btn_main = lv_obj_create(btn_grid_cont);
    lv_obj_set_size(btn_main, BTN_SIZE, BTN_SIZE);
    lv_obj_remove_flag(btn_main, LV_OBJ_FLAG_SCROLLABLE);
    create_icon_button(btn_main, obj, &MainSwitch);
    lv_obj_set_grid_cell(btn_main, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_CENTER, 2, 1);



    /*Cell to 1;1 but 2 column wide (span = 2).Set width and height to stretched.*/
    lv_obj_t * water_tank_cell = lv_obj_create(grid_cont);
    lv_obj_remove_flag(water_tank_cell, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(water_tank_cell, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 2);
    water_tank_indicator_create(water_tank_cell);


    /*Cell to 0;1 but 2 rows tall (span = 2).Set width and height to stretched.*/
    obj = lv_obj_create(grid_cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 2);

    battery_indicator_create(obj);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

//static lv_subject_t temp_out_subject, temp_in_subject;
    /*Create a slider in the center of the display*/
    // lv_obj_t * slider = lv_slider_create(obj);
    // lv_obj_set_size(slider, 100, 20);
    // lv_obj_center(slider);

    // lv_slider_bind_value(slider, &temp_in_subject);


}




#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DASHBOARD_SCREEN*/
