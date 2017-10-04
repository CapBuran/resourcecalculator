::Edit this file

::ms32|ms64
SET compiler=ms64
::SET compiler=ms32

::2012|2013|2015
SET versionVS=2015

SET Config=Debug
SET Config=Release

SET QT_INSTALL_DIR=c:\Qt
SET QT_INSTALL_FRAMEWORK_NAME=mingw53_32
SET QT_INSTALL_FRAMEWORK_NAME=msvc2015_64
SET QT_VERSION=5.9.1

PUSHD ..
SET MyRepository=%CD%
PUSHD ..
SET MyProjectDir=%CD%\%compiler%_%versionVS%
SET MyDeployDir=%CD%\%compiler%_%versionVS%_QT_DEPLOY

POPD
POPD

call set_environment.cmd
