################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../internal/CFault.cpp \
../internal/CHeightmap.cpp \
../internal/CImageBuffer.cpp \
../internal/CInputProcessor.cpp \
../internal/CNoise.cpp \
../internal/CParticleDeposition.cpp \
../internal/CSimplexNoise.cpp \
../internal/CVoronoi.cpp \
../internal/helpers.cpp 

OBJS += \
./internal/CFault.o \
./internal/CHeightmap.o \
./internal/CImageBuffer.o \
./internal/CInputProcessor.o \
./internal/CNoise.o \
./internal/CParticleDeposition.o \
./internal/CSimplexNoise.o \
./internal/CVoronoi.o \
./internal/helpers.o 

CPP_DEPS += \
./internal/CFault.d \
./internal/CHeightmap.d \
./internal/CImageBuffer.d \
./internal/CInputProcessor.d \
./internal/CNoise.d \
./internal/CParticleDeposition.d \
./internal/CSimplexNoise.d \
./internal/CVoronoi.d \
./internal/helpers.d 


# Each subdirectory must supply rules for building sources it contributes
internal/%.o: ../internal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/dragomir/git/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


