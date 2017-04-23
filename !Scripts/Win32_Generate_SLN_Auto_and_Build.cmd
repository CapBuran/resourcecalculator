@ECHO OFF

CALL config32.cmd

::if exist "%MyProjectDir%" RD /s /q "%MyProjectDir%"
MKDIR "%MyProjectDir%"
PUSHD "%MyProjectDir%"

%CMAKE_EXE% -G%CMAKE_GENERATOR_NAME% %MyRepository%

devenv.com Factorio_QT.sln /Build "%Config%|%ARCH_BUILD%" /Project "ALL_BUILD"

POPD

::PAUSE