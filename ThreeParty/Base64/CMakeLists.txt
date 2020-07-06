add_library(Base64 STATIC)

target_sources_local(Base64 PRIVATE base64.h base64.cpp)

target_include_directories(Base64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(Base64 PROPERTIES FOLDER "ThreeParty")
