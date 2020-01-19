###################################################################################################

message(STATUS "CMAKE_VERSION='${CMAKE_VERSION}'")
message(STATUS "CMAKE_GENERATOR='${CMAKE_GENERATOR}'")
cmake_minimum_required(VERSION 3.12)

project(Factorio_QT C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

include(GiveMePath)
include(CMake/CMakeFunctions.cmake)
include(CMake/CMakeDownload.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTORCC ON)

if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  set(CMAKE_CXX_STANDARD 17)
  add_definitions(-DWIN32 -D_WIN32 -DWINDOWS -D_WINDOWS -D_WIN32_WINNT=0x0601 -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++17 -lpthread -lrt -ldl -Wno-deprecated-declarations")
endif()

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5 REQUIRED Widgets LinguistTools)

GIVE_ME_PATH()

download_three_party_tar_gz( "https://zlib.net/zlib-1.2.11.tar.gz" "zlib-1.2.11.tar.gz")
add_subdirectory(${CMAKE_BINARY_DIR}/ThreeParty/External/zlib-1.2.11)

add_subdirectory(ThreeParty)
add_subdirectory(ResourceCalculator)
add_subdirectory(Interface)
add_subdirectory(Tests)
add_subdirectory(Program)
