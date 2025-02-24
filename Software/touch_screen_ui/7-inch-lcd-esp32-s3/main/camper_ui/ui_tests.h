

#include "lvgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define TAG_TESTS "TESTS"
#define FILENAME "ducato_genai.png"

static esp_err_t read_file(const char *path)
{
// SPIFFS CHECK
    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/"FILENAME, &st) == 0) {
        ESP_LOGI(TAG_TESTS, FILENAME" file size: %d", (int)st.st_size);
    }else{
        ESP_LOGI(TAG_TESTS, FILENAME" not found");
    }

    size_t total = 0, used = 0;
    esp_err_t ret;
    ret = esp_spiffs_info("lvgl_assets", &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_TESTS, "Failed to get SPIFFS partition information (%s). NOT Formatting...", esp_err_to_name(ret));
        return ESP_ERR_NOT_FOUND;
    } else {
        ESP_LOGI(TAG_TESTS, "Partition size: total: %d, used: %d", total, used);
    }

    return ESP_OK;
}


static void file_explorer_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        const char * cur_path =  lv_file_explorer_get_current_path(obj);
        const char * sel_fn = lv_file_explorer_get_selected_file_name(obj);
        uint16_t path_len = strlen(cur_path);
        uint16_t fn_len = strlen(sel_fn);

        if((path_len + fn_len) <= LV_FILE_EXPLORER_PATH_MAX_LEN) {
            char file_info[LV_FILE_EXPLORER_PATH_MAX_LEN];

            strcpy(file_info, cur_path);
            strcat(file_info, sel_fn);

            LV_LOG_USER("%s", file_info);
        }
        else    LV_LOG_USER("%s%s", cur_path, sel_fn);
    }
}

void create_file_explorer(lv_obj_t *parent)
{   
    
    lv_obj_t * file_explorer = lv_file_explorer_create(parent);
    lv_file_explorer_set_sort(file_explorer, LV_EXPLORER_SORT_KIND);
    lv_file_explorer_open_dir(file_explorer, "A:/spiffs/");
    
    char home_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(home_dir, "A:/spiffs/");

    LV_LOG_USER("home_dir: %s\n", home_dir);
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_HOME_DIR, home_dir);

    char sd_card_dir[LV_FS_MAX_PATH_LENGTH];
    strcpy(sd_card_dir, "A:/sdcard/");
    lv_file_explorer_set_quick_access_path(file_explorer, LV_EXPLORER_DOCS_DIR, sd_card_dir);

    lv_obj_add_event_cb(file_explorer, file_explorer_event_handler, LV_EVENT_ALL, NULL);
}


void create_camper_genai_screen(lv_obj_t *parent){

    create_file_explorer(parent);
}