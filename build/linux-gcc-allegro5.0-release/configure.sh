#!/bin/sh

echo -n "Searching C++ compiler ... "
[ -z `which g++` ] && { echo no; exit; }
echo yes

# todo: check if allegro 5.0 is present

OLD="BUILD_PATH"
INCLUDE_PATH=${PWD%/*}
DPATH=${PWD}

echo "Ready to touch .mk files within the current folder (and subfolders, recursively)."
echo "Replace $OLD tokens with $INCLUDE_PATH ..." 

find $DPATH -name '*.mk' -type f -exec echo "Touched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$INCLUDE_PATH,g" {} \;

echo "Ready. Now you can run 'make'"
