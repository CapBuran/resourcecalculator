FILE(GLOB ResourceCalculator_HEADERS *.h)
FILE(GLOB ResourceCalculator_SOURCES *.cpp)

add_library(ResourceCalculator STATIC ${ResourceCalculator_HEADERS} ${ResourceCalculator_SOURCES} )

target_link_libraries(ResourceCalculator PUBLIC Base64 JsonCpp)

set_property(TARGET ResourceCalculator PROPERTY FOLDER "Calculator")
