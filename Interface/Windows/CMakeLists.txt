set_property(DIRECTORY PROPERTY CLEAN_NO_CUSTOM TRUE)

file(GLOB Program_HDRS *.h)
file(GLOB Program_SRCS *.cpp)
file(GLOB Program_TRNS *.cpp)

set(TS_FILES translations/ResurceWin_ru_RU.ts)

option(UPDATE_TRANSLATIONS "Update source translation (files translations/*.ts)." ON)

if(UPDATE_TRANSLATIONS)
  message(STATUS "UPDATE_TRANSLATIONS option is set.")
  qt5_create_translation(QM_FILES ${Program_TRNS} ${TS_FILES})
else()
  qt5_add_translation(QM_FILES ${TS_FILES})
endif()

add_custom_target(translations DEPENDS ${QM_FILES})

configure_file( ResurceWin.qrc ${CMAKE_CURRENT_BINARY_DIR}/ResurceWin.qrc)

qt5_add_resources(Program_TRS ${CMAKE_CURRENT_BINARY_DIR}/ResurceWin.qrc)
qt5_add_resources(Program_RES_GUI ResurceGui.qrc)

source_group(MainWindow     FILES mainwindow.h)
source_group(MainWindow     FILES mainwindow.cpp)
source_group(MainWindow     FILES ProductionChainWidget.h)
source_group(MainWindow     FILES ProductionChainWidget.cpp)
source_group(MainWindow     FILES ProductionChainTabWidget.h)
source_group(MainWindow     FILES ProductionChainTabWidget.cpp)

source_group(ItemsDialogs   FILES ItemsEditDialog.h)
source_group(ItemsDialogs   FILES ItemsEditDialog.cpp)

source_group(ItemsDialogs   FILES ItemSelectedDialog.h)
source_group(ItemsDialogs   FILES ItemSelectedDialog.cpp)
source_group(IconsDialogs   FILES IconSelectedDialog.h)
source_group(IconsDialogs   FILES IconSelectedDialog.cpp)

source_group(RecipesDialogs FILES RecipesEditDialog.h)
source_group(RecipesDialogs FILES RecipesEditDialog.cpp)

source_group(FactoryDialogs FILES FactoryTypesEditDialog.h)
source_group(FactoryDialogs FILES FactoryTypesEditDialog.cpp)
source_group(FactoryDialogs FILES FactorysEditDialog.h)
source_group(FactoryDialogs FILES FactorysEditDialog.cpp)

source_group(ModulesDialogs FILES ModulesEditDialog.cpp)
source_group(ModulesDialogs FILES ModulesEditDialog.h)
source_group(ModulesDialogs FILES ModulesSelectDialog.cpp)
source_group(ModulesDialogs FILES ModulesSelectDialog.h)
source_group(ModulesDialogs FILES ModuleSelectDialog.cpp)
source_group(ModulesDialogs FILES ModuleSelectDialog.h)

source_group(NoEdit         FILES ${CMAKE_CURRENT_BINARY_DIR}/ResurceWin.qrc)
source_group(NoEdit         FILES ${CMAKE_CURRENT_BINARY_DIR}/qrc_ResurceWin.cpp)
source_group(NoEdit         FILES ${CMAKE_CURRENT_BINARY_DIR}/qrc_ResurceGui.cpp)
source_group(NoEdit         FILES ${CMAKE_CURRENT_BINARY_DIR}/ResurceWin_automoc.cpp)
                                                                                    
source_group(Translation        FILES translations/ResurceWin_ru_RU.ts)

add_executable(ResurceWin WIN32 ${Program_SRCS} ${Program_HDRS} ${Program_TRS} ${TS_FILES} ${Program_RES} ${Program_RES_GUI} )

qt5_use_modules(ResurceWin Widgets)

target_link_libraries(ResurceWin ResourceCalculator)

add_dependencies(ResurceWin translations)

if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/template.vcxproj.user_path_workdir ${CMAKE_CURRENT_BINARY_DIR}/ResurceWin.vcxproj.user)
set_property(TARGET ResurceWin PROPERTY FOLDER "Interface")
set_property(TARGET translations PROPERTY FOLDER "Interface")
endif (WIN32)
