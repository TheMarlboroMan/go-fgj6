#!/bin/bash

if [[ $# -ne 1 ]]
then
	echo "usage: make_appimage.sh path_to_linuxdeploy"
	echo "make sure to build with make -f makefile_linux BUILDTYPEFLAGS=-DAS_APPIMAGE=1"
	exit 1
fi;

#export SIGN=1
#export SIGN_KEY=0xLAST8CHARSOFGPGKEYID

$1 --appdir AppDir
cp -r data AppDir/usr/share
mkdir -p AppDir/usr/share/metainfo
cp linuxdeploy/*.xml AppDir/usr/share/metainfo/
$1 --executable ./go --appdir AppDir -i ./linuxdeploy/go.png -d ./linuxdeploy/go.desktop --output appimage
exit 0
