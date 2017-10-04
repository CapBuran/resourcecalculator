@ECHO OFF

CALL config64.cmd



SET SourceDir=%MyProjectDir%\Interface\Windows\Release

SET EXE_NAME=ResurceWin.exe

SET PATH=%PATH%;%QT_INSTALL_DIR%\Tools\%QT_INSTALL_FRAMEWORK_NAME%\bin
  
xcopy %SourceDir%\%EXE_NAME% %MyDeployDir%\ /y
 
%QT_INSTALL_DIR%\%QT_VERSION%\%QT_INSTALL_FRAMEWORK_NAME%\bin\windeployqt --release --force --compiler-runtime %MyDeployDir%\%EXE_NAME%

