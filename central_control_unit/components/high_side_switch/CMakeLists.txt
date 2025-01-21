idf_build_get_property(target IDF_TARGET)

file(GLOB HssSources "corelib/*.cpp" "*.cpp" "pal_esp/*.cpp" "pal_esp_expander/*.cpp")

set(SOURCES ${HssSources})
set(INCLUDES "." "pal" "corelib" "pal_esp" "pal_esp_expander")
            

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ${INCLUDES}
                    REQUIRES "esp_io_expander"
                    PRIV_REQUIRES "esp-idf-cxx" "pthread" "esp_io_expander" "cd74hc_mux" "esp_adc"
                    )