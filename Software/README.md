# Software for Camper Controller

UI is currently written in pure C while Control unit is done with a mix of C and C++.  This might be unified in the future.

# Overview
This part is split into [an UI part](Software/touch_screen_ui)
and [central  control unit](Software/central_control_unit).

## UI
UI is written using LVGL framework. Currently an app for 7-inch screen was done. See [an actual app](Software/touch_screen_ui/7-inch-lcd-esp32-s3/main/main.c) for more details.
Also vs-code simulator was made to allow for quick prototyping without uploading code to MCU. [a simulator](Software/touch_screen_ui/lv_port_pc_vscode)


## Central Control Unit (CCU)

Main module which will serve as a source of information to the UI. Module will communicate using CAN. This part is under heavy development currently, but some components were already done like: high-side-switch porting [hss-esp.cpp](Software/central_control_unit/components/high_side_switch/hss-esp.cpp) 