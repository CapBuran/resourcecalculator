FILE(GLOB FactorioImport_HEADERS *.h)
FILE(GLOB FactorioImport_SOURCES *.cpp)

source_group(Parsers     FILES ParseLog.h)
source_group(Parsers     FILES ParseLog.cpp)

add_library(FactorioImport STATIC ${FactorioImport_HEADERS} ${FactorioImport_SOURCES} )

target_link_libraries(FactorioImport PUBLIC Base64 JsonCpp ResourceCalculator)

set_property(TARGET FactorioImport PROPERTY FOLDER "Imports")

add_subdirectory(FactorioImportEXE)