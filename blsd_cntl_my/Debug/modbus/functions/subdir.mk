################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus/functions/mbfunccoils.c \
../modbus/functions/mbfuncdiag.c \
../modbus/functions/mbfuncdisc.c \
../modbus/functions/mbfuncholding.c \
../modbus/functions/mbfuncinput.c \
../modbus/functions/mbfuncother.c \
../modbus/functions/mbutils.c 

OBJS += \
./modbus/functions/mbfunccoils.o \
./modbus/functions/mbfuncdiag.o \
./modbus/functions/mbfuncdisc.o \
./modbus/functions/mbfuncholding.o \
./modbus/functions/mbfuncinput.o \
./modbus/functions/mbfuncother.o \
./modbus/functions/mbutils.o 

C_DEPS += \
./modbus/functions/mbfunccoils.d \
./modbus/functions/mbfuncdiag.d \
./modbus/functions/mbfuncdisc.d \
./modbus/functions/mbfuncholding.d \
./modbus/functions/mbfuncinput.d \
./modbus/functions/mbfuncother.d \
./modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
modbus/functions/%.o: ../modbus/functions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F411xE '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/include" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/port" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/rtu" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/ascii" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/tcp" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/Components/Common" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/Components/l6398" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/X-NUCLEO-IHM08M1" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/MC_6Step_Lib/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/MotorControl/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/CMSIS/Include"  -O2 -g3 -Wall -fmessage-length=0 -fverbose-asm -save-temps=obj -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


