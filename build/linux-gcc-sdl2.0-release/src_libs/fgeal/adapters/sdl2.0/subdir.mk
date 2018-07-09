################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/fgeal/adapters/sdl2.0/core.cpp \
../src_libs/fgeal/adapters/sdl2.0/display.cpp \
../src_libs/fgeal/adapters/sdl2.0/event.cpp \
../src_libs/fgeal/adapters/sdl2.0/font.cpp \
../src_libs/fgeal/adapters/sdl2.0/graphics.cpp \
../src_libs/fgeal/adapters/sdl2.0/image.cpp \
../src_libs/fgeal/adapters/sdl2.0/input.cpp \
../src_libs/fgeal/adapters/sdl2.0/sound.cpp 

OBJS += \
./src_libs/fgeal/adapters/sdl2.0/core.o \
./src_libs/fgeal/adapters/sdl2.0/display.o \
./src_libs/fgeal/adapters/sdl2.0/event.o \
./src_libs/fgeal/adapters/sdl2.0/font.o \
./src_libs/fgeal/adapters/sdl2.0/graphics.o \
./src_libs/fgeal/adapters/sdl2.0/image.o \
./src_libs/fgeal/adapters/sdl2.0/input.o \
./src_libs/fgeal/adapters/sdl2.0/sound.o 

CPP_DEPS += \
./src_libs/fgeal/adapters/sdl2.0/core.d \
./src_libs/fgeal/adapters/sdl2.0/display.d \
./src_libs/fgeal/adapters/sdl2.0/event.d \
./src_libs/fgeal/adapters/sdl2.0/font.d \
./src_libs/fgeal/adapters/sdl2.0/graphics.d \
./src_libs/fgeal/adapters/sdl2.0/image.d \
./src_libs/fgeal/adapters/sdl2.0/input.d \
./src_libs/fgeal/adapters/sdl2.0/sound.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/fgeal/adapters/sdl2.0/%.o: ../src_libs/fgeal/adapters/sdl2.0/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


