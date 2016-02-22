################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Events.c" \
"../Sources/ProcessorExpert.c" \
"../Sources/amostragem.c" \
"../Sources/botaoIRQ.c" \
"../Sources/eeprom.c" \
"../Sources/serial.c" \
"../Sources/timer.c" \

C_SRCS += \
../Sources/Events.c \
../Sources/ProcessorExpert.c \
../Sources/amostragem.c \
../Sources/botaoIRQ.c \
../Sources/eeprom.c \
../Sources/serial.c \
../Sources/timer.c \

OBJS += \
./Sources/Events_c.obj \
./Sources/ProcessorExpert_c.obj \
./Sources/amostragem_c.obj \
./Sources/botaoIRQ_c.obj \
./Sources/eeprom_c.obj \
./Sources/serial_c.obj \
./Sources/timer_c.obj \

OBJS_QUOTED += \
"./Sources/Events_c.obj" \
"./Sources/ProcessorExpert_c.obj" \
"./Sources/amostragem_c.obj" \
"./Sources/botaoIRQ_c.obj" \
"./Sources/eeprom_c.obj" \
"./Sources/serial_c.obj" \
"./Sources/timer_c.obj" \

C_DEPS += \
./Sources/Events_c.d \
./Sources/ProcessorExpert_c.d \
./Sources/amostragem_c.d \
./Sources/botaoIRQ_c.d \
./Sources/eeprom_c.d \
./Sources/serial_c.d \
./Sources/timer_c.d \

C_DEPS_QUOTED += \
"./Sources/Events_c.d" \
"./Sources/ProcessorExpert_c.d" \
"./Sources/amostragem_c.d" \
"./Sources/botaoIRQ_c.d" \
"./Sources/eeprom_c.d" \
"./Sources/serial_c.d" \
"./Sources/timer_c.d" \

OBJS_OS_FORMAT += \
./Sources/Events_c.obj \
./Sources/ProcessorExpert_c.obj \
./Sources/amostragem_c.obj \
./Sources/botaoIRQ_c.obj \
./Sources/eeprom_c.obj \
./Sources/serial_c.obj \
./Sources/timer_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/Events_c.obj: ../Sources/Events.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Events.args" -ObjN="Sources/Events_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/ProcessorExpert_c.obj: ../Sources/ProcessorExpert.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/ProcessorExpert.args" -ObjN="Sources/ProcessorExpert_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/amostragem_c.obj: ../Sources/amostragem.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/amostragem.args" -ObjN="Sources/amostragem_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/botaoIRQ_c.obj: ../Sources/botaoIRQ.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/botaoIRQ.args" -ObjN="Sources/botaoIRQ_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/eeprom_c.obj: ../Sources/eeprom.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/eeprom.args" -ObjN="Sources/eeprom_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/serial_c.obj: ../Sources/serial.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/serial.args" -ObjN="Sources/serial_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/timer_c.obj: ../Sources/timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/timer.args" -ObjN="Sources/timer_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


