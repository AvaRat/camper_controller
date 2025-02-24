/**
 * @file esp_common.h
 *
 */

#ifndef ESP_COMMON_H
#define ESP_COMMON_H


#ifdef __cplusplus
extern "C" {
#endif


#include "lvgl.h"
#include "driver/ledc.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_log.h"
#include <esp_psram.h>
#include "esp_spiffs.h"


#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (2) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (800) // Duty Range = (0, 1024)
#define LEDC_FREQUENCY          (40000) // Frequency in Hertz. Set frequency at 40 kHz


//TODO global max path len
#define MAX_BTN_IMG_PATH    64





#define MOUNT_POINT "/sdcard"
#define ASSETS_MOUNT_POINT "/sdcard/assets"

#define ICONS_MOUNT_POINT "A:/spiffs/icons/"
#define SCREENSAVER_IMG_MOUNT_POINT "A:/spiffs/ducato_genai.png"

// Pin assignments can be set in menuconfig, see "SD SPI Example Configuration" menu.
// You can also change the pin assignments here by changing the following 4 lines.
#define PIN_NUM_MISO  GPIO_NUM_13
#define PIN_NUM_MOSI  GPIO_NUM_11
#define PIN_NUM_CLK   GPIO_NUM_12
#define PIN_NUM_CS    GPIO_NUM_10


/*
ESP hardware stuff
*/
void screen_brightness_observer_cb(lv_observer_t *observer, lv_subject_t *subject);
void init_observers();
esp_err_t s_example_write_file(const char *path, char *data);
esp_err_t s_example_read_file(const char *path);
esp_err_t init_sd_card();
esp_err_t init_spiffs();



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESP_COMMON_H*/
