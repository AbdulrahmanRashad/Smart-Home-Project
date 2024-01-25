/*****************************************************************************/
/* Title        	: 	SERVO Driver									     */
/* File Name    	: 	SERVO_program.c   		                             */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	30/01/2021                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
 ******************************* STD LIB DIRECTIVES ****************************
 ******************************************************************************/
#include "../../../00_LBS/STD_TYPES.h"
#include "../../../00_LBS/BIT_MATH.h"
/******************************************************************************
 ********************************* DIO DIRECTIVES ******************************
 ******************************************************************************/
#include "../../../03_MCAL/01-DIO/V1.0/DIO_interface.h"

/******************************************************************************
 ******************************** TIMER1 DIRECTIVES ****************************
 ******************************************************************************/
#include "../../../03_MCAL/TIMER1/V1/TIMER1_interface.h"

/******************************************************************************
 ****************************** Component DIRECTIVES ***************************
 ******************************************************************************/
#include "SERVO_interface.h"
#include "SERVO_private.h"
#include "SERVO_config.h"


/******************************************************************************
 ***************************** Function Implementation *************************
 ******************************************************************************/

void SERVO_voidInit(void){

	/* set timer 1 prescaler to 8to get 1us tick time
	 * use fast mode with top value at OCRA reg to set the freq of servo
	 * diconnect OC1A PIN ,and generate the PWM on OC1B PIN with non inverting mode
	 * to use OCR1B reg as the ton value 
	 */
	//DIO_voidSetPortDirection(DIO_u8PORTD,0xFF);
	TIMER1_voidInit(TIMER1_FAST_PWM_ICR1_MODE, TIMER1_SCALER_8);
	TIMER1_voidOutputCompareInit(TIMER1_OC1_NON_INVERTING_MODE, TIMER1_OCR1A);
	//TIMER1_voidSetICR1(40000U);  //START 180 THIS IS OVERFLOW VALUE & NEED COMPARE VALUE2
	//for(uint16_t i=0;i<2000;i++){
	//	 TIMER1_voidSetOCR1A(i); // 0 DEG
	//}

	/* 20000us to get 50hz */
	//TIMER1_voidSetOCR1A(2000);
	//_delay_ms(200);
	/* 2000us Ton to get angle zero */
	//TIMER1_voidSetOCR1A(4000); //180
	//_delay_ms(200);
}



void SERVO_voidSetAngle(uint_32  _u8Angle ){

	uint_16 Local_u16Result = (((uint_32)_u8Angle * 1000) / 180) + 1000;
	TIMER1_voidSetOCR1B(Local_u16Result);
}


/******************************************************************************
 ********************************* END OF PROGRAM ******************************
 ******************************************************************************/
