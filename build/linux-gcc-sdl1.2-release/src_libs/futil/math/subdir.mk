################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/math/gauss_random.cpp \
../src_libs/futil/math/more_random.cpp \
../src_libs/futil/math/parse_number.cpp \
../src_libs/futil/math/round.cpp 

OBJS += \
./src_libs/futil/math/gauss_random.o \
./src_libs/futil/math/more_random.o \
./src_libs/futil/math/parse_number.o \
./src_libs/futil/math/round.o 

CPP_DEPS += \
./src_libs/futil/math/gauss_random.d \
./src_libs/futil/math/more_random.d \
./src_libs/futil/math/parse_number.d \
./src_libs/futil/math/round.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/math/%.o: ../src_libs/futil/math/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


