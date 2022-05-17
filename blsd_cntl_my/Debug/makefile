################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include motor/MotorControl/Src/subdir.mk
-include motor/MC_6Step_Lib/Src/subdir.mk
-include motor/BSP/X-NUCLEO-IHM08M1/subdir.mk
-include motor/BSP/Components/l6398/subdir.mk
-include modbus/tcp/subdir.mk
-include modbus/rtu/subdir.mk
-include modbus/port/subdir.mk
-include modbus/functions/subdir.mk
-include modbus/ascii/subdir.mk
-include modbus/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: blsd_cntl_my.elf

# Tool invocations
blsd_cntl_my.elf: $(OBJS) $(USER_OBJS) ../STM32F411RETx_FLASH.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -specs=nano.specs -T"../STM32F411RETx_FLASH.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "blsd_cntl_my.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating hex and Printing size information:'
	arm-none-eabi-objcopy -O ihex "blsd_cntl_my.elf" "blsd_cntl_my.hex"
	arm-none-eabi-size "blsd_cntl_my.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
