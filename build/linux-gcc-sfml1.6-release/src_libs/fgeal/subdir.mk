################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/fgeal/agnostic.cpp 

OBJS += \
./src_libs/fgeal/agnostic.o 

CPP_DEPS += \
./src_libs/fgeal/agnostic.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/fgeal/%.o: ../src_libs/fgeal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -I/home/carlosfaruolo/Codigos/SFML-1.6/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


