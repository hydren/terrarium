#!/bin/sh

echo -n "Searching C++ compiler ... "
[ -z `which g++` ] && { echo no; exit; }
echo yes

echo -n "Searching sdl-config ... "
[ -z `which sdl-config` ] && { echo no; exit; }
echo yes

# create dummy object-file for testing of libraries
echo "int main(int,char **) { return 0; }" | g++ -c -x c++ -o tst.o -

okay=yes
LPATH=`sdl-config --libs | sed 's/ -l.*//;s/-L//'`
echo "Searching libraries in $LPATH"
for LIB in SDL SDL_image SDL_ttf SDL_mixer SDL_gfx
do
  echo -n "library $LIB ... "
  g++ -L$LPATH -l$LIB tst.o 2>/dev/null
  if [ $? = 0 ]
  then echo yes
  else echo no; okay=no
  fi
done
rm -f a.out
rm -f tst.o
[ $okay = no ] && { echo "Some libraries are missing. SDL, SDL_image, SDL_ttf, SDL_mixer and SDL_gfx libraries (development versions) are required to build."; exit; }

OLD="BUILD_PATH"
INCLUDE_PATH=${PWD%/*}
DPATH=${PWD}

echo "Ready to touch .mk files within the current folder (and subfolders, recursively)."
echo "Replace $OLD tokens with $INCLUDE_PATH ..." 

find $DPATH -name '*.mk' -type f -exec echo "Touched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$INCLUDE_PATH,g" {} \;

echo "Ready. Now you can run 'make'"
