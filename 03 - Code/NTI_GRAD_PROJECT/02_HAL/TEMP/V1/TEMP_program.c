/*****************************************************************************/
/* Title        	: 	Temperature Sensor Driver							 */
/* File Name    	: 	TEMP_program.c   		                             */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	05/01/2021                                           */
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
********************************** ADC DIRECTIVES *****************************
******************************************************************************/
#include "../../../03_MCAL/ADC/V1.0/ADC_interface.h"

/******************************************************************************
****************************** Component DIRECTIVES ***************************
******************************************************************************/
#include "TEMP_interface.h"
#include "TEMP_private.h"
#include "TEMP_config.h"


/******************************************************************************
***************************** Function Implementation *************************
******************************************************************************/

void TEMP_voidInit(void){

	ADC_voidInit();

}



uint_16 TEMP_u16GetValue(uint_8 Copy_u8ChannelNumber){

	uint_16 Local_u16AnalogValue = 0;
	uint_16 Local_u16Temperature = 0;

	Local_u16AnalogValue = ADC_u16ReadValue(Copy_u8ChannelNumber);

	Local_u16Temperature = (Local_u16AnalogValue*150*5)/(1023*1.5);

	return Local_u16Temperature;

}


/******************************************************************************
********************************* END OF PROGRAM ******************************
******************************************************************************/
