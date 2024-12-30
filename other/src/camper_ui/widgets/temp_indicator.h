
/**
 * @file temp_indicator.h
 *
 */

#ifndef TEMP_INDICATOR_H
#define TEMP_INDICATOR_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"


#include "../common.h"



/* TODO Temp details on LongPress:
    -temp history
    -max temp this trip
    -min temp this trip
    -humidity in & out
*/

void temp_indicator_create(lv_obj_t *parent,  lv_subject_t *int_subject, const lv_image_dsc_t *icon)
{
 //   lv_subject_init_int(&temp_in_subject, 0);

    lv_obj_t * img_obj;
    lv_obj_t *label;

    img_obj = lv_image_create(parent);
    lv_image_set_src(img_obj, icon);
    lv_obj_align(img_obj, LV_ALIGN_LEFT_MID, 0, 0);
// ------------------------------------------------------------------------------
    label = lv_label_create(parent);
    //lv_label_set_text(label, "14 C");
    lv_label_bind_text(label, int_subject, "%d°C");
    lv_obj_set_style_text_font(label, font_large, LV_PART_MAIN);
    lv_obj_set_style_pad_left(label, 5, LV_PART_MAIN);
    lv_obj_align_to(label, img_obj, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
}



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*TEMP_INDICATOR_H*/
