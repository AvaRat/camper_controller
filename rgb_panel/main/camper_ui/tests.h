

#include "lvgl.h"
#include <stdio.h>

extern lv_subject_t disp_brightness;

void display_brightness_slider_cb(lv_event_t *e)
{

    lv_obj_t * slider =  (lv_obj_t*) lv_event_get_target(e);
    PowerSwitch_t *pwr_switch = (PowerSwitch_t*) lv_obj_get_user_data(slider);
    int32_t new_value = (int32_t)lv_slider_get_value(slider);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", new_value);
    lv_label_set_text(brightness_slider_label, buf);
    lv_obj_align_to(brightness_slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    pwr_switch->pwm_percent = new_value;

}

static void screen_brightness_btn_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * mbox = lv_msgbox_create(NULL);
        lv_msgbox_add_title(mbox, "Display Brightness");
    // lv_msgbox_add_text(mbox1, "This is a message box with two buttons.");
        lv_msgbox_add_close_button(mbox);

        lv_obj_t *msg_cont = lv_obj_create(mbox);
        lv_obj_set_size(msg_cont, lv_pct(80), 150);
        lv_obj_set_style_border_opa(msg_cont, LV_OPA_0, LV_PART_MAIN);
        lv_obj_center(msg_cont);
        lv_obj_t *label = lv_label_create(msg_cont);
        lv_label_set_text(label, "Set brightness of the display");
        /*Create a slider in the center of the display*/
        lv_obj_t * slider = lv_slider_create(msg_cont);
        //lv_slider_set_value(slider, disp_brightness.value, LV_ANIM_OFF);


        lv_obj_set_size(slider, 200, 20);
        lv_slider_set_range(slider, 0, 1020);
        lv_obj_center(slider);
        //lv_obj_add_event_cb(slider, display_brightness_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);
        
        lv_slider_bind_value(slider, &disp_brightness);

        /*Create a label below the slider*/
      //  brightness_slider_label = lv_label_create(msg_cont);

       // lv_label_set_text_fmt(brightness_slider_label, "%d pc", pwr_switch->pwm_percent);

    // lv_obj_align_to(slider, mbox, LV_ALIGN_CENTER, 0, 0);
        //lv_obj_align_to(brightness_slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }
}

void create_camper_genai_screen(lv_obj_t *parent){
    LV_IMAGE_DECLARE(img_camper_van);
    lv_obj_t *img = lv_image_create(parent);
    lv_image_set_src(img, &img_camper_van);

    lv_obj_t *btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, 60, 60);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_CHECKED);
    lv_obj_set_style_pad_all(btn, 10, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_add_event_cb(btn, screen_brightness_btn_cb, LV_EVENT_ALL, NULL);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_SETTINGS, 0);   
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
}
