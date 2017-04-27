file(GLOB JSON_HEADERS *.h)
file(GLOB JSON_SOURCES *.cpp)

ADD_LIBRARY(Base64 ${JSON_SOURCES} ${JSON_HEADERS})

set_property(TARGET Base64 PROPERTY FOLDER "Three Party")

target_include_directories(Base64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
