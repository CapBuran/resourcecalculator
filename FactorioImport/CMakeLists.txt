FILE(GLOB FactorioImport_HEADERS *.h)
FILE(GLOB FactorioImport_SOURCES *.cpp)

source_group(Parsers     FILES ParseLog.h)
source_group(Parsers     FILES ParseLog.cpp)

source_group(Parsers     FILES ParseElements.h)
source_group(Parsers     FILES ParseElements.cpp)

source_group(Parsers     FILES Convert.h)
source_group(Parsers     FILES Convert.cpp)

add_library(FactorioImport STATIC ${FactorioImport_HEADERS} ${FactorioImport_SOURCES} )

target_link_libraries(FactorioImport PUBLIC Base64 JsonCpp ResourceCalculator)

set_property(TARGET FactorioImport PROPERTY FOLDER "Imports")

target_include_directories(FactorioImport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(FactorioImportEXE)

set_target_properties(FactorioImport PROPERTIES FOLDER "Factorio")
