::Edit this file

SET compiler=ms64

::2012|2013|2015|2017
SET versionVS=2017

SET Config=Debug
SET Config=Release

SET QT_INSTALL_DIR=c:\Qt
SET QT_INSTALL_FRAMEWORK_NAME=mingw53_32
SET QT_INSTALL_FRAMEWORK_NAME=msvc%versionVS%_64
SET QT_VERSION=5.9.1
SET QT_VERSION=5.10.1

PUSHD ..
SET MyRepository=%CD%
PUSHD ..
SET MyProjectDir=%CD%\%compiler%_%versionVS%
SET MyDeployDir=%CD%\%compiler%_%versionVS%_QT_DEPLOY

POPD
POPD

call set_environment.cmd
