
###################################################################################################

message(STATUS "CMAKE_VERSION='${CMAKE_VERSION}'")
message(STATUS "CMAKE_GENERATOR='${CMAKE_GENERATOR}'")
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

project(Factorio_QT C CXX)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
#find_package(Qt5Widgets REQUIRED)
find_package(Qt5 REQUIRED Widgets)

if(WIN32)

elseif(WIN32)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)
set( CMAKE_CPU_SUPPORT_FLAGS " -mssse3 -Wno-long-long -fPIC -m64 -DRTE_MACHINE_CPUFLAG_SSE -DRTE_MACHINE_CPUFLAG_SSE2 -DRTE_MACHINE_CPUFLAG_SSE3 -DRTE_MACHINE_CPUFLAG_SSSE3 -DRTE_MACHINE_CPUFLAG_SSE4_1 -DRTE_MACHINE_CPUFLAG_SSE4_2 -DRTE_MACHINE_CPUFLAG_AES -DRTE_MACHINE_CPUFLAG_PCLMULQDQ -DRTE_MACHINE_CPUFLAG_AVX -DRTE_MACHINE_CPUFLAG_RDRAND " )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -Wall -Wno-long-long -fPIC -std=c++11 " )
set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wfatal-errors -Wall -Wno-long-long -fPIC " )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type " )
set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wno-return-type " )
set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${CMAKE_CPU_SUPPORT_FLAGS} " )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CPU_SUPPORT_FLAGS} " )

endif(WIN32)

include(Tools)

DETECT_X64()

DISPATCH_OPTIONS()

ADD_COMMON_DEFINITIONS()

include(GiveMePath)

GIVE_ME_PATH()

add_subdirectory(ThreeParty)
add_subdirectory(FactorioCalculator)
add_subdirectory(Program)
