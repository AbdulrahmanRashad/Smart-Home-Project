################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../03_MCAL/01_EXIT_INTRRUPT/Exit_Program.c 

OBJS += \
./03_MCAL/01_EXIT_INTRRUPT/Exit_Program.o 

C_DEPS += \
./03_MCAL/01_EXIT_INTRRUPT/Exit_Program.d 


# Each subdirectory must supply rules for building sources it contributes
03_MCAL/01_EXIT_INTRRUPT/%.o: ../03_MCAL/01_EXIT_INTRRUPT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


