idf_component_register(SRCS "main.cpp" "power_switch/BTS_power_switch.cpp"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)
