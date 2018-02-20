file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(Test1 ${Program_SRCS} ${Program_HDRS})
target_link_libraries(Test1 ResourceCalculator)

if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/template.vcxproj.user_path_workdir ${CMAKE_CURRENT_BINARY_DIR}/Test1.vcxproj.user)
endif (WIN32)

set_property(TARGET Test1 PROPERTY FOLDER "TESTS")