################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../internal/CImageBuffer.cpp \
../internal/CNoise.cpp \
../internal/CSimplexNoise.cpp 

OBJS += \
./internal/CImageBuffer.o \
./internal/CNoise.o \
./internal/CSimplexNoise.o 

CPP_DEPS += \
./internal/CImageBuffer.d \
./internal/CNoise.d \
./internal/CSimplexNoise.d 


# Each subdirectory must supply rules for building sources it contributes
internal/%.o: ../internal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

