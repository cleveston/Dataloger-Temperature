################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/ASerial.c" \
"../Generated_Code/Cpu.c" \
"../Generated_Code/IO_Map.c" \
"../Generated_Code/Vectors.c" \
"../Generated_Code/Virq.c" \
"../Generated_Code/Vlvd.c" \
"../Generated_Code/Vtpm1ovf.c" \
"../Generated_Code/Vtpm2ovf.c" \
"../Generated_Code/Vtpm3ovf.c" \
"../Generated_Code/WDog.c" \
"../Generated_Code/relogio.c" \

C_SRCS += \
../Generated_Code/ASerial.c \
../Generated_Code/Cpu.c \
../Generated_Code/IO_Map.c \
../Generated_Code/Vectors.c \
../Generated_Code/Virq.c \
../Generated_Code/Vlvd.c \
../Generated_Code/Vtpm1ovf.c \
../Generated_Code/Vtpm2ovf.c \
../Generated_Code/Vtpm3ovf.c \
../Generated_Code/WDog.c \
../Generated_Code/relogio.c \

OBJS += \
./Generated_Code/ASerial_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/IO_Map_c.obj \
./Generated_Code/Vectors_c.obj \
./Generated_Code/Virq_c.obj \
./Generated_Code/Vlvd_c.obj \
./Generated_Code/Vtpm1ovf_c.obj \
./Generated_Code/Vtpm2ovf_c.obj \
./Generated_Code/Vtpm3ovf_c.obj \
./Generated_Code/WDog_c.obj \
./Generated_Code/relogio_c.obj \

OBJS_QUOTED += \
"./Generated_Code/ASerial_c.obj" \
"./Generated_Code/Cpu_c.obj" \
"./Generated_Code/IO_Map_c.obj" \
"./Generated_Code/Vectors_c.obj" \
"./Generated_Code/Virq_c.obj" \
"./Generated_Code/Vlvd_c.obj" \
"./Generated_Code/Vtpm1ovf_c.obj" \
"./Generated_Code/Vtpm2ovf_c.obj" \
"./Generated_Code/Vtpm3ovf_c.obj" \
"./Generated_Code/WDog_c.obj" \
"./Generated_Code/relogio_c.obj" \

C_DEPS += \
./Generated_Code/ASerial_c.d \
./Generated_Code/Cpu_c.d \
./Generated_Code/IO_Map_c.d \
./Generated_Code/Vectors_c.d \
./Generated_Code/Virq_c.d \
./Generated_Code/Vlvd_c.d \
./Generated_Code/Vtpm1ovf_c.d \
./Generated_Code/Vtpm2ovf_c.d \
./Generated_Code/Vtpm3ovf_c.d \
./Generated_Code/WDog_c.d \
./Generated_Code/relogio_c.d \

C_DEPS_QUOTED += \
"./Generated_Code/ASerial_c.d" \
"./Generated_Code/Cpu_c.d" \
"./Generated_Code/IO_Map_c.d" \
"./Generated_Code/Vectors_c.d" \
"./Generated_Code/Virq_c.d" \
"./Generated_Code/Vlvd_c.d" \
"./Generated_Code/Vtpm1ovf_c.d" \
"./Generated_Code/Vtpm2ovf_c.d" \
"./Generated_Code/Vtpm3ovf_c.d" \
"./Generated_Code/WDog_c.d" \
"./Generated_Code/relogio_c.d" \

OBJS_OS_FORMAT += \
./Generated_Code/ASerial_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/IO_Map_c.obj \
./Generated_Code/Vectors_c.obj \
./Generated_Code/Virq_c.obj \
./Generated_Code/Vlvd_c.obj \
./Generated_Code/Vtpm1ovf_c.obj \
./Generated_Code/Vtpm2ovf_c.obj \
./Generated_Code/Vtpm3ovf_c.obj \
./Generated_Code/WDog_c.obj \
./Generated_Code/relogio_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/ASerial_c.obj: ../Generated_Code/ASerial.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/ASerial.args" -ObjN="Generated_Code/ASerial_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/%.d: ../Generated_Code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Generated_Code/Cpu_c.obj: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Cpu.args" -ObjN="Generated_Code/Cpu_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/IO_Map_c.obj: ../Generated_Code/IO_Map.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/IO_Map.args" -ObjN="Generated_Code/IO_Map_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vectors_c.obj: ../Generated_Code/Vectors.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Vectors.args" -ObjN="Generated_Code/Vectors_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Virq_c.obj: ../Generated_Code/Virq.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Virq.args" -ObjN="Generated_Code/Virq_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vlvd_c.obj: ../Generated_Code/Vlvd.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Vlvd.args" -ObjN="Generated_Code/Vlvd_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vtpm1ovf_c.obj: ../Generated_Code/Vtpm1ovf.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Vtpm1ovf.args" -ObjN="Generated_Code/Vtpm1ovf_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vtpm2ovf_c.obj: ../Generated_Code/Vtpm2ovf.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Vtpm2ovf.args" -ObjN="Generated_Code/Vtpm2ovf_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vtpm3ovf_c.obj: ../Generated_Code/Vtpm3ovf.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/Vtpm3ovf.args" -ObjN="Generated_Code/Vtpm3ovf_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/WDog_c.obj: ../Generated_Code/WDog.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/WDog.args" -ObjN="Generated_Code/WDog_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/relogio_c.obj: ../Generated_Code/relogio.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Generated_Code/relogio.args" -ObjN="Generated_Code/relogio_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


