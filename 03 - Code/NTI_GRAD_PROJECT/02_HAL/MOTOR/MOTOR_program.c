/*
 * Motor.c
 *
 *  Created on: Oct 16, 2023
 *      Author: Tahoun
 */



/*-------------- Includes ---------------*/
#include "../MOTOR/MOTOR_interface.h"
#define MOTOR1_1  DIO_PIN1
/*#define MOTOR1_2  PIN1
#define MOTOR2_1  PIN2
#define MOTOR2_2  PIN3*/
/*-------------- Helper Functions Declarations ---------------*/

/*-------------- Global Variable Definitions  ---------------*/

/*-------------- APIs Definitions ---------------*/

void Motor_void_Intilize(void){
	DIO_voidSetPinDirection(MOTOR_PORT,MOTOR1_1,Output);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR1_1,Low);
	//DIO_voidSetPinDirection(MOTOR_PORT,MOTOR1_2,Output);
	//DIO_voidSetPinDirection(MOTOR_PORT,MOTOR2_1,Output);
	//DIO_voidSetPinDirection(MOTOR_PORT,MOTOR2_2,Output);
}
void Motor_void_ck(void){
	DIO_voidSetPinValue(MOTOR_PORT,MOTOR1_1,High);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR1_2,Low);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR2_1,High);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR2_2,Low);
}
void Motor_void_stop(void){
	DIO_voidSetPinValue(MOTOR_PORT,MOTOR1_1,Low);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR1_2,Low);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR2_1,Low);
	//DIO_voidSetPinValue(MOTOR_PORT,MOTOR2_2,Low);
}
void Ecual_DC_Motor_Intialize(const uint_8 copy_u8Port, const uint_8 copy_u8PinsNum[2], const uint_8 copy_u8State){
	DIO_voidSetPinDirection(copy_u8Port, copy_u8PinsNum[FIRST_PIN], copy_u8State);
	//DIO_voidSetPinDirection(copy_u8Port, copy_u8PinsNum[SECOND_PIN], Output);
}
void Ecual_DC_Motor_Forward(const uint_8 copy_u8Port, const uint_8 copy_u8PinsNum[2]){
	DIO_voidSetPinValue(copy_u8Port, copy_u8PinsNum[FIRST_PIN], MOTOR_ON);
	//DIO_voidSetPinValue(copy_u8Port, copy_u8PinsNum[SECOND_PIN], MOTOR_OFF);
}
void Ecual_DC_Motor_Reverse(const uint_8 copy_u8Port, const uint_8 copy_u8PinsNum[2]){
	DIO_voidSetPinValue(copy_u8Port, copy_u8PinsNum[FIRST_PIN], MOTOR_OFF);
	DIO_voidSetPinValue(copy_u8Port, copy_u8PinsNum[SECOND_PIN], MOTOR_ON);
}
void Ecual_DC_Motor_Stop(const uint_8 copy_u8Port, const uint_8 copy_u8PinsNum[2]){
	DIO_voidSetPinValue(copy_u8Port, copy_u8PinsNum[FIRST_PIN], MOTOR_OFF);
	DIO_voidSetPinValue(copy_u8Port, copy_u8PinsNum[SECOND_PIN], MOTOR_OFF);
}

Motor_State Ecual_DC_Motor_Status(const uint_8 copy_u8PortNum, const uint_8 copy_u8PinsNum[2]){

	Motor_State Local_u8State = MotorStop;
	if(DIO_u8GetPinValue(copy_u8PortNum, copy_u8PinsNum[FIRST_PIN]) != DIO_u8GetPinValue(copy_u8PortNum, copy_u8PinsNum[SECOND_PIN]))
	{
		if(MOTOR_ON == DIO_u8GetPinValue(copy_u8PortNum, copy_u8PinsNum[FIRST_PIN])){
			Local_u8State = MotorForward;
		}
		else{
			Local_u8State = MotorReverse;
		}
	}
	else{
		Local_u8State = MotorStop;
	}

	return Local_u8State;
}
/*-------------- ISRs Definitions ---------------*/

/*-------------- Helper or Static Functions Definitions ---------------*/
