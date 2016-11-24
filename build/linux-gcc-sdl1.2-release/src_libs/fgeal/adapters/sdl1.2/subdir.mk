################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/fgeal/adapters/sdl1.2/core.cpp \
../src_libs/fgeal/adapters/sdl1.2/display.cpp \
../src_libs/fgeal/adapters/sdl1.2/event.cpp \
../src_libs/fgeal/adapters/sdl1.2/font.cpp \
../src_libs/fgeal/adapters/sdl1.2/image.cpp \
../src_libs/fgeal/adapters/sdl1.2/sound.cpp 

OBJS += \
./src_libs/fgeal/adapters/sdl1.2/core.o \
./src_libs/fgeal/adapters/sdl1.2/display.o \
./src_libs/fgeal/adapters/sdl1.2/event.o \
./src_libs/fgeal/adapters/sdl1.2/font.o \
./src_libs/fgeal/adapters/sdl1.2/image.o \
./src_libs/fgeal/adapters/sdl1.2/sound.o 

CPP_DEPS += \
./src_libs/fgeal/adapters/sdl1.2/core.d \
./src_libs/fgeal/adapters/sdl1.2/display.d \
./src_libs/fgeal/adapters/sdl1.2/event.d \
./src_libs/fgeal/adapters/sdl1.2/font.d \
./src_libs/fgeal/adapters/sdl1.2/image.d \
./src_libs/fgeal/adapters/sdl1.2/sound.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/fgeal/adapters/sdl1.2/%.o: ../src_libs/fgeal/adapters/sdl1.2/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


