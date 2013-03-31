################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../internal/CBrownianTree.cpp \
../internal/CFault.cpp \
../internal/CGradient.cpp \
../internal/CGradientNoise.cpp \
../internal/CHeightmap.cpp \
../internal/CHeightmapRenderer.cpp \
../internal/CHydraulicErosion.cpp \
../internal/CImageBuffer.cpp \
../internal/CInputProcessor.cpp \
../internal/CNoise.cpp \
../internal/CParticleDeposition.cpp \
../internal/CPerturbation.cpp \
../internal/CPointsSet2i.cpp \
../internal/CThermalErosion.cpp \
../internal/CVoronoi.cpp \
../internal/helpers.cpp 

OBJS += \
./internal/CBrownianTree.o \
./internal/CFault.o \
./internal/CGradient.o \
./internal/CGradientNoise.o \
./internal/CHeightmap.o \
./internal/CHeightmapRenderer.o \
./internal/CHydraulicErosion.o \
./internal/CImageBuffer.o \
./internal/CInputProcessor.o \
./internal/CNoise.o \
./internal/CParticleDeposition.o \
./internal/CPerturbation.o \
./internal/CPointsSet2i.o \
./internal/CThermalErosion.o \
./internal/CVoronoi.o \
./internal/helpers.o 

CPP_DEPS += \
./internal/CBrownianTree.d \
./internal/CFault.d \
./internal/CGradient.d \
./internal/CGradientNoise.d \
./internal/CHeightmap.d \
./internal/CHeightmapRenderer.d \
./internal/CHydraulicErosion.d \
./internal/CImageBuffer.d \
./internal/CInputProcessor.d \
./internal/CNoise.d \
./internal/CParticleDeposition.d \
./internal/CPerturbation.d \
./internal/CPointsSet2i.d \
./internal/CThermalErosion.d \
./internal/CVoronoi.d \
./internal/helpers.d 


# Each subdirectory must supply rules for building sources it contributes
internal/%.o: ../internal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/dragomir/git/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


