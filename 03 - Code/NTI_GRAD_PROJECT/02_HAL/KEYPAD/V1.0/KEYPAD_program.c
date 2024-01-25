/*
 * Keypad_program.c
 *
 *  Created on: Oct 11, 2023
 *      Author: Tahoun
 */


/*-------------- Includes ---------------*/
#include "../../../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../../KEYPAD/V1.0/KEYPAD_interface.h"

/*-------------- Helper Functions Declarations ---------------*/

/*-------------- Global Variable Definitions  ---------------*/


/*-------------- APIs Definitions ---------------*/
void Keypad_voidInit(void)
{
	DIO_voidSetPortDirection(KEYPAD_U8PORT, 0X0F);
	DIO_voidSetPortValue(KEYPAD_U8PORT, 0XFF);
}

uint_8 Keypad_u8GetPressed(const uint_8 Copy_u8Keypad[KEYPAD_ROW][KEYPAD_COL])
{

	uint_8 Local_u8Key = '+', Local_u8Row = 0, Local_u8Col = 0;

	for(Local_u8Row = 0; Local_u8Row < KEYPAD_ROW; Local_u8Row++){
		DIO_voidSetPinValue(KEYPAD_U8PORT, Local_u8Row, Low);

		for(Local_u8Col = 0; Local_u8Col < KEYPAD_COL; Local_u8Col++){
			if(0 == DIO_u8GetPinValue(KEYPAD_U8PORT, Local_u8Col+KEYPAD_ROW)){
				Local_u8Key = Copy_u8Keypad[Local_u8Row][Local_u8Col];
				//break;
			/* If you use this line ( break; ) here instead of ( while(0 == DIO_u8GetPinLogic(KEYPAD_U8PORT, Local_u8Col)); ),
			   the function will execute right and it will return the right number but you should know that the function is
			   used in while(1) in the (main function) so if you press on the button, the function will execute more than once
			   because of the high speed of the processor. So if you want to use (break) you have to change the main function */
			}
			while(0 == DIO_u8GetPinValue(KEYPAD_U8PORT, Local_u8Col+KEYPAD_ROW));
			/* But here there are no return from the function (it will stuck) until you release the pressed button */
		}

		DIO_voidSetPinValue(KEYPAD_U8PORT, Local_u8Row, High);
	}

	return Local_u8Key;
}

/*-------------- ISRs Definitions ---------------*/

/*-------------- Helper or Static Functions Definitions ---------------*/
