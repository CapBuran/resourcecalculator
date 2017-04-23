file(GLOB Program_SRCS *.cpp)
file(GLOB Program_HDRS *.h  )

add_executable(QT_TABLE1 ${Program_SRCS} ${Program_HDRS})

qt5_use_modules(QT_TABLE1 Widgets)

if (WIN32)
configure_file( template.vcxproj.user_path_workdir ${CMAKE_CURRENT_BINARY_DIR}/QT_TABLE1.vcxproj.user)
endif (WIN32)
