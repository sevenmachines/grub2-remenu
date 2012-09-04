################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DefaultsEntry.cpp \
../src/GenericEntry.cpp \
../src/GrubConfigObject.cpp \
../src/GrubDefaultsObject.cpp \
../src/GrubScriptObject.cpp \
../src/Main.cpp \
../src/MainWindow.cpp \
../src/MenuEntry.cpp 

OBJS += \
./src/DefaultsEntry.o \
./src/GenericEntry.o \
./src/GrubConfigObject.o \
./src/GrubDefaultsObject.o \
./src/GrubScriptObject.o \
./src/Main.o \
./src/MainWindow.o \
./src/MenuEntry.o 

CPP_DEPS += \
./src/DefaultsEntry.d \
./src/GenericEntry.d \
./src/GrubConfigObject.d \
./src/GrubDefaultsObject.d \
./src/GrubScriptObject.d \
./src/Main.d \
./src/MainWindow.d \
./src/MenuEntry.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/gtkmm-3.0 -I/usr/include/libgksu -O2 -g -pedantic -Wall -c -fmessage-length=0 `pkg-config --cflags gtkmm-3.0 libgksu2` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


