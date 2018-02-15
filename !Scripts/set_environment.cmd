
IF %compiler%==ms32 (
 SET ARCH_MS=x86
 SET VER_MS=11.0
 SET ARCH_CMAKE=
)

IF %compiler%==ms64 (
 SET ARCH_MS=amd64
 SET ARCH_CMAKE= Win64
 SET ARCH_BUILD=x64
)

IF %versionVS%==2012 (
 SET VER_MS=11
 CALL "C:\Program Files (x86)\Microsoft Visual Studio %VER_MS%.0\VC\vcvarsall.bat" %ARCH_MS%
)

IF %versionVS%==2013 (
 SET VER_MS=12
 CALL "C:\Program Files (x86)\Microsoft Visual Studio %VER_MS%.0\VC\vcvarsall.bat" %ARCH_MS%
)

IF %versionVS%==2015 (
 SET VER_MS=14
 CALL "C:\Program Files (x86)\Microsoft Visual Studio %VER_MS%.0\VC\vcvarsall.bat" %ARCH_MS%
)

IF %versionVS%==2017 (
 SET VER_MS=15
 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
)

SET CMAKE_GENERATOR_NAME="Visual Studio %VER_MS% %versionVS%%ARCH_CMAKE%"

if exist "C:\Program Files (x86)\CMake 2.8\bin\cmake.exe" set CMAKE_EXE="C:\Program Files (x86)\CMake\bin\cmake.exe"
if exist "C:\Program Files (x86)\CMake 2.8\bin\cmake-gui.exe" set CMAKE_EXE_GUI="C:\Program Files (x86)\CMake\bin\cmake-gui.exe"

if exist "C:\Program Files (x86)\CMake\bin\cmake.exe" set CMAKE_EXE="C:\Program Files (x86)\CMake\bin\cmake.exe"
if exist "C:\Program Files (x86)\CMake\bin\cmake-gui.exe" set CMAKE_EXE_GUI="C:\Program Files (x86)\CMake\bin\cmake-gui.exe"

if exist "C:\Program Files\CMake\bin\cmake.exe" set CMAKE_EXE="C:\Program Files\CMake\bin\cmake.exe"
if exist "C:\Program Files\CMake\bin\cmake-gui.exe" set CMAKE_EXE_GUI="C:\Program Files\CMake\bin\cmake-gui.exe"

IF NOT DEFINED CMAKE_EXE (
  Echo CMake not found: Todo: install cmake from https://cmake.org/download/
  exit -3
)

call C:\QT\%QT_VERSION%\%QT_INSTALL_FRAMEWORK_NAME%\bin\qtenv2.bat

ECHO ==========================================================================
ECHO CMake:                %CMAKE_EXE%
ECHO CMAKE_GENERATOR_NAME: %CMAKE_GENERATOR_NAME%
Echo CMakeGUI:             %CMAKE_EXE_GUI%
ECHO MyRepository:         "%MyRepository%"
ECHO MyProjectDir:         "%MyProjectDir%"
ECHO ==========================================================================
