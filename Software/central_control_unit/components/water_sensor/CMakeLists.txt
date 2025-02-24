set(SOURCES water_sensor.cpp)
#set(INCLUDES ".")

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES "gpio_pal"
                    )
