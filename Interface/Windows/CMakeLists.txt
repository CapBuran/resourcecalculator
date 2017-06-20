file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(ResurceWin WIN32 ${Program_SRCS} ${Program_HDRS})

qt5_use_modules(ResurceWin Widgets)

target_link_libraries(ResurceWin ResourceCalculator)

if (WIN32)
configure_file( template.vcxproj.user_path_workdir ${CMAKE_CURRENT_BINARY_DIR}/ResurceWin.vcxproj.user)
set_property(TARGET ResurceWin PROPERTY FOLDER "Interface")
endif (WIN32)
