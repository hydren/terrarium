################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/libgeramun/brush.cpp \
../src_libs/libgeramun/cave.cpp \
../src_libs/libgeramun/debug.cpp \
../src_libs/libgeramun/generators.cpp \
../src_libs/libgeramun/midpoint_displacement.cpp \
../src_libs/libgeramun/miners.cpp \
../src_libs/libgeramun/preview_callback.cpp \
../src_libs/libgeramun/random_walk.cpp \
../src_libs/libgeramun/spring.cpp \
../src_libs/libgeramun/surf_replacer.cpp 

OBJS += \
./src_libs/libgeramun/brush.o \
./src_libs/libgeramun/cave.o \
./src_libs/libgeramun/debug.o \
./src_libs/libgeramun/generators.o \
./src_libs/libgeramun/midpoint_displacement.o \
./src_libs/libgeramun/miners.o \
./src_libs/libgeramun/preview_callback.o \
./src_libs/libgeramun/random_walk.o \
./src_libs/libgeramun/spring.o \
./src_libs/libgeramun/surf_replacer.o 

CPP_DEPS += \
./src_libs/libgeramun/brush.d \
./src_libs/libgeramun/cave.d \
./src_libs/libgeramun/debug.d \
./src_libs/libgeramun/generators.d \
./src_libs/libgeramun/midpoint_displacement.d \
./src_libs/libgeramun/miners.d \
./src_libs/libgeramun/preview_callback.d \
./src_libs/libgeramun/random_walk.d \
./src_libs/libgeramun/spring.d \
./src_libs/libgeramun/surf_replacer.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/libgeramun/%.o: ../src_libs/libgeramun/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -I/home/carlosfaruolo/Codigos/SFML-1.6/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


