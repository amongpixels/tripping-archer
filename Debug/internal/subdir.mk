################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../internal/CFault.cpp \
../internal/CHeightmap.cpp \
../internal/CImageBuffer.cpp \
../internal/CNoise.cpp \
../internal/CSimplexNoise.cpp \
../internal/IBufferProcessor.cpp 

OBJS += \
./internal/CFault.o \
./internal/CHeightmap.o \
./internal/CImageBuffer.o \
./internal/CNoise.o \
./internal/CSimplexNoise.o \
./internal/IBufferProcessor.o 

CPP_DEPS += \
./internal/CFault.d \
./internal/CHeightmap.d \
./internal/CImageBuffer.d \
./internal/CNoise.d \
./internal/CSimplexNoise.d \
./internal/IBufferProcessor.d 


# Each subdirectory must supply rules for building sources it contributes
internal/%.o: ../internal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/dragomir/git/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


