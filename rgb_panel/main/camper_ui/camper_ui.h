/**
 * @file camper_ui.h
 *
 */

#ifndef CAMPER_UI_H
#define CAMPER_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <esp_log.h>

#include "lvgl.h"

// #include "lvgl/demos/widgets/lv_demo_widgets.h"
// #include "electric_screen.h"
#include "dashboard_screen.h"
#include "common.h"
#include "ui_tests.h"
    // #include "animation_custom_tryouts.h"

    static lv_obj_t *tabview;

    static lv_obj_t *screensaver_img;
    static lv_style_t style_icon;


    extern lv_subject_t disp_brightness;
    extern lv_subject_t water_tank_lvl_subject;


static void anim_display_brightness(void * var, int32_t v)
{
    lv_subject_set_int(var, v);
}

    static void inactivity_anim_create()
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, &disp_brightness_subject);
        if(screensaver_active)
            lv_anim_set_values(&a, lv_subject_get_int(&disp_brightness_subject), SCREENSAVER_BRIGHTNESS);
        else
            lv_anim_set_values(&a, SCREENSAVER_BRIGHTNESS, old_brightness);
        lv_anim_set_duration(&a, 500);
        lv_anim_set_exec_cb(&a, anim_display_brightness);
        lv_anim_set_path_cb(&a, lv_anim_path_linear);
        lv_anim_start(&a);
    }

    void my_timer(lv_timer_t *timer)
    {
        lv_obj_t *disp = timer->user_data;
        int inactive_time;
        inactive_time = (int) lv_display_get_inactive_time(NULL);
     //   ESP_LOGI("CAMPER_UI", "incative time: %d", inactive_time);
        
        if(inactive_time >= SCREENSAVER_TIMEOUT && (screensaver_active == 0))
        {
                ESP_LOGW("CAMPER_UI", "INACTIVITY DETECTED Showing backgound img");
                screensaver_active = 1;
                old_brightness = lv_subject_get_int(&disp_brightness_subject);
                inactivity_anim_create();
                lv_screen_load_anim(screensaver_img, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, false);
                return;
        }
        
        if(inactive_time < SCREENSAVER_TIMEOUT && screensaver_active)
        {
            //lv_obj_set_style_opa(disp, LV_OPA_100, LV_PART_MAIN);
            lv_screen_load_anim(disp, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, false);
            //lv_screen_load(disp);
            screensaver_active = 0;
            inactivity_anim_create();
        }
    }

    static void inactivity_handler_create(lv_obj_t *disp)
    {
    //    lv_obj_set_style_bg_image_src(disp, "A:/spiffs/ducato_genai.png", LV_PART_MAIN);
        //lv_obj_set_style_bg_img_src(disp, );

        screensaver_img = lv_image_create(NULL);
        lv_img_set_src(screensaver_img, SCREENSAVER_IMG_MOUNT_POINT);

       lv_timer_t *timer = lv_timer_create(my_timer, 500, disp);
       lv_timer_set_repeat_count(timer, -1); //run indefinately
        
    }

    static void screen_brightness_btn_cb(lv_event_t *e)
    {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED)
        {
            lv_obj_t *mbox = lv_msgbox_create(NULL);
            lv_msgbox_add_title(mbox, "Display Brightness");
            // lv_msgbox_add_text(mbox1, "This is a message box with two buttons.");
            lv_msgbox_add_close_button(mbox);

            lv_obj_t *msg_cont = lv_obj_create(mbox);
            lv_obj_set_size(msg_cont, lv_pct(100), 150);
            lv_obj_set_style_border_opa(msg_cont, LV_OPA_0, LV_PART_MAIN);
            lv_obj_center(msg_cont);
            lv_obj_t *label = lv_label_create(msg_cont);
            lv_label_set_text(label, "Set brightness of the display");
            /*Create a slider in the center of the display*/
            lv_obj_t *slider = lv_slider_create(msg_cont);
            // lv_slider_set_value(slider, disp_brightness.value, LV_ANIM_OFF);

            lv_obj_set_size(slider, 200, 20);
            lv_slider_set_range(slider, 1, 1020);
            lv_obj_center(slider);

            lv_slider_bind_value(slider, &disp_brightness_subject);

            /*Create a label below the slider*/
            //  brightness_slider_label = lv_label_create(msg_cont);

            // lv_label_set_text_fmt(brightness_slider_label, "%d pc", pwr_switch->pwm_percent);

            // lv_obj_align_to(slider, mbox, LV_ALIGN_CENTER, 0, 0);
            // lv_obj_align_to(brightness_slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        }
    }

    static void brightness_changer_create(lv_obj_t *parent)
    {

        lv_obj_t *btn = lv_button_create(parent);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_CHECKED);
        lv_obj_set_style_pad_all(btn, 10, 0);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
        lv_obj_add_event_cb(btn, screen_brightness_btn_cb, LV_EVENT_ALL, NULL);
        lv_obj_set_style_shadow_width(btn, 0, 0);
        // TODO Screen Brightness Icon or IMG
        lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_EYE_OPEN, 0);

        lv_obj_set_size(btn, LV_DPX(60), LV_DPX(60));
        lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -LV_DPX(15), -LV_DPX(15));
    }

    static void camper_ui_create()
    {
        init_subjects();

        init_observers();

        font_large = &lv_font_montserrat_24;
        font_normal = &lv_font_montserrat_18;
        lv_style_init(&camper_style_title);
        lv_style_set_text_font(&camper_style_title, font_large);

#if LV_USE_THEME_DEFAULT
        lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                              font_normal);
#endif

        lv_style_init(&style_icon);
        lv_style_set_text_color(&style_icon, lv_theme_get_color_primary(NULL));
        lv_style_set_text_font(&style_icon, font_large);

        /*Create a Tab view object*/
        // tabview = lv_tabview_create(lv_screen_active());
        // lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
        // lv_tabview_set_tab_bar_size(tabview, 60);

        // lv_obj_t * t1 = lv_tabview_add_tab(tabview, "Main");
        // lv_obj_t * t2 = lv_tabview_add_tab(tabview, "Electric");
        // lv_obj_t * t3 = lv_tabview_add_tab(tabview, "Img");
        // lv_obj_t * t4 = lv_tabview_add_tab(tabview, "4");

        lv_obj_t *disp = lv_screen_active();

        create_dashboard_screen(disp);
        //  create_camper_genai_screen(t3);

        //  create_electric_screen(t2);

        //  lv_demo_widgets(t4);

        // test_animation(t2);
        brightness_changer_create(disp);
        
        inactivity_handler_create(disp);
    }

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*CAMPER_UI_H*/
