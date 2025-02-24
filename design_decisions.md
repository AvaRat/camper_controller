# Design Decisions

## 12V Load Switching and protection

A couple of options are available that are widely used in automotive electronics:
1. physical buttons with thermal fuses
2. relays with thermal fuses
3. semiconductor based switches with  built-in protection

My first design bas based on relays with thermal fuses. It's pretty easy to do and components are widely available. This would probably by the go-to solution for most. However after doing some research I discovered that a recent trend in automotive electronics is to utilize silicon-based switching with protection. Not only it allows to have more control but also the size of a whole device can be decreased. Additionally a module with a built in protection can be "hidden" deeply and no access for fuse replacement is required. 
I decided to try the silicon-based switching eventually. BTS700x family of switches from Infineon were used as they provide a C++ library to control them which makes a development much faster. [high-side-switch repo](https://github.com/Infineon/high-side-switch)


## MCU for Control Unit
ESP32 platform was chosen becouse of my past experience with it. I experimented with RP-Pico for some time, but with much broader, ready to use component library available for ESP-IDF platform development will be much faster. Price is comparable between those two. 

## Lightning
Initially standard 12V LED strip was planned. However with addressable LED strips like those based on WS2812b chip there are much more possibilities for amazing lightning effects, color changes and more. This is my current approach. It's realised using RMT module in ESP.

## Water Sensor
A simple sensor based on 4 metal probes is perfectly enough. This was already tested and solution based on comparator circuit (like LM393) works great.

## User Interface
Initially I wanted to do some user interface on linux OS with a SBC like RPI zero. This hovewer will increase the cost of a whole system rather significantly ($20 for a just a board without display) and the energy usage. Currently I'm exploring aa ESP32-S3 based display module like this one [crowpanel 7inch](https://www.elecrow.com/crowpanel-advance-7-0-hmi-esp32-ai-display-800x480-artificial-intelligent-ips-touch-screen-support-meshtastic-and-arduino-lvgl-micropython.html)
The cost for a whole module is $20-$30 which is great. Simple UI app was created for such module and you can find it here [7_inch lcd LVGL app](Software/touch_screen_ui/7-inch-lcd-esp32-s3/main/main.c)


