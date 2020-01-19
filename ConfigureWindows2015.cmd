SET REP=%CD%

SET QT_VERSION=5.9.1
SET QT_INSTALL_DIR=C:\QT
SET QT_VER_DIR=msvc2015_64

CALL %QT_INSTALL_DIR%\%QT_VERSION%\%QT_VER_DIR%\bin\qtenv2.bat

PUSHD ..
SET MyRepository=%CD%
PUSHD ..
SET MyProjectDir=%CD%\%compiler%_%versionVS%
SET MyDeployDir=%CD%\%compiler%_%versionVS%_QT_DEPLOY

if exist "C:\Program Files (x86)\CMake\bin\cmake.exe" set CMAKE_EXE="C:\Program Files (x86)\CMake\bin\cmake.exe"
if exist "C:\Program Files\CMake\bin\cmake.exe" set CMAKE_EXE="C:\Program Files\CMake\bin\cmake.exe"

if not exist "%REP%\bin" MKDIR "%REP%\bin"

PUSHD "%REP%\bin"
%CMAKE_EXE% -A x64 "%REP%"
exit
%CMAKE_EXE% -A x64 "%REP%" > "%REP%\cmake.log" 2>&1
%CMAKE_EXE% --build . --config debug  > "%REP%\build_debug.log" 2>&1
%CMAKE_EXE% --build . --config release  > "%REP%\build_release.log" 2>&1
POPD
