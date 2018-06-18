################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/fgeal/adapters/allegro4.4/core.cpp \
../src_libs/fgeal/adapters/allegro4.4/display.cpp \
../src_libs/fgeal/adapters/allegro4.4/event.cpp \
../src_libs/fgeal/adapters/allegro4.4/font.cpp \
../src_libs/fgeal/adapters/allegro4.4/image.cpp \
../src_libs/fgeal/adapters/allegro4.4/input.cpp \
../src_libs/fgeal/adapters/allegro4.4/sound.cpp 

OBJS += \
./src_libs/fgeal/adapters/allegro4.4/core.o \
./src_libs/fgeal/adapters/allegro4.4/display.o \
./src_libs/fgeal/adapters/allegro4.4/event.o \
./src_libs/fgeal/adapters/allegro4.4/font.o \
./src_libs/fgeal/adapters/allegro4.4/image.o \
./src_libs/fgeal/adapters/allegro4.4/input.o \
./src_libs/fgeal/adapters/allegro4.4/sound.o 

CPP_DEPS += \
./src_libs/fgeal/adapters/allegro4.4/core.d \
./src_libs/fgeal/adapters/allegro4.4/display.d \
./src_libs/fgeal/adapters/allegro4.4/event.d \
./src_libs/fgeal/adapters/allegro4.4/font.d \
./src_libs/fgeal/adapters/allegro4.4/image.d \
./src_libs/fgeal/adapters/allegro4.4/input.d \
./src_libs/fgeal/adapters/allegro4.4/sound.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/fgeal/adapters/allegro4.4/%.o: ../src_libs/fgeal/adapters/allegro4.4/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


