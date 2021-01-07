file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(TestUnZip ${Program_SRCS} ${Program_HDRS})

#target_include_directories(TestUnZip PRIVATE ${ZIPLIB_INCLUDE_DIR})
#target_link_libraries(TestUnZip ${ZIPLIB_LIBRARY} ${ZLIB_LIBRARY})
#target_link_libraries(TestUnZip ${ZIPLIB_LIBRARY})
target_link_libraries(TestUnZip zip)
set_target_properties(TestUnZip PROPERTIES FOLDER "TESTS")
