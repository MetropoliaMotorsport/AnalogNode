################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
motorsport_AnalogNode.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
motorsport_AnalogNode.list \


# All Target
all: motorsport_AnalogNode.elf secondary-outputs

# Tool invocations
motorsport_AnalogNode.elf: $(OBJS) $(USER_OBJS) C:\Users\primary\Desktop\Programming\stm32s\motorsport_AnalogNdoe\STM32G441KBTX_FLASH.ld
	arm-none-eabi-gcc -o "motorsport_AnalogNode.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\primary\Desktop\Programming\stm32s\motorsport_AnalogNdoe\STM32G441KBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="motorsport_AnalogNode.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

motorsport_AnalogNode.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "motorsport_AnalogNode.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
