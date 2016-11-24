#!/bin/sh

echo -n "Searching C++ compiler ... "
[ -z `which g++` ] && { echo no; exit; }
echo yes

read -p "Enter fgeal include folder:" FGEAL_INCLUDE_PATH
[ -z "$FGEAL_INCLUDE_PATH" ] && { echo "invalid path"; exit; }

read -p "Enter fgeal library folder (empty means same as include folder):" FGEAL_LIBS_PATH
if [ -z "$FGEAL_LIBS_PATH" ]; then
	FGEAL_LIBS_PATH=FGEAL_INCLUDE_PATH
fi

# todo: check if libfgeal is present

OLD="BUILD_PATH"
INCLUDE_PATH=${PWD%/*}
DPATH=${PWD}

echo "Ready to touch .mk files within the current folder (and subfolders, recursively)."
echo "Replace $OLD tokens with $INCLUDE_PATH ..." 

find $DPATH -name '*.mk' -type f -exec echo "Touched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$INCLUDE_PATH,g" {} \;


OLD="FGEAL_INCLUDE_PATH"
echo "Replace $OLD tokens with $FGEAL_INCLUDE_PATH ..." 

find $DPATH -name '*.mk' -type f -exec echo "Touched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$FGEAL_INCLUDE_PATH,g" {} \;

OLD="FGEAL_LIBS_PATH"
echo "Replace $OLD tokens with $FGEAL_LIBS_PATH ..." 

find $DPATH -name makefile -type f -exec echo "Touched " {} \;
find $DPATH -name makefile -type f -exec sed -i "s,$OLD,$FGEAL_LIBS_PATH,g" {} \;

echo "Ready. Now you can run 'make'"
