#!/bin/bash
echo "### Preparing makefiles for prime time ###"
echo "- Erasing content of build folder (except configure.sh)..."
mv build/$1/configure.sh .
rm build/$1/* -rf
mv configure.sh build/$1/
echo "- Performing make clean in $1 folder..."
cd $1
make clean
cd ..
echo "- Copying makefiles from $1 folder to build folder..."
cp $1/* -r build/$1/
echo "- Replacing current folder with generic BUILD_PATH token..."
./touch_makefiles.sh ./build/$1/ ${PWD} BUILD_PATH

echo "Done."
