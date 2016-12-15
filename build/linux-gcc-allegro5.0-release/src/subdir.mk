################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/animation.cpp \
../src/block.cpp \
../src/entity.cpp \
../src/file_parser.cpp \
../src/game.cpp \
../src/ingame_state.cpp \
../src/main.cpp \
../src/main_menu.cpp \
../src/map.cpp \
../src/menu.cpp \
../src/menu_state.cpp \
../src/physics.cpp \
../src/terrarium_game.cpp \
../src/util.cpp 

OBJS += \
./src/animation.o \
./src/block.o \
./src/entity.o \
./src/file_parser.o \
./src/game.o \
./src/ingame_state.o \
./src/main.o \
./src/main_menu.o \
./src/map.o \
./src/menu.o \
./src/menu_state.o \
./src/physics.o \
./src/terrarium_game.o \
./src/util.o 

CPP_DEPS += \
./src/animation.d \
./src/block.d \
./src/entity.d \
./src/file_parser.d \
./src/game.d \
./src/ingame_state.d \
./src/main.d \
./src/main_menu.d \
./src/map.d \
./src/menu.d \
./src/menu_state.d \
./src/physics.d \
./src/terrarium_game.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"BUILD_PATH/src" -I"BUILD_PATH/src_libs" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


