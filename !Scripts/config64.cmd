::Edit this file

::ms32|ms64
SET compiler=ms64
::SET compiler=ms32

::2012|2013|2015
SET versionVS=2015

SET Config=Debug
SET Config=Release

PUSHD ..
SET MyRepository=%CD%
PUSHD ..
SET MyProjectDir=%CD%\%compiler%_%versionVS%

POPD
POPD

call set_environment.cmd
