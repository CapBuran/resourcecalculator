FILE(GLOB FactorioCalculator_HEADERS *.h)
FILE(GLOB FactorioCalculator_SOURCES *.cpp)

add_library(FactorioCalculator STATIC ${FactorioCalculator_HEADERS} ${FactorioCalculator_SOURCES} )
set_property(TARGET FactorioCalculator PROPERTY FOLDER "Factorio")
