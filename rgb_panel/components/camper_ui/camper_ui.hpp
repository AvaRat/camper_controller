/**
 * @file camper_ui.h
 *
 */

#ifndef CAMPER_UI_H
#define CAMPER_UI_H


#include <vector>
#include <string>



void create_button(std::string, lv_obj_t*);

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"

#include "lvgl/demos/widgets/lv_demo_widgets.h"
#include "electric_screen.h"
#include "dashboard_screen.h"
#include "common.h"

#include "animation_custom_tryouts.h"


static lv_obj_t * tabview;



static void camper_ui_create()
{

    init_subjects();
    font_large = &lv_font_montserrat_28;
    font_normal = &lv_font_montserrat_18;
    lv_style_init(&camper_style_title);
    lv_style_set_text_font(&camper_style_title, font_large);
#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);
#endif

    /*Create a Tab view object*/
    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview, 120);

    lv_obj_t * t1 = lv_tabview_add_tab(tabview, "Main");
    lv_obj_t * t3 = lv_tabview_add_tab(tabview, "Electric");
    lv_obj_t * t2 = lv_tabview_add_tab(tabview, "2");
    lv_obj_t * t4 = lv_tabview_add_tab(tabview, "4");


    create_dashboard_screen(t1);
    create_electric_screen(t3);
    test_animation(t2);
    LV_IMG_DECLARE(genai_camper_ducato);
    lv_obj_t *img = lv_image_create(t4);
    lv_image_set_src(img, &genai_camper_ducato);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*CAMPER_UI_H*/
