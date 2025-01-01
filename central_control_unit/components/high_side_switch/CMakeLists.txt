idf_build_get_property(target IDF_TARGET)

file(GLOB HssSources corelib/*.cpp)

set(SOURCES ${HssSources})
set(INCLUDES "." "pal" "corelib")
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ${INCLUDES}
                    )