################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/string/actions.cpp \
../src_libs/futil/string/more_operators.cpp \
../src_libs/futil/string/split.cpp 

OBJS += \
./src_libs/futil/string/actions.o \
./src_libs/futil/string/more_operators.o \
./src_libs/futil/string/split.o 

CPP_DEPS += \
./src_libs/futil/string/actions.d \
./src_libs/futil/string/more_operators.d \
./src_libs/futil/string/split.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/string/%.o: ../src_libs/futil/string/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -I"FGEAL_INCLUDE_PATH" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


