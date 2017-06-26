################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/exception.cpp \
../src_libs/futil/properties.cpp 

OBJS += \
./src_libs/futil/exception.o \
./src_libs/futil/properties.o 

CPP_DEPS += \
./src_libs/futil/exception.d \
./src_libs/futil/properties.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/%.o: ../src_libs/futil/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


