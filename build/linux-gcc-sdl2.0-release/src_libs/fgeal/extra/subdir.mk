################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/fgeal/extra/game.cpp \
../src_libs/fgeal/extra/menu.cpp \
../src_libs/fgeal/extra/sprite.cpp 

OBJS += \
./src_libs/fgeal/extra/game.o \
./src_libs/fgeal/extra/menu.o \
./src_libs/fgeal/extra/sprite.o 

CPP_DEPS += \
./src_libs/fgeal/extra/game.d \
./src_libs/fgeal/extra/menu.d \
./src_libs/fgeal/extra/sprite.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/fgeal/extra/%.o: ../src_libs/fgeal/extra/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


