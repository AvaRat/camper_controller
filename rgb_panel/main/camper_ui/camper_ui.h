/**
 * @file camper_ui.h
 *
 */

#ifndef CAMPER_UI_H
#define CAMPER_UI_H



#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"

//#include "lvgl/demos/widgets/lv_demo_widgets.h"
//#include "electric_screen.h"
#include "dashboard_screen.h"
#include "common.h"
#include "tests.h"
//#include "animation_custom_tryouts.h"


static lv_obj_t * tabview;



static void camper_ui_create()
{

    init_subjects();
    font_large = &lv_font_montserrat_22;
    font_normal = LV_FONT_DEFAULT;
    lv_style_init(&camper_style_title);
    lv_style_set_text_font(&camper_style_title, font_large);
#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);
#endif

    /*Create a Tab view object*/
    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview, 60);

    lv_obj_t * t1 = lv_tabview_add_tab(tabview, "Main");
    lv_obj_t * t2 = lv_tabview_add_tab(tabview, "Electric");
    lv_obj_t * t3 = lv_tabview_add_tab(tabview, "Img");
    lv_obj_t * t4 = lv_tabview_add_tab(tabview, "4");


    create_dashboard_screen(t1);
    create_camper_genai_screen(t3);
  //  create_electric_screen(t2);

  //  lv_demo_widgets(t4);

    //test_animation(t2);
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*CAMPER_UI_H*/
