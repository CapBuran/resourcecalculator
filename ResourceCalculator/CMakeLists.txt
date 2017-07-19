FILE(GLOB ResourceCalculator_HEADERS *.h)
FILE(GLOB ResourceCalculator_SOURCES *.cpp)

source_group(Item     FILES Item.h)
source_group(Item     FILES Item.cpp)
source_group(Item     FILES ItemCollection.h)
source_group(Item     FILES ItemCollection.cpp)

source_group(Module   FILES Module.h)
source_group(Module   FILES Module.cpp)
source_group(Module   FILES ModuleCollection.cpp)
source_group(Module   FILES ModuleCollection.h)

source_group(Recipe   FILES Recipe.cpp)
source_group(Recipe   FILES Recipe.h)
source_group(Recipe   FILES RecipeCollection.cpp)
source_group(Recipe   FILES RecipeCollection.h)

source_group(Factory  FILES Factory.h)
source_group(Factory  FILES Factory.cpp)
source_group(Factory  FILES FactoryCollection.h)
source_group(Factory  FILES FactoryCollection.cpp)

source_group(Base     FILES Types.h)
source_group(Base     FILES Types.cpp)

source_group(Icon     FILES Icon.h)
source_group(Icon     FILES Icon.cpp)
source_group(Icon     FILES IconCollection.h)
source_group(Icon     FILES IconCollection.cpp)

source_group(Other    FILES ParamsCollection.h)
source_group(Other    FILES ParamsCollection.cpp)
source_group(Other    FILES ProductionChainModel.cpp)
source_group(Other    FILES ProductionChainModel.h)
source_group(Other    FILES Serializer.h)

add_library(ResourceCalculator STATIC ${ResourceCalculator_HEADERS} ${ResourceCalculator_SOURCES} )

target_link_libraries(ResourceCalculator PUBLIC Base64 JsonCpp)

set_property(TARGET ResourceCalculator PROPERTY FOLDER "Calculator")
