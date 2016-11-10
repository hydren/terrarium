#!/bin/bash
echo "### Preparing makefiles for prime time ###"
echo "- Erasing content of build folder (except configure.sh)..."
mv build/configure.sh .
rm build/* -rf
mv configure.sh build/
echo "- Performing make clean in linux-gcc-release folder..."
cd linux-gcc-release
make clean
cd ..
echo "- Copying makefiles from linux-gcc-release folder to build folder..."
cp linux-gcc-release/* -r build/
echo "- Replacing current folder with generic BUILD_PATH token..."
./touch_makefiles.sh ./build/ ${PWD} BUILD_PATH
echo "- Replacing current fgeal include folder with generic FGEAL_INCLUDE_PATH token..."
./touch_makefiles.sh ./build/ $1 FGEAL_INCLUDE_PATH
echo "- Replacing current fgeal include folder with generic FGEAL_LIBS_PATH token..."
find ./build/ -name makefile -type f -exec echo "Patched " {} \;
find ./build/ -name makefile -type f -exec sed -i "s,$2,FGEAL_LIBS_PATH,g" {} \;

echo "Done."
