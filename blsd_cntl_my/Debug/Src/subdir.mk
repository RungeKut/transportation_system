################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adctenso.c \
../Src/button.c \
../Src/eprom.c \
../Src/hall.c \
../Src/main.c \
../Src/median.c \
../Src/poweron.c \
../Src/proto.c \
../Src/ramp.c \
../Src/spimbaddr.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/stm32f4xx_swtimer.c \
../Src/swtimer.c \
../Src/system_stm32f4xx.c \
../Src/template.c 

OBJS += \
./Src/adctenso.o \
./Src/button.o \
./Src/eprom.o \
./Src/hall.o \
./Src/main.o \
./Src/median.o \
./Src/poweron.o \
./Src/proto.o \
./Src/ramp.o \
./Src/spimbaddr.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/stm32f4xx_swtimer.o \
./Src/swtimer.o \
./Src/system_stm32f4xx.o \
./Src/template.o 

C_DEPS += \
./Src/adctenso.d \
./Src/button.d \
./Src/eprom.d \
./Src/hall.d \
./Src/main.d \
./Src/median.d \
./Src/poweron.d \
./Src/proto.d \
./Src/ramp.d \
./Src/spimbaddr.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/stm32f4xx_swtimer.d \
./Src/swtimer.d \
./Src/system_stm32f4xx.d \
./Src/template.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F411xE '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/include" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/port" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/rtu" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/ascii" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/tcp" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/Components/Common" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/Components/l6398" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/X-NUCLEO-IHM08M1" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/MC_6Step_Lib/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/MotorControl/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/CMSIS/Include"  -O2 -g3 -Wall -fmessage-length=0 -fverbose-asm -save-temps=obj -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


