/*
 * MOTOR_interface.h
 *
 *  Created on: Oct 16, 2023
 *      Author: Tahoun
 */

#ifndef MOTOR_INTERFACE_H_
#define MOTOR_INTERFACE_H_


/*-------------- Includes ---------------*/
#include "../../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../MOTOR/MOTOR_confg.h"

/*-------------- Macro Declarations ---------------*/


/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/
typedef enum{
	FIRST_PIN,
	SECOND_PIN
}Motor_Pins;

typedef enum{
	MotorStop,
	MotorForward,
	MotorReverse
}Motor_State;

/*-------------- Software Interfaces Declarations "APIs" ---------------*/
void Motor_void_Intilize(void);
void Motor_void_ck(void);
void Motor_void_stop(void);
void Ecual_DC_Motor_Intialize(const uint_8 copy_u8Port, const uint_8 copy_u8PinsNum[2], const uint_8 copy_u8State);
void Ecual_DC_Motor_Forward(const uint_8 copy_u8PortNum, const uint_8 copy_u8PinsNum[2]);
void Ecual_DC_Motor_Reverse(const uint_8 copy_u8PortNum, const uint_8 copy_u8PinsNum[2]);
void Ecual_DC_Motor_Stop(const uint_8 copy_u8PortNum, const uint_8 copy_u8PinsNum[2]);
Motor_State Ecual_DC_Motor_Status(const uint_8 copy_u8PortNum, const uint_8 copy_u8PinsNum[2]);


/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/



#endif /* MOTOR_INTERFACE_H_ */
