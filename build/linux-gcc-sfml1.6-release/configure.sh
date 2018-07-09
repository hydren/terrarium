#!/bin/sh

SFML_PRE_1_6=0

for var in "$@"
do
  if [ "$var" = "--help" ] || [ "$var" = "-h" ]
  then 
    echo "usage: configure.sh [--pre-1.6]"; 
    echo "the --pre-1.6 flag indicates that the installed SFML version is less than 1.6, and attempts compatibility.";
    exit
  fi
done

for var in "$@"
do
  if [ "$var" = "--pre-1.6" ]
  then 
    SFML_PRE_1_6=1;
	echo "Flagged usage of SFML versions prior to 1.6 (experimental and may not work)."
  fi
done

echo -n "Searching C++ compiler ... "
[ -z `which g++` ] && { echo no; exit; }
echo yes

# create dummy object-file for testing of libraries
echo "int main(int,char **) { return 0; }" | g++ -c -x c++ -o tst.o -

okay=yes
echo "Searching libraries in $LPATH"
for LIB in sfml-system sfml-window sfml-graphics sfml-audio
do
  echo -n "library $LIB ... "
  g++ -l$LIB tst.o 2>/dev/null
  if [ $? = 0 ]
  then echo yes
  else echo no; okay=no
  fi
done
rm -f a.out
rm -f tst.o
[ $okay = no ] && { echo "Some libraries are missing. sfml-system, sfml-window, sfml-graphics and sfml-audio (development versions) are required to build."; exit; }

OLD="BUILD_PATH"
INCLUDE_PATH=${PWD%/*}
DPATH=${PWD}

echo "Ready to touch .mk files within the current folder (and subfolders, recursively)."
echo "Replace $OLD tokens with $INCLUDE_PATH ..." 

find $DPATH -name '*.mk' -type f -exec echo "Touched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$INCLUDE_PATH,g" {} \;

if [ "$SFML_PRE_1_6" -eq 1 ]
then
	echo -n "Uncommenting FGEAL_SFML1_6_ENABLE_COMPATIBILITY_PRE_1_6 macro in src_libs/fgeal/adapters/sfml1.6/implementation.hpp ...";
	sed -i "s,//#define FGEAL_SFML1_6_ENABLE_COMPATIBILITY_PRE_1_6,#define FGEAL_SFML1_6_ENABLE_COMPATIBILITY_PRE_1_6,g" ../src_libs/fgeal/adapters/sfml1.6/implementation.hpp;
	echo "done";
fi

echo "Ready. Now you can run 'make'"