file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(TestUnZip ${Program_SRCS} ${Program_HDRS})

target_include_directories(TestUnZip PRIVATE ${ZLIB_INCLUDES})

target_link_libraries(TestUnZip zlibstatic)

set_property(TARGET TestUnZip PROPERTY FOLDER "TESTS")