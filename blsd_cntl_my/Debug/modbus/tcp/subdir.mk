################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus/tcp/mbtcp.c 

OBJS += \
./modbus/tcp/mbtcp.o 

C_DEPS += \
./modbus/tcp/mbtcp.d 


# Each subdirectory must supply rules for building sources it contributes
modbus/tcp/%.o: ../modbus/tcp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F411xE '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/include" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/port" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/rtu" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/ascii" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/modbus/tcp" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/Components/Common" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/Components/l6398" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/BSP/X-NUCLEO-IHM08M1" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/MC_6Step_Lib/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/motor/MotorControl/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Keil/blsd_cntl_my/blsd_cntl_my/Drivers/CMSIS/Include"  -O2 -g3 -Wall -fmessage-length=0 -fverbose-asm -save-temps=obj -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


