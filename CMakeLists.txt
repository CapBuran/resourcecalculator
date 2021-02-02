###################################################################################################

message(STATUS "CMAKE_VERSION='${CMAKE_VERSION}'")
message(STATUS "CMAKE_GENERATOR='${CMAKE_GENERATOR}'")
cmake_minimum_required(VERSION 3.12)

project(Factorio_QT C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

include(GiveMePath)
include(CMake/CMakeFunctions.cmake)
include(CMake/CMakeDownload.cmake)
include(DownloadProject)

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
find_package(Qt5 COMPONENTS Core Widgets LinguistTools)

GIVE_ME_PATH()

SET(ENABLE_GNUTLS OFF)
SET(ENABLE_MBEDTLS OFF)
SET(ENABLE_OPENSSL OFF)
SET(ENABLE_WINDOWS_CRYPTO OFF)

SET(BUILD_SHARED_LIBS OFF)

download_project(PROJ                zlib
                 GIT_REPOSITORY      git@github.com:madler/zlib.git
                 GIT_TAG             master
                 UPDATE_DISCONNECTED 1
)

download_project(PROJ                libzip
                 GIT_REPOSITORY      git@github.com:nih-at/libzip.git
                 GIT_TAG             master
                 UPDATE_DISCONNECTED 1
)

add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
SET(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})

if (WIN32)
  SET(ZLIB_LIBRARY_DEBUG ${zlib_BINARY_DIR}/Debug/zlibstaticd.lib)
  SET(ZLIB_LIBRARY_RELEASE ${zlib_BINARY_DIR}/Release/zlibstatic.lib)
else (WIN32)
  SET(ZLIB_LIBRARY ${ZLIB})
endif (WIN32)

add_subdirectory(${libzip_SOURCE_DIR} ${libzip_BINARY_DIR})
SET(ZIPLIB_INCLUDE_DIR ${libzip_SOURCE_DIR}/lib ${libzip_BINARY_DIR})
#SET(ZIPLIB_LIBRARY zip)
SET(ZIPLIB_LIBRARY ${ZIPLIB})

add_subdirectory(ThreeParty)

add_subdirectory(ResourceCalculator)
add_subdirectory(FactorioImport)
add_subdirectory(Interface)
add_subdirectory(Tests)
add_subdirectory(Program)
