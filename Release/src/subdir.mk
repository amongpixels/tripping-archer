################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CBrownianTree.cpp \
../src/CFractalNoise.cpp \
../src/CGradient.cpp \
../src/CHeightmap.cpp \
../src/CHeightmapRenderer.cpp \
../src/CHydraulicErosion.cpp \
../src/CInputProcessor.cpp \
../src/CNoise.cpp \
../src/CParticleDeposition.cpp \
../src/CPerturbation.cpp \
../src/CPointsSet2i.cpp \
../src/CThermalErosion.cpp \
../src/CVoronoi.cpp \
../src/helpers.cpp 

OBJS += \
./src/CBrownianTree.o \
./src/CFractalNoise.o \
./src/CGradient.o \
./src/CHeightmap.o \
./src/CHeightmapRenderer.o \
./src/CHydraulicErosion.o \
./src/CInputProcessor.o \
./src/CNoise.o \
./src/CParticleDeposition.o \
./src/CPerturbation.o \
./src/CPointsSet2i.o \
./src/CThermalErosion.o \
./src/CVoronoi.o \
./src/helpers.o 

CPP_DEPS += \
./src/CBrownianTree.d \
./src/CFractalNoise.d \
./src/CGradient.d \
./src/CHeightmap.d \
./src/CHeightmapRenderer.d \
./src/CHydraulicErosion.d \
./src/CInputProcessor.d \
./src/CNoise.d \
./src/CParticleDeposition.d \
./src/CPerturbation.d \
./src/CPointsSet2i.d \
./src/CThermalErosion.d \
./src/CVoronoi.d \
./src/helpers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/dragomir/git/tripping-archer/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


