################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../02_HAL/SERVO/V1/SERVO_program.c 

OBJS += \
./02_HAL/SERVO/V1/SERVO_program.o 

C_DEPS += \
./02_HAL/SERVO/V1/SERVO_program.d 


# Each subdirectory must supply rules for building sources it contributes
02_HAL/SERVO/V1/%.o: ../02_HAL/SERVO/V1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


