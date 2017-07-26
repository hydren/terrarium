################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/exception.cpp \
../src_libs/futil/properties.cpp \
../src_libs/futil/stox.cpp \
../src_libs/futil/string_actions.cpp \
../src_libs/futil/string_extra_operators.cpp \
../src_libs/futil/string_parse.cpp \
../src_libs/futil/string_split.cpp 

C_SRCS += \
../src_libs/futil/chrono.c \
../src_libs/futil/random.c \
../src_libs/futil/round.c \
../src_libs/futil/snprintf.c \
../src_libs/futil/strtox.c 

OBJS += \
./src_libs/futil/chrono.o \
./src_libs/futil/exception.o \
./src_libs/futil/properties.o \
./src_libs/futil/random.o \
./src_libs/futil/round.o \
./src_libs/futil/snprintf.o \
./src_libs/futil/stox.o \
./src_libs/futil/string_actions.o \
./src_libs/futil/string_extra_operators.o \
./src_libs/futil/string_parse.o \
./src_libs/futil/string_split.o \
./src_libs/futil/strtox.o 

CPP_DEPS += \
./src_libs/futil/exception.d \
./src_libs/futil/properties.d \
./src_libs/futil/stox.d \
./src_libs/futil/string_actions.d \
./src_libs/futil/string_extra_operators.d \
./src_libs/futil/string_parse.d \
./src_libs/futil/string_split.d 

C_DEPS += \
./src_libs/futil/chrono.d \
./src_libs/futil/random.d \
./src_libs/futil/round.d \
./src_libs/futil/snprintf.d \
./src_libs/futil/strtox.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/%.o: ../src_libs/futil/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_libs/futil/%.o: ../src_libs/futil/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


