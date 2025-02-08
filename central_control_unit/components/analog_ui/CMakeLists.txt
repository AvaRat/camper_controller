idf_component_register(SRCS "led_indicators.cpp" "analog_ui.cpp"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES "esp-idf-cxx" "driver" "led_strip"
                    )
