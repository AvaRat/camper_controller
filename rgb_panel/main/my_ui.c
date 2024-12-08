/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/examples.html#scatter-chart

#include "esp_log.h"
#include "lvgl.h"
#include "stdio.h"

#include "camper_ui/camper_ui.h"

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

void example_lvgl_demo_ui(lv_disp_t *disp)
{


    camper_ui_create();


}

#ifdef __cplusplus
}
#endif
