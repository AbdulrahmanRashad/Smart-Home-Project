################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../01_APP/APP_program.c \
../01_APP/main.c 

OBJS += \
./01_APP/APP_program.o \
./01_APP/main.o 

C_DEPS += \
./01_APP/APP_program.d \
./01_APP/main.d 


# Each subdirectory must supply rules for building sources it contributes
01_APP/%.o: ../01_APP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


