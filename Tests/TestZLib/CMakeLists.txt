file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(TestZLib ${Program_SRCS} ${Program_HDRS})

target_include_directories(TestZLib PRIVATE ${ZLIB_INCLUDE_DIR})
#target_link_libraries(TestZLib ${ZLIB})
target_link_libraries(TestZLib zlib)

set_target_properties(TestZLib PROPERTIES FOLDER "TESTS")

