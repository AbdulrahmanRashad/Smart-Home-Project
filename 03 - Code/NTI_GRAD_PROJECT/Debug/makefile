################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include 03_MCAL/UART/subdir.mk
-include 03_MCAL/TIMER1/V1/subdir.mk
-include 03_MCAL/TIMER0/V1/subdir.mk
-include 03_MCAL/EEPROM/subdir.mk
-include 03_MCAL/ADC/V1.0/subdir.mk
-include 03_MCAL/01_EXIT_INTRRUPT/subdir.mk
-include 03_MCAL/01-DIO/V1.0/subdir.mk
-include 02_HAL/TEMP/V1/subdir.mk
-include 02_HAL/SERVO/V1/subdir.mk
-include 02_HAL/MOTOR/subdir.mk
-include 02_HAL/LED/V1.0/subdir.mk
-include 02_HAL/KEYPAD/V1.0/subdir.mk
-include 02_HAL/CLCD/V1.0/subdir.mk
-include 02_HAL/BUZZER/V1/subdir.mk
-include 02_HAL/BLUETOOTH/V1/subdir.mk
-include 01_APP/subdir.mk
-include 00_LBS/CALLBACK/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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
LSS += \
NTI_GRAD_PROJECT.lss \

FLASH_IMAGE += \
NTI_GRAD_PROJECT.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: NTI_GRAD_PROJECT.elf secondary-outputs

# Tool invocations
NTI_GRAD_PROJECT.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,NTI_GRAD_PROJECT.map -mmcu=atmega32 -o "NTI_GRAD_PROJECT.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

NTI_GRAD_PROJECT.lss: NTI_GRAD_PROJECT.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S NTI_GRAD_PROJECT.elf  >"NTI_GRAD_PROJECT.lss"
	@echo 'Finished building: $@'
	@echo ' '

NTI_GRAD_PROJECT.hex: NTI_GRAD_PROJECT.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex NTI_GRAD_PROJECT.elf  "NTI_GRAD_PROJECT.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: NTI_GRAD_PROJECT.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 NTI_GRAD_PROJECT.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) NTI_GRAD_PROJECT.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
