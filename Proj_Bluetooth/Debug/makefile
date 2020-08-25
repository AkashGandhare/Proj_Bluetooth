################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/ST/BlueNRG-MS/utils/subdir.mk
-include Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic/subdir.mk
-include Middlewares/ST/BlueNRG-MS/hci/controller/subdir.mk
-include Middlewares/ST/BlueNRG-MS/hci/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include BlueNRG_User/src/subdir.mk
-include BlueNRG-MS/Target/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Proj_Bluetooth.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Proj_Bluetooth.list \

OBJCOPY_BIN += \
Proj_Bluetooth.bin \


# All Target
all: Proj_Bluetooth.elf secondary-outputs

# Tool invocations
Proj_Bluetooth.elf: $(OBJS) $(USER_OBJS) D:\ST_Embedded_Project\gitClones\Proj_Bluetooth\Proj_Bluetooth\STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "Proj_Bluetooth.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\ST_Embedded_Project\gitClones\Proj_Bluetooth\Proj_Bluetooth\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Proj_Bluetooth.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Proj_Bluetooth.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Proj_Bluetooth.list"
	@echo 'Finished building: $@'
	@echo ' '

Proj_Bluetooth.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Proj_Bluetooth.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
