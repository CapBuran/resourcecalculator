file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(Test2 ${Program_SRCS} ${Program_HDRS})
target_link_libraries(Test2 ResourceCalculator)

if (WIN32)
  configure_file( ${CMAKE_SOURCE_DIR}/CMake/template.vcxproj.user_path_workdir ${CMAKE_CURRENT_BINARY_DIR}/Test2.vcxproj.user)
endif (WIN32)

set_target_properties(Test2 PROPERTIES FOLDER "TESTS")

