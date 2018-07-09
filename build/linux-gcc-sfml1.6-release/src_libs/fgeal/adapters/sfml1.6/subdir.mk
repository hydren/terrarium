################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/fgeal/adapters/sfml1.6/core.cpp \
../src_libs/fgeal/adapters/sfml1.6/display.cpp \
../src_libs/fgeal/adapters/sfml1.6/event.cpp \
../src_libs/fgeal/adapters/sfml1.6/font.cpp \
../src_libs/fgeal/adapters/sfml1.6/graphics.cpp \
../src_libs/fgeal/adapters/sfml1.6/image.cpp \
../src_libs/fgeal/adapters/sfml1.6/input.cpp \
../src_libs/fgeal/adapters/sfml1.6/sound.cpp 

OBJS += \
./src_libs/fgeal/adapters/sfml1.6/core.o \
./src_libs/fgeal/adapters/sfml1.6/display.o \
./src_libs/fgeal/adapters/sfml1.6/event.o \
./src_libs/fgeal/adapters/sfml1.6/font.o \
./src_libs/fgeal/adapters/sfml1.6/graphics.o \
./src_libs/fgeal/adapters/sfml1.6/image.o \
./src_libs/fgeal/adapters/sfml1.6/input.o \
./src_libs/fgeal/adapters/sfml1.6/sound.o 

CPP_DEPS += \
./src_libs/fgeal/adapters/sfml1.6/core.d \
./src_libs/fgeal/adapters/sfml1.6/display.d \
./src_libs/fgeal/adapters/sfml1.6/event.d \
./src_libs/fgeal/adapters/sfml1.6/font.d \
./src_libs/fgeal/adapters/sfml1.6/graphics.d \
./src_libs/fgeal/adapters/sfml1.6/image.d \
./src_libs/fgeal/adapters/sfml1.6/input.d \
./src_libs/fgeal/adapters/sfml1.6/sound.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/fgeal/adapters/sfml1.6/%.o: ../src_libs/fgeal/adapters/sfml1.6/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


