idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES "high_side_switch" 
                    PRIV_REQUIRES pthread high_side_switch water_sensor esp_adc cd74hc_mux analog_ui)

                    


