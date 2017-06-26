#!/bin/bash
echo "### Preparing makefiles for prime time ###"
echo "- Erasing content of build folder (except configure.sh)..."
mv build/others/linux-gcc-libfgeal-release/configure.sh .
rm build/others/linux-gcc-libfgeal-release/* -rf
mv configure.sh build/others/linux-gcc-libfgeal-release/
echo "- Performing make clean in linux-gcc-libfgeal-release folder..."
cd linux-gcc-libfgeal-release
make clean
cd ..
echo "- Copying makefiles from linux-gcc-libfgeal-release folder to build/others/linux-gcc-libfgeal-release folder..."
cp linux-gcc-libfgeal-release/* -r build/others/linux-gcc-libfgeal-release/
echo "- Replacing current folder with generic BUILD_PATH token..."
./scripts/touch_makefiles.sh ./build/others/linux-gcc-libfgeal-release/ ${PWD} BUILD_PATH
echo "- Replacing current fgeal include folder with generic FGEAL_INCLUDE_PATH token..."
./scripts/touch_makefiles.sh ./build/others/linux-gcc-libfgeal-release/ $1 FGEAL_INCLUDE_PATH
echo "- Replacing current fgeal include folder with generic FGEAL_LIBS_PATH token..."
find ./build/others/linux-gcc-libfgeal-release/ -name makefile -type f -exec echo "Patched " {} \;
find ./build/others/linux-gcc-libfgeal-release/ -name makefile -type f -exec sed -i "s,$2,FGEAL_LIBS_PATH,g" {} \;

echo "Done."
