@echo off

Set FindFileName=qtenv2.bat
set QTCMDLocation=QT_NOT_FOUND
set OLD_DIR_FIND_QT=%CD%

cd /d "c:\QT"

for /f "delims=" %%F in ('dir /b /s "%FindFileName%" 2^>nul') do set QTCMDLocation=%%F

cd /d %OLD_DIR_FIND_QT%

echo %QTCMDLocation%

echo %OLD_DIR_FIND_QT%

set OLD_DIR_FIND_QT=

echo %OLD_DIR_FIND_QT%
