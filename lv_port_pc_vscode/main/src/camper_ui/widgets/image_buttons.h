/**
 * @file image_buttons.h
 *
 */

#ifndef IMAGE_BUTTONS_H
#define IMAGE_BUTTONS_H


#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


/*
TODO

Each load details on long press:
    -Current consumption.
    -Settings:
        -max current before warning
        -on/off on default turnOn


*/

static lv_obj_t * brightness_slider_label;

void brightness_slider_cb(lv_event_t *e)
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

static void create_lights_brightness_popup(PowerSwitch_t *pwr_switch){

    lv_obj_t * mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "Brightness");
   // lv_msgbox_add_text(mbox1, "This is a message box with two buttons.");
    lv_msgbox_add_close_button(mbox);

    lv_obj_t *msg_cont = lv_obj_create(mbox);
    lv_obj_set_size(msg_cont, lv_pct(100), 150);
    lv_obj_set_style_border_opa(msg_cont, LV_OPA_0, LV_PART_MAIN);
    lv_obj_center(msg_cont);
    lv_obj_t *label = lv_label_create(msg_cont);
    lv_label_set_text(label, "Set brightness of the LEDs");
    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(msg_cont);
    lv_slider_set_value(slider, pwr_switch->pwm_percent, LV_ANIM_OFF);
    lv_obj_set_size(slider, 200, 20);
    lv_obj_center(slider);
    lv_obj_set_user_data(slider, pwr_switch);
    lv_obj_add_event_cb(slider, brightness_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*Create a label below the slider*/
    brightness_slider_label = lv_label_create(msg_cont);

    lv_label_set_text_fmt(brightness_slider_label, "%d %", pwr_switch->pwm_percent);

   // lv_obj_align_to(slider, mbox, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align_to(brightness_slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_LONG_PRESSED) {
       // LV_LOG_USER("Toggled");
        lv_obj_t *obj = lv_event_get_target_obj(e);
        char txt[50];

        bool btn_switched_on = lv_obj_has_state(obj, LV_STATE_CHECKED);
        PowerSwitch_t *pwr_sw = (PowerSwitch_t *) lv_obj_get_user_data(obj);
    //    pwr_sw->state = !pwr_sw->state;

        snprintf(txt, 50, "LONG PRESS \n");
        LV_LOG_USER(txt);

        // popup PWM slider if enabled
        if(pwr_sw->pwm_enabled)
            create_lights_brightness_popup(pwr_sw);


    }else if(code == LV_EVENT_SHORT_CLICKED) {
       // LV_LOG_USER("Toggled");
        lv_obj_t *obj = lv_event_get_target_obj(e);
        char txt[50];

        bool btn_switched_on = lv_obj_has_state(obj, LV_STATE_CHECKED);
        PowerSwitch_t *pwr_sw = (PowerSwitch_t *) lv_obj_get_user_data(obj);
        pwr_sw->state = !pwr_sw->state;

        lv_obj_set_state(obj, LV_STATE_USER_1, pwr_sw->state);

        snprintf(txt, 50, "%s state: %s\n PowerSwitch->state %d", (char*) (pwr_sw->btn_text), btn_switched_on ? "On" : "Off", pwr_sw->state);
        LV_LOG_USER(txt);
    }

}

static void create_icon_button(lv_obj_t *parent, lv_obj_t *icon_obj, PowerSwitch_t *power_switch)
{
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_border_opa(&style_pr, LV_OPA_100);
    lv_style_set_bg_opa(&style_pr, LV_OPA_20);

    icon_obj = lv_image_create(parent);
    lv_image_set_src(icon_obj, power_switch->btn_image);

    //setting user data
    lv_obj_set_user_data(parent, power_switch);

    lv_obj_add_event_cb(parent, event_handler, LV_EVENT_ALL, NULL);

    //lv_obj_add_flag(parent, LV_OBJ_FLAG_CHECKABLE);
    //lv_obj_add_style(parent, &style_pr, LV_STATE_PRESSED);
    lv_style_set_border_color(&style_pr, lv_palette_main(LV_PALETTE_GREEN));
    lv_obj_add_style(parent, &style_pr, LV_STATE_USER_1);


//    lv_obj_set_style_pad_all(btn_obj, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(parent, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(parent, LV_OPA_10, LV_PART_MAIN);
    lv_obj_set_style_border_opa(parent, 100, 0);
    lv_obj_set_style_border_color(parent, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_style_border_width(parent, 5, LV_PART_MAIN);


    lv_obj_t *btn_label = lv_label_create(parent);
    lv_label_set_text(btn_label, power_switch->btn_text);

    lv_obj_align(icon_obj, LV_ALIGN_CENTER, 0, -10);
    lv_obj_align(btn_label, LV_ALIGN_BOTTOM_MID, 0, 15);

    // properly initialize button with a ON/OFF state
    lv_obj_set_state(parent, LV_STATE_USER_1, power_switch->state);
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*IMAGE_BUTTONS_H*/
