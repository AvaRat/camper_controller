idf_component_register(SRCS "main.cpp" "power_switch/BTS_power_switch.cpp" "camper_control/camper_control.cpp" "gpio/gpio_extended.cpp"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)
