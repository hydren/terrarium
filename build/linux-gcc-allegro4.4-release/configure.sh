#!/bin/sh

for var in "$@"
do
	if [ "$var" = "--help" ] || [ "$var" = "-h" ]
		then echo "usage: configure.sh [--skip-allegro-config] [--use-logg|--use-alogg]"; 
			echo "the --skip-allegro-config flag triggers skipping the check for the allegro-config program";
			echo "the --use-logg flag triggers the usage of the logg library instead of alogg";
			echo "the --use-alogg flag triggers the usage of the alogg library instead of logg";
			echo "this script also uses the EXT_LPATH environment variable as an additional library path";
			exit
		fi
done

SKIP_ALLEGRO_CONFIG=0
ALLEGRO_OGG_LIBRARY=logg

for var in "$@"
do
	if [ "$var" = "--skip-allegro-config" ]
		then SKIP_ALLEGRO_CONFIG=1;
		echo "Will skip usage of allegro-config."
	fi
	
	if [ "$var" = "--use-logg" ]
		then ALLEGRO_OGG_LIBRARY=logg;
		echo "Will use logg library as OGG library."
	fi

	if [ "$var" = "--use-alogg" ]
		then ALLEGRO_OGG_LIBRARY=alogg;
		echo "Will use alogg library as OGG library."
	fi
done

echo -n "Searching C++ compiler ... "
[ -z `which g++` ] && { echo no; exit; }
echo yes

if [ "$SKIP_ALLEGRO_CONFIG" -eq 0 ]
	then echo -n "Searching allegro-config ... "
		[ -z `which allegro-config` ] && { echo no; exit; };
		LPATH=`allegro-config --libs | sed 's/ -l.*//;s/-Wl.*//;s/-L//'`;
	echo yes
fi

# create dummy object-file for testing of libraries
echo "int main(int,char **) { return 0; }" | g++ -c -x c++ -o tst.o -

okay=yes
echo "Searching libraries in $LPATH"

echo -n "library alleg (Allegro)..."
g++ -L$LPATH -L$EXT_LPATH -lalleg tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes
  else
	echo no; okay=no
fi

echo -n "library loadpng..."
g++ -L$LPATH -L$EXT_LPATH -lloadpng tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes;
    ALLEGRO_LOADPNG_AVAILABLE=1
  else
	echo no;
	ALLEGRO_LOADPNG_AVAILABLE=0
fi

echo -n "library jpgalleg (JPGAlleg)..."
g++ -L$LPATH -L$EXT_LPATH -ljpgalleg tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes;
    ALLEGRO_JPGALLEG_AVAILABLE=1
  else
	echo no;
	ALLEGRO_JPGALLEG_AVAILABLE=0
fi

echo -n "library $ALLEGRO_OGG_LIBRARY..."
g++ -L$LPATH -L$EXT_LPATH -l$ALLEGRO_OGG_LIBRARY tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes;
    ALLEGRO_OGG_AVAILABLE=1
  else
	echo no;
	ALLEGRO_OGG_AVAILABLE=0
fi

echo -n "library alttf (AllegTTF)..."
g++ -L$LPATH -L$EXT_LPATH -lalttf tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes;
    ALLEGRO_ALLEGTTF_AVAILABLE=1
  else
	echo no;
	ALLEGRO_ALLEGTTF_AVAILABLE=0
fi

echo -n "library algif..."
g++ -L$LPATH -L$EXT_LPATH -lalgif tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes;
    ALLEGRO_ALGIF_AVAILABLE=1
  else
	echo no;
	ALLEGRO_ALGIF_AVAILABLE=0
fi

echo -n "library almp3 (AllegroMP3)..."
g++ -L$LPATH -L$EXT_LPATH -lalmp3 tst.o 2>/dev/null
if [ $? = 0 ] 
  then
    echo yes;
    ALLEGRO_ALLEGROMP3_AVAILABLE=1
  else
	echo no;
	ALLEGRO_ALLEGROMP3_AVAILABLE=0
fi

rm -f a.out
rm -f tst.o
[ $okay = no ] && { echo "The allegro library is missing. The allegro library (development version) is required to build."; exit; }

OLD="BUILD_PATH"
INCLUDE_PATH=${PWD%/*}
DPATH=${PWD}

echo "Ready to touch .mk files within the current folder (and subfolders, recursively)."
echo "Replace $OLD tokens with $INCLUDE_PATH ..." 

find $DPATH -name '*.mk' -type f -exec echo "Touched " {} \;
find $DPATH -name '*.mk' -type f -exec sed -i "s,$OLD,$INCLUDE_PATH,g" {} \;

if [ "$ALLEGRO_OGG_LIBRARY" = "alogg" ] && [ "$ALLEGRO_OGG_AVAILABLE" -eq 1 ]
then
	echo -n "Replace -llogg with -lalogg...";
	sed -i "s,-llogg,-lalogg,g" objects.mk;
	echo "done"
fi

if [ "$ALLEGRO_LOADPNG_AVAILABLE" -eq 0 ]
then
	echo "loadpng library not available, removing usage...";
	echo -n "commenting out FGEAL_ALLEGRO_4_LOADPNG_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_LOADPNG_INSTALLED,//#define FGEAL_ALLEGRO_4_LOADPNG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "removing -lloadpng flag...";
	sed -i "s,-lloadpng,,g" objects.mk;
	echo "done"
fi

if [ "$ALLEGRO_JPGALLEG_AVAILABLE" -eq 0 ]
then
	echo "JPGAlleg library not available, removing usage...";
	echo -n "commenting out FGEAL_ALLEGRO_4_JPGALLEG_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_JPGALLEG_INSTALLED,//#define FGEAL_ALLEGRO_4_JPGALLEG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "removing -ljpgalleg flag...";
	sed -i "s,-ljpgalleg,,g" objects.mk;
	echo "done"
fi

if [ "$ALLEGRO_OGG_AVAILABLE" -eq 0 ]
then
	echo "logg or alogg library not available, removing usage...";
	echo -n "commenting out FGEAL_ALLEGRO_4_LOGG_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_LOGG_INSTALLED,//#define FGEAL_ALLEGRO_4_LOGG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "commenting out FGEAL_ALLEGRO_4_ALOGG_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_ALOGG_INSTALLED,//#define FGEAL_ALLEGRO_4_ALOGG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "removing -llogg flag...";
	sed -i "s,-llogg,,g" objects.mk;
	echo "done"
	echo -n "removing -lalogg flag...";
	sed -i "s,-lalogg,,g" objects.mk;
	echo "done"
elif [ "$ALLEGRO_OGG_LIBRARY" = "logg" ] 
then
	echo -n "Commenting out FGEAL_ALLEGRO_4_ALOGG_INSTALLED in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_ALOGG_INSTALLED,//#define FGEAL_ALLEGRO_4_ALOGG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "Uncommenting FGEAL_ALLEGRO_4_LOGG_INSTALLED in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,//#define FGEAL_ALLEGRO_4_LOGG_INSTALLED,#define FGEAL_ALLEGRO_4_LOGG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "Replace -lalogg with -llogg...";
	sed -i "s,-lalogg,-llogg,g" objects.mk;
	echo "done"
elif [ "$ALLEGRO_OGG_LIBRARY" = "alogg" ] 
then
	echo -n "Commenting out FGEAL_ALLEGRO_4_LOGG_INSTALLED in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_LOGG_INSTALLED,//#define FGEAL_ALLEGRO_4_LOGG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "Uncommenting FGEAL_ALLEGRO_4_ALOGG_INSTALLED in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ..."
	sed -i "s,//#define FGEAL_ALLEGRO_4_ALOGG_INSTALLED,#define FGEAL_ALLEGRO_4_ALOGG_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "Replace -llogg with -lalogg...";
	sed -i "s,-llogg,-lalogg,g" objects.mk;
	echo "done"
fi

if [ "$ALLEGRO_ALLEGTTF_AVAILABLE" -eq 0 ]
then 
	echo "AllegTTF library not available, removing usage...";
	echo -n "commenting out FGEAL_ALLEGRO_4_ALLEGTTF_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_ALLEGTTF_INSTALLED,//#define FGEAL_ALLEGRO_4_ALLEGTTF_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "removing -lalttf flag...";
	sed -i "s,-lalttf,,g" objects.mk;
	echo "done"
fi

if [ "$ALLEGRO_ALGIF_AVAILABLE" -eq 0 ]
then 
	echo "algif library not available, removing usage...";
	echo -n "commenting out FGEAL_ALLEGRO_4_ALGIF_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ...";
	sed -i "s,#define FGEAL_ALLEGRO_4_ALGIF_INSTALLED,//#define FGEAL_ALLEGRO_4_ALGIF_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "removing -lalgif flag...";
	sed -i "s,-lalgif,,g" objects.mk;
	echo "done"
fi

if [ "$ALLEGRO_ALLEGROMP3_AVAILABLE" -eq 0 ]
then
	echo "AllegroMP3 library not available, removing usage...";
	echo -n "commenting out FGEAL_ALLEGRO_4_ALLEGROMP3_INSTALLED flag in src_libs/fgeal/adapters/allegro4.4/implementation.hpp ..."; 
	sed -i "s,#define FGEAL_ALLEGRO_4_ALLEGROMP3_INSTALLED,//#define FGEAL_ALLEGRO_4_ALLEGROMP3_INSTALLED,g" ../src_libs/fgeal/adapters/allegro4.4/implementation.hpp;
	echo "done";
	echo -n "removing -lalmp3 flag...";
	sed -i "s,-lalmp3,,g" objects.mk;
	echo "done"
fi

echo "done"

echo "Ready. Now you can run 'make'"
