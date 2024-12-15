/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/examples.html#scatter-chart

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lvgl_port.h"
#include "driver/ledc.h"
#include "esp_lcd_touch_gt911.h"

#include "camper_ui.h"
#include "../esp_common.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

        static const char *TAG = "CAMPER_UI_APP";
        /* LCD IO and panel */
        static esp_lcd_panel_handle_t lcd_panel = NULL;
        static esp_lcd_touch_handle_t touch_handle = NULL;

        /* LVGL display and touch */
        static lv_display_t *lvgl_disp = NULL;
        static lv_indev_t *lvgl_touch_indev = NULL;

     //   extern lv_subject_t disp_brightness;

/* LCD size */
#define EXAMPLE_LCD_H_RES (800)
#define EXAMPLE_LCD_V_RES (480)

/* LCD settings */
#define EXAMPLE_LCD_LVGL_FULL_REFRESH (0)
#define EXAMPLE_LCD_LVGL_DIRECT_MODE (1)
#define EXAMPLE_LCD_LVGL_AVOID_TEAR (1)
#define EXAMPLE_LCD_RGB_BOUNCE_BUFFER_MODE (1)
#define EXAMPLE_LCD_DRAW_BUFF_DOUBLE (0)
#define EXAMPLE_LCD_DRAW_BUFF_HEIGHT (100)
#define EXAMPLE_LCD_RGB_BUFFER_NUMS (2)
#define EXAMPLE_LCD_RGB_BOUNCE_BUFFER_HEIGHT (10)

/* LCD pins */
#define EXAMPLE_LCD_GPIO_VSYNC (GPIO_NUM_40)
#define EXAMPLE_LCD_GPIO_HSYNC (GPIO_NUM_39)
#define EXAMPLE_LCD_GPIO_DE (GPIO_NUM_41)
#define EXAMPLE_LCD_GPIO_PCLK (GPIO_NUM_0)
#define EXAMPLE_LCD_GPIO_DISP (-1)
#define EXAMPLE_LCD_GPIO_DATA0 (GPIO_NUM_15)
#define EXAMPLE_LCD_GPIO_DATA1 (GPIO_NUM_17)
#define EXAMPLE_LCD_GPIO_DATA2 (GPIO_NUM_6)
#define EXAMPLE_LCD_GPIO_DATA3 (GPIO_NUM_5)
#define EXAMPLE_LCD_GPIO_DATA4 (GPIO_NUM_4)
#define EXAMPLE_LCD_GPIO_DATA5 (GPIO_NUM_9)
#define EXAMPLE_LCD_GPIO_DATA6 (GPIO_NUM_46)
#define EXAMPLE_LCD_GPIO_DATA7 (GPIO_NUM_3)
#define EXAMPLE_LCD_GPIO_DATA8 (GPIO_NUM_8)
#define EXAMPLE_LCD_GPIO_DATA9 (GPIO_NUM_16)
#define EXAMPLE_LCD_GPIO_DATA10 (GPIO_NUM_1)
#define EXAMPLE_LCD_GPIO_DATA11 (GPIO_NUM_14)
#define EXAMPLE_LCD_GPIO_DATA12 (GPIO_NUM_21)
#define EXAMPLE_LCD_GPIO_DATA13 (GPIO_NUM_47)
#define EXAMPLE_LCD_GPIO_DATA14 (GPIO_NUM_48)
#define EXAMPLE_LCD_GPIO_DATA15 (GPIO_NUM_45)

/* Touch settings */
#define EXAMPLE_TOUCH_I2C_NUM (0)
#define EXAMPLE_TOUCH_I2C_CLK_HZ (400000)

/* LCD touch pins */
#define EXAMPLE_TOUCH_I2C_SCL (GPIO_NUM_20)
#define EXAMPLE_TOUCH_I2C_SDA (GPIO_NUM_19)

#define EXAMPLE_LCD_PANEL_35HZ_RGB_TIMING() \
    {                                       \
        .pclk_hz = 15 * 1000 * 1000,        \
        .h_res = EXAMPLE_LCD_H_RES,         \
        .v_res = EXAMPLE_LCD_V_RES,         \
        .hsync_pulse_width = 48,            \
        .hsync_back_porch = 40,             \
        .hsync_front_porch = 40,            \
        .vsync_pulse_width = 31,            \
        .vsync_back_porch = 13,             \
        .vsync_front_porch = 1,             \
        .flags.pclk_active_neg = true,      \
    }

        static esp_err_t backlight_ledc_init(void)
        {
            esp_err_t ret = ESP_OK;
            // Prepare and then apply the LEDC PWM timer configuration
            const ledc_timer_config_t ledc_timer = {
                .speed_mode = LEDC_MODE,
                .duty_resolution = LEDC_DUTY_RES,
                .timer_num = LEDC_TIMER,
                .freq_hz = LEDC_FREQUENCY, // Set output frequency at 4 kHz
                .clk_cfg = LEDC_AUTO_CLK};
            ret = ledc_timer_config(&ledc_timer);

            // Prepare and then apply the LEDC PWM channel configuration
            ledc_channel_config_t ledc_channel = {
                .gpio_num = LEDC_OUTPUT_IO,
                .speed_mode = LEDC_MODE,
                .channel = LEDC_CHANNEL,
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = LEDC_TIMER,
                .duty = LEDC_DUTY, // Set duty to 0%
                .hpoint = 0};
            ret = ledc_channel_config(&ledc_channel);
            // ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, target_duty, max_fade_time_ms);
            // ledc_fade_func_install(10);
            return ret;
        }

        static esp_err_t app_lcd_init(void)
        {
            esp_err_t ret = ESP_OK;

            /* LCD initialization */
            ESP_LOGI(TAG, "Initialize RGB panel");
            esp_lcd_rgb_panel_config_t panel_conf = {
                .clk_src = SOC_MOD_CLK_PLL_F160M,
                .timings = EXAMPLE_LCD_PANEL_35HZ_RGB_TIMING(),
                .data_width = 16,
                .bits_per_pixel = 16,
                .num_fbs = EXAMPLE_LCD_RGB_BUFFER_NUMS,
#if EXAMPLE_LCD_RGB_BOUNCE_BUFFER_MODE
                .bounce_buffer_size_px = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_RGB_BOUNCE_BUFFER_HEIGHT,
#endif
                .sram_trans_align = 0,
                .psram_trans_align = 64,
                .hsync_gpio_num = EXAMPLE_LCD_GPIO_HSYNC,
                .vsync_gpio_num = EXAMPLE_LCD_GPIO_VSYNC,
                .de_gpio_num = EXAMPLE_LCD_GPIO_DE,
                .pclk_gpio_num = EXAMPLE_LCD_GPIO_PCLK,
                .disp_gpio_num = EXAMPLE_LCD_GPIO_DISP,
                .data_gpio_nums = {
                    EXAMPLE_LCD_GPIO_DATA0,
                    EXAMPLE_LCD_GPIO_DATA1,
                    EXAMPLE_LCD_GPIO_DATA2,
                    EXAMPLE_LCD_GPIO_DATA3,
                    EXAMPLE_LCD_GPIO_DATA4,
                    EXAMPLE_LCD_GPIO_DATA5,
                    EXAMPLE_LCD_GPIO_DATA6,
                    EXAMPLE_LCD_GPIO_DATA7,
                    EXAMPLE_LCD_GPIO_DATA8,
                    EXAMPLE_LCD_GPIO_DATA9,
                    EXAMPLE_LCD_GPIO_DATA10,
                    EXAMPLE_LCD_GPIO_DATA11,
                    EXAMPLE_LCD_GPIO_DATA12,
                    EXAMPLE_LCD_GPIO_DATA13,
                    EXAMPLE_LCD_GPIO_DATA14,
                    EXAMPLE_LCD_GPIO_DATA15,
                },
                .flags.fb_in_psram = 1,
            };
            ESP_GOTO_ON_ERROR(esp_lcd_new_rgb_panel(&panel_conf, &lcd_panel), err, TAG, "RGB init failed");
            ESP_GOTO_ON_ERROR(esp_lcd_panel_init(lcd_panel), err, TAG, "LCD init failed");

            return ret;

        err:
            if (lcd_panel)
            {
                esp_lcd_panel_del(lcd_panel);
            }
            return ret;
        }

        ////////////////////////////////////////TOUCH STUFF
        static esp_err_t app_touch_init(void)
        {
            // Initilize I2C
            const i2c_config_t i2c_conf = {
                .mode = I2C_MODE_MASTER,
                .sda_io_num = EXAMPLE_TOUCH_I2C_SDA,
                .sda_pullup_en = GPIO_PULLUP_ENABLE,
                .scl_io_num = EXAMPLE_TOUCH_I2C_SCL,
                .scl_pullup_en = GPIO_PULLUP_ENABLE,
                .master.clk_speed = EXAMPLE_TOUCH_I2C_CLK_HZ};
            ESP_RETURN_ON_ERROR(i2c_param_config(EXAMPLE_TOUCH_I2C_NUM, &i2c_conf), TAG, "I2C configuration failed");
            ESP_RETURN_ON_ERROR(i2c_driver_install(EXAMPLE_TOUCH_I2C_NUM, i2c_conf.mode, 0, 0, 0), TAG, "I2C initialization failed");

            const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();

            const esp_lcd_touch_io_gt911_config_t tp_gt911_config = {
                .dev_addr = tp_io_config.dev_addr,
            };

            const esp_lcd_touch_config_t tp_cfg = {
                .x_max = EXAMPLE_LCD_H_RES,
                .y_max = EXAMPLE_LCD_V_RES,
                .rst_gpio_num = GPIO_NUM_38,
                .int_gpio_num = -1, // GPIO_NUM_17,
                .levels = {
                    .reset = 0,
                    .interrupt = 0,
                },
                .flags = {
                    .swap_xy = 0,
                    .mirror_x = 0,
                    .mirror_y = 0,
                },
                .driver_data = &tp_gt911_config,
            };
            esp_lcd_panel_io_handle_t tp_io_handle = NULL;
            // const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
            ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)EXAMPLE_TOUCH_I2C_NUM, &tp_io_config, &tp_io_handle), TAG, "");
            esp_err_t er = esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);
            return er;
        }

        static esp_err_t app_lvgl_init(void)
        {
            /* Initialize LVGL */
            const lvgl_port_cfg_t lvgl_cfg = {
                .task_priority = 4,        /* LVGL task priority */
                .task_stack = 61440,       /* LVGL task stack size */
                .task_affinity = -1,       /* LVGL task pinned to core (-1 is no affinity) */
                .task_max_sleep_ms = 1000, /* Maximum sleep in LVGL task */
                .timer_period_ms = 5       /* LVGL timer tick period in ms */
            };
            ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

            uint32_t buff_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT;
#if EXAMPLE_LCD_LVGL_FULL_REFRESH || EXAMPLE_LCD_LVGL_DIRECT_MODE
            buff_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES;
#endif

            /* Add LCD screen */
            ESP_LOGD(TAG, "Add LCD screen");
            const lvgl_port_display_cfg_t disp_cfg = {
                .panel_handle = lcd_panel,
                .buffer_size = buff_size,
                .double_buffer = EXAMPLE_LCD_DRAW_BUFF_DOUBLE,
                .hres = EXAMPLE_LCD_H_RES,
                .vres = EXAMPLE_LCD_V_RES,
                .monochrome = false,
#if LVGL_VERSION_MAJOR >= 9
                .color_format = LV_COLOR_FORMAT_RGB565,
#endif
                .rotation = {
                    .swap_xy = false,
                    .mirror_x = false,
                    .mirror_y = false,
                },
                .flags = {
                    .buff_dma = false,
                    .buff_spiram = true,
#if EXAMPLE_LCD_LVGL_FULL_REFRESH
                    .full_refresh = true,
#elif EXAMPLE_LCD_LVGL_DIRECT_MODE
            .direct_mode = true,
#endif
#if LVGL_VERSION_MAJOR >= 9
                    .swap_bytes = false,
#endif
                }};
            const lvgl_port_display_rgb_cfg_t rgb_cfg = {
                .flags = {
#if EXAMPLE_LCD_RGB_BOUNCE_BUFFER_MODE
                    .bb_mode = true,
#else
            .bb_mode = false,
#endif
#if EXAMPLE_LCD_LVGL_AVOID_TEAR
                    .avoid_tearing = true,
#else
            .avoid_tearing = false,
#endif
                }};
            lvgl_disp = lvgl_port_add_disp_rgb(&disp_cfg, &rgb_cfg);

            /* Add touch input (for selected screen) */
            const lvgl_port_touch_cfg_t touch_cfg = {
                .disp = lvgl_disp,
                .handle = touch_handle,
            };
            lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

            return ESP_OK;
        }

        void camper_app_ui()
        {
            /* LCD HW initialization */
            ESP_ERROR_CHECK(app_lcd_init());

            ESP_ERROR_CHECK(backlight_ledc_init());

            /* Touch initialization */
            ESP_ERROR_CHECK(app_touch_init());

            /* LVGL initialization */
            ESP_ERROR_CHECK(app_lvgl_init());

            /* Show LVGL objects */
            lvgl_port_lock(0);

            camper_ui_create();

            lvgl_port_unlock();
        }

#ifdef __cplusplus
    }
#endif
