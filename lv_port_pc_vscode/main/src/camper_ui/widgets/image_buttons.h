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


static void create_image_button(lv_obj_t *parent)
{
    LV_IMAGE_DECLARE(pwr_icon_64);

    /*Create a transition animation on width transformation and recolor.*/
    static lv_style_prop_t tr_prop[] = {LV_STYLE_IMAGE_RECOLOR, LV_STYLE_IMAGE_RECOLOR_OPA, LV_STYLE_BG_IMAGE_RECOLOR, LV_STYLE_BG_IMAGE_RECOLOR_OPA, 0};
    static lv_style_transition_dsc_t tr;
    lv_style_transition_dsc_init(&tr, tr_prop, lv_anim_path_linear, 20, 0, NULL);


//normal style when NOT pressed
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &tr);

   // Darken the button when pressed and make it wider
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
   // lv_style_set_img_recolor(&style_pr, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_image_recolor_opa(&style_pr, LV_OPA_80);
    lv_style_set_image_recolor(&style_pr, lv_palette_main(LV_PALETTE_RED));

    lv_style_set_bg_image_recolor(&style_pr, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_image_recolor_opa(&style_pr, 20);
   // lv_style_set_img_recolor(&style_pr, );


    /*Create an image button*/
    lv_obj_t * imagebutton1 = lv_imagebutton_create(parent);
    lv_obj_set_size(imagebutton1, 64, 64);

    lv_imagebutton_set_src(imagebutton1, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &pwr_icon_64,
                           NULL);
    lv_obj_add_style(imagebutton1, &style_def, 0);
    lv_obj_add_style(imagebutton1, &style_pr, LV_STATE_PRESSED);



    lv_obj_align(imagebutton1, LV_ALIGN_TOP_MID, 0, 0);
    /*Create a label on the image button*/
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Mains");
    lv_obj_align_to(label, imagebutton1, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);

    lv_obj_set_style_pad_all(imagebutton1, 5, LV_PART_MAIN);

    lv_obj_align(imagebutton1, LV_ALIGN_CENTER, 0,0 );

}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*IMAGE_BUTTONS_H*/
