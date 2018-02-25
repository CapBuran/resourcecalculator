
###################################################################################################

message(STATUS "CMAKE_VERSION='${CMAKE_VERSION}'")
message(STATUS "CMAKE_GENERATOR='${CMAKE_GENERATOR}'")
cmake_minimum_required(VERSION 2.8.11)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 " )
set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} " )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

#project(Factorio_QT C CXX)
project(Factorio_QT CXX)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5 REQUIRED Widgets LinguistTools)

include(Tools)

DETECT_X64()

DISPATCH_OPTIONS()

ADD_COMMON_DEFINITIONS()

include(GiveMePath)

GIVE_ME_PATH()

add_subdirectory(ThreeParty)
add_subdirectory(ResourceCalculator)
add_subdirectory(Interface)
add_subdirectory(Tests)
add_subdirectory(Program)
