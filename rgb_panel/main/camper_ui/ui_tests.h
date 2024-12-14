

#include "lvgl.h"
#include <stdio.h>
#include <esp_psram.h>


#define TAG_TESTS "TESTS"
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
static void print_mem_info(){
    lv_mem_monitor_t mon;
    ESP_LOGI(TAG_TESTS, "Free heap size %u", (unsigned int) esp_get_free_heap_size());
    ESP_LOGI(TAG_TESTS, "Free internal heap size %u", (unsigned int) esp_get_free_internal_heap_size());
    ESP_LOGI(TAG_TESTS, "PSRAM size: %d", esp_psram_get_size());

    lv_mem_monitor(&mon);
    printf("   Dynamic memory stats\n");
    printf("total_size: %d\npct used: %d\nfree size: %d\nfree biggest: %d\n\n", (int)mon.total_size, (int)mon.used_pct, (int)mon.free_size, (int)mon.free_biggest_size);

    // lv_mem_monitor_core(&mon);
    // printf("   Heap memory stats\n");
    // printf("total_size: %d\npct used: %d\n", (int)mon.total_size, (int)mon.used_pct);
}

static esp_err_t read_file(const char *path)
{
    ESP_LOGI(TAG_TESTS, "Reading file %s", path);
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        ESP_LOGE(TAG_TESTS, "Failed to open file for reading");
        return ESP_FAIL;
    }
    char line[EXAMPLE_MAX_CHAR_SIZE];
    fread(line, sizeof(char), EXAMPLE_MAX_CHAR_SIZE, f);
    
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG_TESTS, "Read from file: '%s'", line);

    return ESP_OK;
}



#define FILENAME "small_3.png"
static void btn_handle(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
        read_file("/sdcard/"FILENAME);
        print_mem_info();
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}


void create_camper_genai_screen(lv_obj_t *parent){
 /*
    LV_IMAGE_DECLARE(genai_camper_ducato);
    lv_obj_t *img = lv_image_create(parent);
    lv_image_set_src(img, &genai_camper_ducato);
    lv_obj_center(img);
 */  
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
   // lv_obj_t *lbl = lv_label_create(lv_screen_active());

    

    lv_obj_t * btn1 = lv_button_create(parent);
    lv_obj_add_event_cb(btn1, btn_handle, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);
    lv_obj_set_size(btn1, 100, 50);
    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_add_flag(btn1, LV_OBJ_FLAG_FLOATING);



  //  LV_IMAGE_DECLARE(img_wink_png);

    lv_obj_t * img = lv_image_create(parent);
    // Assuming a File system is attached to letter 'A'
    // E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h 

    print_mem_info();

    printf("!!!!!!!!!!!!img creation!!\n\n");
/*
    LV_IMAGE_DECLARE(genai_camper_ducato);
    img = lv_image_create(parent);
    lv_image_set_src(img, &genai_camper_ducato);
    lv_obj_center(img);
*/

    lv_image_set_src(img, "A:/sdcard/"FILENAME);
    lv_obj_center(img);
    lv_obj_set_size(img, 400, 200);



    print_mem_info();
    

}
