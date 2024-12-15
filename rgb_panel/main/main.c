/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "camper_ui/camper_app_ui.h"
#include "stdio.h"
#include "esp_common.h"



void app_main(void)
{
    init_spiffs();
    init_sd_card();
    camper_app_ui();
}
