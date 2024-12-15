/**
 * @file camper_ui.h
 *
 */

#ifndef ELECTRIC_SCREEN_H
#define ELECTRIC_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"

#include "common.h"


#define MAX_CURRENT 50
#define CANVAS_WIDTH  200
#define CANVAS_HEIGHT  120



/**
 * A custom cubic bezier animation path, need to specify cubic-parameters in a->parameter.bezier3
 * @param a     pointer to an animation
 * @return      the current value to set
 */
//int32_t lv_anim_path_custom_bezier3(const lv_anim_t * a);

static lv_obj_t * pv_slider_label;
static lv_obj_t * grid_slider_label;
static lv_obj_t * alt_slider_label;

typedef struct slideruserdata{
    lv_obj_t *slider_label;
    lv_obj_t *parent;
    float *value;
    char label_text[8];
}slider_user_data_t;

slider_user_data_t pv_data;
slider_user_data_t grid_data;
slider_user_data_t alt_data;

static void custom_electric_animation(lv_obj_t *parent);

static void slider_event_cb(lv_event_t * e)
{
    slider_user_data_t *u_data = (slider_user_data_t *) (e->user_data);
    lv_obj_t * slider = (lv_obj_t *)lv_event_get_target(e);

    char buf[10];
    float value = (float)lv_slider_get_value(slider);
    lv_snprintf(buf, 10, "%s: %dA", u_data->label_text, (int)value);

    *(u_data->value) = value;

    lv_label_set_text(u_data->slider_label, buf);
    lv_obj_align_to(u_data->slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    custom_electric_animation(u_data->parent);
}

//use slider to test electric visualizations
static void test_electric_control(lv_obj_t *parent, lv_obj_t *canva_parent)
{
    //PV current
    lv_obj_t * slider_pv = lv_slider_create(parent);
    lv_obj_set_width(slider_pv, 100);
    lv_obj_set_height(slider_pv, 8);
    lv_obj_align(slider_pv, LV_ALIGN_TOP_MID, 0, 0);
    lv_slider_set_range(slider_pv, 0, MAX_CURRENT);
    lv_obj_set_style_anim_duration(slider_pv, 2000, 0);
    /*Create a label below the slider*/
    pv_slider_label = lv_label_create(parent);
    lv_label_set_text(pv_slider_label, "PV: 0A");
    lv_obj_align_to(pv_slider_label, slider_pv, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    pv_data.slider_label = pv_slider_label;
    pv_data.value = &(CamperState.pv_current);
    pv_data.parent = canva_parent;
    lv_strncpy(pv_data.label_text, "PV:", 3);
    lv_obj_add_event_cb(slider_pv, slider_event_cb, LV_EVENT_VALUE_CHANGED, &pv_data);

    //Alternator current
    lv_obj_t * slider_alt = lv_slider_create(parent);
    lv_obj_set_width(slider_alt, 100);
    lv_obj_set_height(slider_alt, 8);
    lv_obj_align(slider_alt, LV_ALIGN_CENTER, 0, -10);
    lv_slider_set_range(slider_alt, 0, MAX_CURRENT);

    lv_obj_set_style_anim_duration(slider_alt, 2000, 0);
    /*Create a label below the slider*/
    alt_slider_label = lv_label_create(parent);
    lv_label_set_text(alt_slider_label, "Alt: 0A");
    lv_obj_align_to(alt_slider_label, slider_alt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    alt_data.slider_label = alt_slider_label;
    alt_data.value = &(CamperState.alternator_current);
    alt_data.parent = canva_parent;
    lv_strncpy(alt_data.label_text, "Alt:", 5);

    lv_obj_add_event_cb(slider_alt, slider_event_cb, LV_EVENT_VALUE_CHANGED, &alt_data);

    //Grid current
    lv_obj_t * slider_grid = lv_slider_create(parent);
    lv_obj_set_width(slider_grid, 100);
    lv_obj_set_height(slider_grid, 8);
    lv_obj_align(slider_grid, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_slider_set_range(slider_grid, 0, MAX_CURRENT);

    lv_obj_set_style_anim_duration(slider_grid, 2000, 0);
    /*Create a label below the slider*/
    grid_slider_label = lv_label_create(parent);

    lv_label_set_text(grid_slider_label, "Grid: 0A");
    lv_obj_align_to(grid_slider_label, slider_grid, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    grid_data.slider_label = grid_slider_label;
    grid_data.value = &(CamperState.grid_current);
    grid_data.parent = canva_parent;
    lv_strncpy(grid_data.label_text, "Grid:", 5);

    lv_obj_add_event_cb(slider_grid, slider_event_cb, LV_EVENT_VALUE_CHANGED, &grid_data);

}


static lv_draw_buf_t draw_buf;

static void electric_update()
{

}

//Animation with a Canvas
static void custom_electric_animation(lv_obj_t *parent)
{
    lv_obj_update_layout(parent);
    int anim_con_width = lv_obj_get_width(parent);
    int anim_con_height = lv_obj_get_height(parent);

    int32_t x_mid = anim_con_width/2;
    int32_t y_mid = anim_con_height/2;

    // those parameters can change online
    int32_t left_height = CamperState.grid_current;
    int32_t right_height = CamperState.alternator_current;
    int32_t top_width = CamperState.pv_current;
    int32_t bottom_width = left_height+right_height+top_width;

    int32_t x2_size = 30;
    int32_t y2_size = 30;

    int32_t x3_size = 30;
    int32_t y3_size = 30;
    char label_text_buf[5];


    printf("size: %d, %d\n", anim_con_width, anim_con_height);

    /*Create a buffer for the canvas*/
    LV_DRAW_BUF_DEFINE(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
    /*Create a canvas and initialize its palette*/
    lv_obj_t * canvas = lv_canvas_create(parent);
    lv_canvas_set_draw_buf(canvas, &draw_buf);
    lv_obj_center(canvas);

    //structs for layer
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    //structs for label
    lv_draw_label_dsc_t label_dsc;
 //   lv_area_t coords = {x_mid - bottom_width -5, y_mid - left_height/2, x_mid, y_mid};

    /*Grey background (There is no dedicated alpha channel in indexed images so LV_OPA_COVER is ignored)*/
    lv_canvas_fill_bg(canvas, lv_palette_main(LV_PALETTE_GREY), LV_OPA_0);



    lv_draw_line_dsc_t lines_dsc;
    lv_draw_line_dsc_init(&lines_dsc);
    lines_dsc.color = lv_palette_main(LV_PALETTE_LIGHT_GREEN);
    lines_dsc.opa = LV_OPA_100;

//bottom
    lines_dsc.width = bottom_width;
    lines_dsc.round_end = 0;
    lines_dsc.round_start = 0;

    lines_dsc.p1.x = x_mid;
    lines_dsc.p1.y = left_height>right_height ? (y_mid-left_height/2) : y_mid -(right_height/2);

    lines_dsc.p2.x = x_mid;
    lines_dsc.p2.y = anim_con_height;
    lv_draw_line(&layer, &lines_dsc);
//top
    lines_dsc.width = top_width;
    lines_dsc.round_end = 0;
    lines_dsc.round_start = 0;
    lines_dsc.p1.x = x_mid;
    lines_dsc.p1.y = 0;
    lines_dsc.p2.x = x_mid;
    lines_dsc.p2.y = y_mid;
    lv_draw_line(&layer, &lines_dsc);
//right label
    lv_draw_label_dsc_init(&label_dsc);
    //dsc.color = lv_palette_main(LV_PALE);
    label_dsc.font = LV_FONT_DEFAULT;
    label_dsc.decor = LV_TEXT_DECOR_NONE;
    label_dsc.text = "6A";
  //  coords = {x_mid - top_width/2+10, y_mid - 60, x_mid+25, y_mid};
 //   lv_draw_label(&layer, &label_dsc, &coords);


//left
    lines_dsc.width = left_height;
    lines_dsc.round_end = 0;
    lines_dsc.round_start = 0;
    lines_dsc.p1.x = 0;
    lines_dsc.p1.y = y_mid;
    lines_dsc.p2.x = x_mid;
    lines_dsc.p2.y = y_mid;
    lv_draw_line(&layer, &lines_dsc);
//left label
    lv_draw_label_dsc_init(&label_dsc);
    //dsc.color = lv_palette_main(LV_PALE);
    label_dsc.font = LV_FONT_DEFAULT;
    label_dsc.decor = LV_TEXT_DECOR_NONE;

//    coords = {x_mid - bottom_width -5, y_mid - left_height/2, x_mid, y_mid};
//    lv_draw_label(&layer, &label_dsc, &coords);


//right
    lines_dsc.width = right_height;
    lines_dsc.round_end = 0;
    lines_dsc.round_start = 0;
    lines_dsc.p1.x = anim_con_width;
    lines_dsc.p1.y = y_mid;
    lines_dsc.p2.x = x_mid;
    lines_dsc.p2.y = y_mid;
    lv_draw_line(&layer, &lines_dsc);

//right label
    lv_draw_label_dsc_init(&label_dsc);
    //dsc.color = lv_palette_main(LV_PALE);
    label_dsc.font = LV_FONT_DEFAULT;
    label_dsc.decor = LV_TEXT_DECOR_NONE;
    label_dsc.text = "10A";
 //   coords = {x_mid + bottom_width + 5, y_mid - left_height/2, anim_con_width, anim_con_height};
//    lv_draw_label(&layer, &label_dsc, &coords);

    lv_canvas_finish_layer(canvas, &layer);
   // lv_canvas_finish_layer(canvas, &layer_label);

}


void create_electric_screen(lv_obj_t *parent){

    /*Init the style for the default state*/
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, 3);

    lv_style_set_bg_opa(&style, LV_OPA_0);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&style, LV_OPA_100);
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_color_black());


    lv_style_set_outline_opa(&style, LV_OPA_COVER);
    lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_text_color(&style, lv_color_black());
    lv_style_set_pad_all(&style, 10);


    static int32_t grid_elec_col_dsc[] = {LV_GRID_FR(1), 50, LV_GRID_FR(1), 50, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t grid_elec_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};




    LV_IMAGE_DECLARE(battery_64);
    lv_obj_t * battery_icon = lv_image_create(parent);
    lv_image_set_src(battery_icon, &battery_64);

    LV_IMAGE_DECLARE(solar_panel_64);
    lv_obj_t * pv_icon = lv_image_create(parent);
    lv_image_set_src(pv_icon, &solar_panel_64);

    LV_IMAGE_DECLARE(inverter_64);
    lv_obj_t * inverter_icon = lv_image_create(parent);
    lv_image_set_src(inverter_icon, &inverter_64);

    LV_IMAGE_DECLARE(alternator_64);
    lv_obj_t * alternator_icon = lv_image_create(parent);
    lv_image_set_src(alternator_icon, &alternator_64);

    LV_IMAGE_DECLARE(grid_64);
    lv_obj_t * grid_icon = lv_image_create(parent);
    lv_image_set_src(grid_icon, &grid_64);
    //lv_obj_set_fit(grid_icon, true, true);


    lv_obj_t * dc_main_icon = lv_button_create(parent);
    lv_obj_t * dc_main_label= lv_label_create(dc_main_icon);
    lv_label_set_text(dc_main_label, "DC Mains");
    lv_obj_remove_style_all(dc_main_icon);
    lv_obj_add_style(dc_main_icon, &style, 0);
    lv_obj_remove_flag(dc_main_icon, LV_OBJ_FLAG_CLICKABLE);

//    lv_obj_t *dc_main_icon = lv_label_create(parent);
//    lv_label_set_text(dc_main_icon, "DC Mains");





    lv_obj_set_grid_dsc_array(parent, grid_elec_col_dsc, grid_elec_row_dsc);

    lv_obj_set_grid_cell(battery_icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 2, 1); //middle
    lv_obj_set_grid_cell(pv_icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_START, 0, 1); //top middle
    lv_obj_set_grid_cell(grid_icon, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1); //middle lines_dsc
    lv_obj_set_grid_cell(alternator_icon, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 1, 1); //right
    lv_obj_set_grid_cell(inverter_icon, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 2, 1); //lines_dsc bottom
    lv_obj_set_grid_cell(dc_main_icon, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 2, 1); //right bottom


    lv_obj_t *anim_container = lv_obj_create(parent);
    lv_style_set_border_opa(&style, LV_OPA_0);
    lv_obj_add_style(anim_container, &style, 0);
    lv_obj_set_size(anim_container, CANVAS_WIDTH, CANVAS_HEIGHT);


    lv_obj_set_grid_cell(anim_container, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 1, 1); //middle

    lv_obj_remove_flag(anim_container, LV_OBJ_FLAG_SCROLLABLE);


    lv_obj_t *slider_cont = lv_obj_create(parent);
    lv_style_set_border_opa(&style, LV_OPA_50);
    lv_obj_add_style(slider_cont, &style, 0);
    //v_obj_set_size(slider_cont, lv_pct(20), lv_pct(20));

    lv_obj_set_grid_cell(slider_cont, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1); //right top
    test_electric_control(slider_cont, anim_container);
    lv_obj_remove_flag(slider_cont, LV_OBJ_FLAG_SCROLLABLE);


    //electric_animation(anim_container);
    custom_electric_animation(anim_container);
}






#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ELECTRIC_SCREEN_H*/
