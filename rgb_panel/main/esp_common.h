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

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (2) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (800) // Duty Range = (0, 1024)
#define LEDC_FREQUENCY          (40000) // Frequency in Hertz. Set frequency at 40 kHz






#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ESP_COMMON_H*/
