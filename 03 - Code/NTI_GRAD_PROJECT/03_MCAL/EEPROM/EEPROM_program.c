/*
 * EEPROM_program.c
 *
 *  Created on: Oct 23, 2023
 *      Author: Tahoun
 */


/******************************************************************************
******************************* STD LIB DIRECTIVES ****************************
******************************************************************************/
#include "../../00_LBS/STD_TYPES.h"
#include "../../00_LBS/BIT_MATH.h"

/******************************************************************************
****************************** Component DIRECTIVES ***************************
******************************************************************************/
#include "EEPROM_interface.h"
#include "EEPROM_private.h"
#include "EEPROM_confg.h"



// Function to write data to EEPROM
void writeDataToEEPROM(uint16_t address, uint8_t data) {
  eeprom_write_byte((uint8_t*)address, data);
}

// Function to read data from EEPROM
uint8_t readDataFromEEPROM(uint16_t address) {
  return eeprom_read_byte((uint8_t*)address);
}

// Function to write a block of data to EEPROM
void writeDataBlockToEEPROM(uint16_t address, uint8_t* data, uint16_t length) {
  eeprom_write_block(data, (void*)address, length);
}

// Function to read a block of data from EEPROM
void readDataBlockFromEEPROM(uint16_t address, uint8_t* data, uint16_t length) {
  eeprom_read_block(data, (void*)address, length);
}



/******************************************************************************
* !comment  :  Pointer to function for callback.  							  *
******************************************************************************/

static void (*EEPROM_CallBack)(void) = NULL;


/******************************************************************************
***************************** Function Implementation *************************
******************************************************************************/


uint_8 EEPROM_u8ReadByte(uint_16 Copy_u16Address){

	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));

	/* Set up address register */
	EEAR = Copy_u16Address;

	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);

	/* Return data from data register */
	return EEDR;

}


void EEPROM_voidReadeBlock(uint_16 Copy_u16Address, uint_8 *Ptr_u8Return, uint_16 Copy_u16Size){

	uint_16 Local_u16ByteCounter = 0;

	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));

	while(Local_u16ByteCounter < Copy_u16Size){

		/* Set up address register */
		EEAR = Copy_u16Address + Local_u16ByteCounter;

		/* Start eeprom read by writing EERE */
		EECR |= (1<<EERE);

		/* move data form data register to the array */
		*(Ptr_u8Return + Local_u16ByteCounter) = EEDR;

		/* increase the bytes counter */
		Local_u16ByteCounter++;

	}

}


void EEPROM_voidWriteByte(uint_16 Copy_u16Address, uint_8 Copy_u8Data){

	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));

	/* Set up address register */
	EEAR = Copy_u16Address;

	/* Read the byte in the address of EEAR */
	EECR |= (1<<EERE);

	/* Compare the value read to value to be written */
	if(EEDR != Copy_u8Data){

		/* if they are not equal then write the data */
		/* Set up data registers */
		EEDR = Copy_u8Data;

		/* Write logical one to EEMWE */
		EECR |= (1<<EEMWE);

		/* Start eeprom write by setting EEWE */
		EECR |= (1<<EEWE);
	}

	else{    /* return error */    }

}




void EEPROM_voidWriteBlock(uint_16 Copy_u16Address, uint_8 *Copy_u8Data, uint_16 Copy_u16Size){

	uint_16 Local_u16ByteCounter = 0;

	while(Local_u16ByteCounter < Copy_u16Size){

		/* Wait for completion of previous write */
		while(EECR & (1<<EEWE));

		/* Set up address register */
		EEAR = Copy_u16Address + Local_u16ByteCounter;

		/* Read the byte in the address of EEAR */
		EECR |= (1<<EERE);

		/* Compare the value read to value to be written */
		if(EEDR != (*(Copy_u8Data + Local_u16ByteCounter))){

			/* if they are not equal then write the data */
			/* Set up data registers */
			EEDR = *(Copy_u8Data + Local_u16ByteCounter);

			/* Write logical one to EEMWE */
			EECR |= (1<<EEMWE);

			/* Start eeprom write by setting EEWE */
			EECR |= (1<<EEWE);

		}

		else{    /* return error */    }

		/* increase the bytes counter */
		Local_u16ByteCounter++;
	}

}



void EEPROM_voidReadyInterruptEnable(void){

	SET_BIT(EECR, EERIE);
}



void EEPROM_voidReadyInterruptDisable(void){

	CLR_BIT(EECR, EERIE);
}



/******************************************************************************
* Description 	: This function used to	communicate with the function in App  *
*                 Layer(The Callback function EEPROM) -                       *
*		          [ Layer Architecture Problem Solved ].                      *
******************************************************************************/

void EEPROM_voidSetCallBack(void(*Local_ptr)(void)){

	EEPROM_CallBack = Local_ptr;

}


/******************************************************************************
* !comment  :  ISR Function EEPROM.  							 			  *
******************************************************************************/

ISR(EEPROM_RDY_VECT){

	if(EEPROM_CallBack != NULL){

		EEPROM_CallBack();
	}

	else{ /* Return error */ }
}


/******************************************************************************
********************************* END OF PROGRAM ******************************
******************************************************************************/
