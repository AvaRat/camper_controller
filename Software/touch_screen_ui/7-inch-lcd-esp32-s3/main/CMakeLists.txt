file(GLOB LVGL_ASSETS 
    "assets/*.c"
)




idf_component_register(SRCS "main.c" "esp_common.c" ${LVGL_ASSETS}
                       INCLUDE_DIRS ".")

                       
#target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)


# Create a SPIFFS image from the contents of the 'spiffs_assets' directory
# that fits the partition named 'lvgl_assets'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash'.
spiffs_create_partition_image(lvgl_assets ../spiffs_assets FLASH_IN_PROJECT)
