/*
 * CLCD_program.c
 *
 *  Created on: Oct 10, 2023
 *      Author: Tahoun
 */

/*-------------- Includes ---------------*/
#include "../../CLCD/V1.0/CLCD_interface.h"

/*-------------- Helper Functions Declarations ---------------*/

/*-------------- Global Variable Definitions  ---------------*/

/*-------------- APIs Definitions ---------------*/
void CLCD_voidInit(void)
{
	DIO_voidSetPortDirection(CLCD_DATA_PORT, 0XFF);
	DIO_voidSetPinDirection(CLCD_CONTROL_PORT, CLCD_RS_PIN, Output);
	DIO_voidSetPinDirection(CLCD_CONTROL_PORT, CLCD_RW_PIN, Output);
	DIO_voidSetPinDirection(CLCD_CONTROL_PORT, CLCD_EN_PIN, Output);

	_delay_ms(50);
	CLCD_voidSendCommand(CLCD_FUNCTION_SET);
	CLCD_voidSendCommand(0X0C);
	CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
	_delay_ms(1);
	CLCD_voidSendCommand(0X06);
}

void CLCD_voidSendCommand(uint_8 Copy_u8Command){
	DIO_voidSetPinValue(CLCD_CONTROL_PORT, CLCD_RS_PIN, Low);
	DIO_voidSetPortValue(CLCD_DATA_PORT, Copy_u8Command); //Function Set
	DIO_voidSetPinValue(CLCD_CONTROL_PORT, CLCD_EN_PIN, High);
	_delay_ms(1);
	DIO_voidSetPinValue(CLCD_CONTROL_PORT, CLCD_EN_PIN, Low);
	_delay_ms(1);
}

void CLCD_voidWriteData(uint_8 Copy_u8Data){
	DIO_voidSetPinValue(CLCD_CONTROL_PORT, CLCD_RS_PIN, High); //Send Data
	DIO_voidSetPortValue(CLCD_DATA_PORT, Copy_u8Data); //Data
	DIO_voidSetPinValue(CLCD_CONTROL_PORT, CLCD_EN_PIN, High);
	_delay_ms(1);
	DIO_voidSetPinValue(CLCD_CONTROL_PORT, CLCD_EN_PIN, Low);
	_delay_ms(1);
}

void CLCD_voidWriteString(char String[]){
	uint_16 Counter = 0;
	for(Counter = 0; Counter < strlen(String); Counter++)
	{
		CLCD_voidWriteData(String[Counter]);
	}
}


void CLCD_voidGoto(uint_8 Copy_u8Row, uint_8 Copy_u8Coloum){
	Copy_u8Coloum--;
	switch(Copy_u8Row)
	{
		case 1 : CLCD_voidSendCommand(0X80+Copy_u8Coloum); break;
		case 2 : CLCD_voidSendCommand(0XC0+Copy_u8Coloum); break;
		case 3 : CLCD_voidSendCommand(0x90+Copy_u8Coloum); break;
		case 4 : CLCD_voidSendCommand(0xD0+Copy_u8Coloum); break;
		default : /* Error */ break;
	}
}

void CLCD_voidWriteNumbers(uint_32 Copy_u32Number){
	int remainder = 0, reverse = 1;
	while (Copy_u32Number != 0) {
	    remainder = Copy_u32Number % 10;
	    reverse = reverse * 10 + remainder;
	    Copy_u32Number /= 10;

	}
	if(reverse == 1)
	{
			CLCD_voidWriteData(48);
	}
	while (reverse != 1){
	CLCD_voidWriteData((reverse%10)+48);
	reverse /=10;
	}

}

void CLCD_voidWriteNumbersInBinary(uint_32 Copy_u32Number){

	sint_8 Counter = 0;
	CLCD_voidWriteString("0b");
	for(Counter = 7; Counter <= 0; Counter--)
	{
		CLCD_voidWriteData(GET_BIT(Copy_u32Number, Counter)?'1':'0');
	}

  /*uint32 Local_u32Num = 10;
	CLCD_voidWriteString("0b");

	while(2 != Local_u32Num){

		if(0 == Copy_u32Number%2){
			CLCD_voidGoto(1,Local_u32Num);
			CLCD_voidWriteNumbers(0);
			Copy_u32Number = Copy_u32Number/2;
			Local_u32Num--;
		}
		else{
			CLCD_voidGoto(1,Local_u32Num);
			CLCD_voidWriteNumbers(1);
			Copy_u32Number = Copy_u32Number/2;
			Local_u32Num--;
		}
	}*/

}

/*
void CLCD_WriteNumbersInHexa(uint32 Copy_u32Number){
	uint32 Local_u32Num = 4, Remainder = 0;
	CLCD_voidWriteString("0X");

	while(1 != Local_u32Num){
		Remainder = Copy_u32Number%16;

		if(0 == Remainder){
			CLCD_voidGoto(1,Local_u32Num);
			CLCD_voidWriteNumbers(0);
			Copy_u32Number = Copy_u32Number/16;
			Local_u32Num--;
		}
		else{
			Remainder *= 16;
			CLCD_voidGoto(1,Local_u32Num);
			CLCD_voidWriteNumbers(Remainder);
			Copy_u32Number = Copy_u32Number/16;
			Local_u32Num--;
		}
	}

}*/

void CLCD_voidWriteNumbersInHexa(uint_32 Copy_u32Number){
	uint_8 Upper = 0, Lower = 0;
	CLCD_voidWriteString("0X");

	Upper = Copy_u32Number >> 4; //0b0000 0010
	Lower = Copy_u32Number & 0X0F; //0b0000 0000

	if(Upper < 10){
		CLCD_voidGoto(1,3);
		CLCD_voidWriteData(Upper+'0');
	}
	else{
		CLCD_voidGoto(1,3);
		CLCD_voidWriteData(Upper+'A'-10);
	}

	if(Lower < 10){
		CLCD_voidGoto(1,4);
		CLCD_voidWriteData(Lower+'0');
	}
	else{
		CLCD_voidGoto(1,4);
		CLCD_voidWriteData(Lower+'A'-10);
	}
}

void CLCD_voidStoreCustomChar(uint_8 Copy_u8CharAddress, uint_8 customChar[8]){
	uint_8 Counter = 0;

		CLCD_voidSendCommand(0X40 + 0X08*Copy_u8CharAddress);

		for(Counter = 0; Counter < 8; Counter++){
			CLCD_voidWriteData(customChar[Counter]);
		}

}
void CLCD_voidWriteCustomChar(uint_8 Copy_u8CharAddress, uint_8 customChar[8], uint_8 copy_u8Line){
	uint_8 Counter = 0;

	CLCD_voidSendCommand(0X40 + 0X08*Copy_u8CharAddress);

	for(Counter = 0; Counter < 8; Counter++){
		CLCD_voidWriteData(customChar[Counter]);
	}

	CLCD_voidGoto(copy_u8Line,16-Copy_u8CharAddress);
	CLCD_voidWriteData(Copy_u8CharAddress);

}

void CLCD_voidWriteArabicNames(uint_8 Copy_u8Name[][8], uint_8 copy_u8CharNumbers, uint_8 copy_u8Line)
{
	uint_8 Counter = 0;
	for(Counter = 0; Counter < copy_u8CharNumbers; Counter++)
	{
		CLCD_voidWriteCustomChar(Counter, Copy_u8Name[Counter], copy_u8Line);
	}

}

/*-------------- ISRs Definitions ---------------*/

/*-------------- Helper or Static Functions Definitions ---------------*/

