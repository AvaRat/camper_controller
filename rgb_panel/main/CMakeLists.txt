file(GLOB LVGL_ASSETS 
    "assets/*.c"
)

idf_component_register(SRCS "main.c" "my_ui.c" ${LVGL_ASSETS}
                       INCLUDE_DIRS ".")

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)