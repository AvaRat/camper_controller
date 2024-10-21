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
#include "widgets/water_tank.h"
#include "widgets/image_buttons.h"


void create_dashboard_screen(lv_obj_t *parent)
{
    lv_obj_set_style_border_width(parent, 0, 0);
    lv_obj_set_style_outline_width(parent, 0, 0);

    static int32_t main_col_dsc[] = {LV_GRID_FR(1), 1, LV_GRID_FR(1), 1, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t main_row_dsc[] = {LV_GRID_FR(1), 1, LV_GRID_FR(1), 1, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};


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
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_START, 0, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c0, r0");


    /*Cell to 1;0 and align to to the start (left) horizontally and center vertically too*/
    lv_obj_t *btn_main = lv_obj_create(grid_cont);
    lv_obj_remove_flag(btn_main, LV_OBJ_FLAG_SCROLLABLE);
  //  lv_obj_set_style_outline_width(btn_main, 0, 0);
    lv_obj_set_style_border_width(btn_main, 0, 0);
    lv_obj_set_size(btn_main, 100, 100);
    lv_obj_set_style_border_opa(btn_main, 100, 0);
    lv_obj_set_style_border_color(btn_main, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_main, 5, LV_PART_MAIN);

//    lv_obj_set_style_outline_opa(parent, 0, 0);
//    lv_obj_set_style_outline_color(parent, BUTTON_OUTLINE_COLOR, LV_PART_MAIN);
//    lv_obj_set_style_outline_width(parent, 0, LV_PART_MAIN);

    create_image_button(btn_main);
        lv_obj_set_grid_cell(btn_main, LV_GRID_ALIGN_CENTER, 2, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);

    //label = lv_label_create(obj);
    //lv_label_set_text(label, "c1, r0");
    //lv_obj_align_to(label, obj, LV_ALIGN_OUT_BOTTOM_MID, 0,0 );

    /*Cell to 2;0 and align to to the start (left) horizontally and end (bottom) vertically too*/
    obj = lv_obj_create(grid_cont);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 4, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c2, r0");


    /*Cell to 2;0 and align to to the start (left) horizontally and end (bottom) vertically too*/
    obj = lv_obj_create(grid_cont);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 4, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "T_in");

    /*Cell to 2;0 and align to to the start (left) horizontally and end (bottom) vertically too*/
    obj = lv_obj_create(grid_cont);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 4, 1,
                         LV_GRID_ALIGN_STRETCH, 4, 1);
    label = lv_label_create(obj);
    lv_label_set_text(label, "T_out");


    /*Cell to 1;1 but 2 column wide (span = 2).Set width and height to stretched.*/
    lv_obj_t * water_tank_cell = lv_obj_create(grid_cont);
    lv_obj_remove_flag(water_tank_cell, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_grid_cell(water_tank_cell, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 3);
    water_tank_create(water_tank_cell);


    /*Cell to 0;1 but 2 rows tall (span = 2).Set width and height to stretched.*/
    obj = lv_obj_create(grid_cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 3);
    label = lv_label_create(obj);
    lv_label_set_text(label, "c0,r1-2");
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

}




#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DASHBOARD_SCREEN*/
