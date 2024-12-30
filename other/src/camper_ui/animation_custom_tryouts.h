

#include "lvgl.h"
#include <stdio.h>


static void anim_x_cb2(void * var, int32_t v)
{
    lv_obj_set_x((lv_obj_t*)var, v);
}

void test_animation(lv_obj_t *parent)
{
    lv_obj_t *anim_obj;

    anim_obj = lv_obj_create(parent);
    lv_obj_set_style_bg_color(anim_obj, lv_palette_main(LV_PALETTE_RED), 0);
   // lv_obj_set_style_radius(anim_obj, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(anim_obj, 10, 20);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, anim_obj);
    lv_anim_set_values(&a, 0, 50);
    lv_anim_set_duration(&a, 2000);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_playback_duration(&a, 300);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);


    lv_anim_set_exec_cb(&a, anim_x_cb2);
    lv_anim_start(&a);

}

void tryouts(lv_obj_t *parent)
{


lv_anim_timeline_t * anim_timeline;
lv_anim_t a1, a2, a3, a_reset;
lv_obj_t *anim_obj1, *anim_obj2, *anim_obj3;
        /* Create anim timeline */
    anim_timeline = lv_anim_timeline_create();
    lv_obj_update_layout(parent);

    int32_t anim_duration = 1500;

    int32_t x1_size = 50;
    int32_t y1_size = 30;

    int32_t x2_size = 50;
    int32_t y2_size = 35;

    int32_t x3_size = y1_size+y2_size;
    int32_t y3_size = 50;
    int32_t anim_con_width = lv_obj_get_width(parent);
    int32_t anim_con_height = lv_obj_get_height(parent);

    lv_color_t indicator_color = lv_palette_main(LV_PALETTE_GREEN);

    printf("h: %d, w: %d\n", anim_con_width, anim_con_height);

    printf("animate container width: %d\n", anim_con_width);

    int32_t start_x = 0;
    int32_t start_y = 0;

    int32_t end_x = anim_con_width/2 - 30;
    int32_t end_y = anim_con_height * 5/7;

    int32_t nc = 15; // number of circles

    //calculate proportions (horizontal and vertical)

    float x_y_ratio = float(end_x - start_x) / float(end_y - start_y);
    // nx = number of x circles
    // ny = number of y circles
    int32_t nx = (x_y_ratio * nc) / (1+x_y_ratio);
    int32_t ny = nc - nx;

    printf("x_y_ratio = %.2f\nnx = %d\nny = %d\n",  x_y_ratio, nx, ny);

    //set array with circle coordinates in user data
    struct Point{
        int32_t x;
        int32_t y;
    }Point_t;


    Point circle_coords[nc];

    int32_t i =0;
    for(i=0; i<=nx; i++)
    {

        circle_coords[i].x = 0; // x coordinate
        circle_coords[i].y = 0; // y coordinate

    }
    for(i=0; i<=ny; i++)
    {
        circle_coords[nx+i].x = 0; // x coordinate
        circle_coords[nx+i].y = 0; // y coordinate
    }

    for(i=0; i<=nc; i ++)
    {
        printf("(%d, %d)\n", circle_coords[i], circle_coords[i+1]);
    }

    //lv_anim_set_user_data(a, void * user_data)

        int32_t circle_d = 15;
        lv_obj_t *ob;

        ob = lv_obj_create(parent);
        //lv_obj_remove_style_all(ob1);
        lv_obj_set_style_bg_color(ob, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_obj_set_size(ob, circle_d, circle_d);
        lv_obj_set_pos(ob, start_x, start_y);
        //lv_obj_align(ob1, LV_ALIGN_BOTTOM_MID, 0, 10);
        lv_obj_set_style_radius(ob, LV_RADIUS_CIRCLE, 0);



        ob = lv_obj_create(parent);
        //lv_obj_remove_style_all(ob1);
        lv_obj_set_style_bg_color(ob, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_obj_set_size(ob, circle_d, circle_d);
        lv_obj_set_pos(ob, end_x, end_y);
        //lv_obj_align(ob1, LV_ALIGN_BOTTOM_MID, 0, 10);
        lv_obj_set_style_radius(ob, LV_RADIUS_CIRCLE, 0);
}



void tryouts2(lv_obj_t *parent)
{
    lv_anim_timeline_t * anim_timeline;
    lv_anim_t a1x, a1y, a2x, a2y, a3, a3_;
    lv_obj_t *anim_obj1, *anim_obj2, *anim_obj3, *middle_line;
    /* Create anim timeline */
    lv_obj_update_layout(parent);

    int32_t anim_x_duration = 3000;
    int32_t anim_y_duration = 1000;

    int32_t x1_size = 30;
    int32_t y1_size = 30;

    int32_t x2_size = 30;
    int32_t y2_size = 30;

    int32_t x3_size = 30;
    int32_t y3_size = 30;



    int32_t anim_con_width = lv_obj_get_width(parent);
    int32_t anim_con_height = lv_obj_get_height(parent);

    lv_color_t indicator_color = lv_palette_main(LV_PALETTE_GREEN);
    int32_t bottom_x_distance = x1_size + x2_size;


    //draw line in the middle
    // middle_line = lv_obj_create(parent);
    // lv_obj_set_style_bg_color(middle_line, lv_palette_main(LV_PALETTE_DEEP_PURPLE), 0);
    // lv_obj_set_size(middle_line, 50, anim_con_height);
    // lv_obj_align(middle_line, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_opa(middle_line, LV_OPA_100, 0);
    // lv_obj_set_style_border_width(middle_line, 0, 0);



    //lines_dsc to bottom-middle
    anim_obj1 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(anim_obj1, indicator_color, 0);
    lv_obj_set_size(anim_obj1, x1_size, y1_size);
    lv_obj_align(anim_obj1, LV_ALIGN_LEFT_MID, 0, -40);
    lv_obj_set_style_radius(anim_obj1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_opa(anim_obj1, LV_OPA_100, 0);


    //right to bottom-middle
    anim_obj2 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(anim_obj2, indicator_color, 0);
    lv_obj_set_size(anim_obj2, x2_size, y2_size);
    lv_obj_align(anim_obj2, LV_ALIGN_RIGHT_MID, 0, 40);
    lv_obj_set_style_radius(anim_obj2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_opa(anim_obj2, LV_OPA_100, 0);


    //top-middle to bottom-middle
    anim_obj3 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(anim_obj3, indicator_color, 0);
    lv_obj_set_size(anim_obj3, x3_size, y3_size);
    lv_obj_align(anim_obj3, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(anim_obj3, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_opa(anim_obj3, LV_OPA_100, 0);


 //   printf("style width: %d\n", lv_obj_get_style_width(anim_obj1, LV_PART_MAIN));

    lv_anim_init(&a1x);
    lv_anim_set_var(&a1x, anim_obj1);
    lv_anim_set_values(&a1x, 0, (anim_con_width/2) - bottom_x_distance);
    lv_anim_set_duration(&a1x, anim_x_duration);
    lv_anim_set_repeat_delay(&a1x, anim_y_duration);
    lv_anim_set_repeat_count(&a1x, LV_ANIM_REPEAT_INFINITE);

    lv_anim_init(&a1y);
    lv_anim_set_var(&a1y, anim_obj1);
    lv_anim_set_values(&a1y, 0, anim_con_height - y1_size);
    lv_anim_set_duration(&a1y, anim_y_duration);
    lv_anim_set_delay(&a1y, anim_x_duration);
    lv_anim_set_repeat_delay(&a1y, anim_x_duration);
    lv_anim_set_repeat_count(&a1y, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_user_data(&a1x, &a1y);


    lv_anim_init(&a2x);
    lv_anim_set_var(&a2x, anim_obj2);
    lv_anim_set_values(&a2x, 0, -(anim_con_width/2) + bottom_x_distance);
    lv_anim_set_duration(&a2x, anim_x_duration);
    lv_anim_set_repeat_delay(&a2x, anim_y_duration);
    lv_anim_set_repeat_count(&a2x, LV_ANIM_REPEAT_INFINITE);

    lv_anim_init(&a2y);
    lv_anim_set_var(&a2y, anim_obj2);
    lv_anim_set_values(&a2y, 0, anim_con_height- y2_size);
    lv_anim_set_duration(&a2y, anim_y_duration);
    lv_anim_set_delay(&a2y, anim_x_duration);
    lv_anim_set_repeat_delay(&a2y, anim_x_duration);
    lv_anim_set_repeat_count(&a2y, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_user_data(&a2x, &a2y);


    lv_anim_init(&a3_);
    lv_anim_set_var(&a3_, anim_obj3);
    lv_anim_set_values(&a3_, 0, 35 );
    lv_anim_set_duration(&a3_, anim_x_duration);
    lv_anim_set_repeat_delay(&a3_, anim_y_duration);
    lv_anim_set_repeat_count(&a3_, LV_ANIM_REPEAT_INFINITE);

    lv_anim_init(&a3);
    lv_anim_set_var(&a3, anim_obj3);
    lv_anim_set_values(&a3, 37, 75);
    lv_anim_set_duration(&a3, anim_y_duration);
    lv_anim_set_repeat_delay(&a3, 0);
    lv_anim_set_delay(&a3, anim_x_duration);
    lv_anim_set_repeat_delay(&a3, anim_x_duration);
    lv_anim_set_repeat_count(&a3, LV_ANIM_REPEAT_INFINITE);


    // // set animation exec cb
    // lv_anim_set_custom_exec_cb(&a1x, set_x_pos_cb);
    // lv_anim_set_custom_exec_cb(&a1y, set_y_pos_cb);

    // lv_anim_set_custom_exec_cb(&a2x, set_x_pos_cb);
    // lv_anim_set_custom_exec_cb(&a2y, set_y_pos_cb);

    // lv_anim_set_custom_exec_cb(&a3_, set_y_pos_pct_cb);
    // lv_anim_set_custom_exec_cb(&a3, set_y_pos_pct_cb);


    // set path collbacks
    lv_anim_set_path_cb(&a1x, lv_anim_path_linear);
    lv_anim_set_path_cb(&a1y, lv_anim_path_linear);

    lv_anim_set_path_cb(&a2x, lv_anim_path_linear);
    lv_anim_set_path_cb(&a2y, lv_anim_path_linear);

    lv_anim_set_path_cb(&a3_, lv_anim_path_linear);
    lv_anim_set_path_cb(&a3, lv_anim_path_linear);


    // lv_anim_start(&a1x);
    // lv_anim_start(&a1y);
    // lv_anim_start(&a2x);
    // lv_anim_start(&a2y);
    // lv_anim_start(&a3_);
    // lv_anim_start(&a3);


}
