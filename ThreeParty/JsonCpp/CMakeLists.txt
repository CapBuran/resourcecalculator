file(GLOB JSON_HEADERS include/json/*.h)
file(GLOB JSON_SOURCES src/*.cpp)

ADD_LIBRARY(JsonCpp STATIC ${JSON_SOURCES} ${JSON_HEADERS})

set_property(TARGET JsonCpp PROPERTY FOLDER "Three Party")

target_include_directories(JsonCpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
