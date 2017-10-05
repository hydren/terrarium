################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/animation.cpp \
../src/block.cpp \
../src/entity.cpp \
../src/ingame_state.cpp \
../src/inventory.cpp \
../src/loading_state.cpp \
../src/main.cpp \
../src/main_menu_state.cpp \
../src/map.cpp \
../src/map_io.cpp \
../src/physics.cpp \
../src/resolutions.cpp \
../src/terrarium_game.cpp 

OBJS += \
./src/animation.o \
./src/block.o \
./src/entity.o \
./src/ingame_state.o \
./src/inventory.o \
./src/loading_state.o \
./src/main.o \
./src/main_menu_state.o \
./src/map.o \
./src/map_io.o \
./src/physics.o \
./src/resolutions.o \
./src/terrarium_game.o 

CPP_DEPS += \
./src/animation.d \
./src/block.d \
./src/entity.d \
./src/ingame_state.d \
./src/inventory.d \
./src/loading_state.d \
./src/main.d \
./src/main_menu_state.d \
./src/map.d \
./src/map_io.d \
./src/physics.d \
./src/resolutions.d \
./src/terrarium_game.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -I"FGEAL_INCLUDE_PATH" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


