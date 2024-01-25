/*
 * keypad_interface.h
 *
 *  Created on: Oct 11, 2023
 *      Author: Tahoun
 */

#ifndef KEYPAD_INTERFACE_H_
#define KEYPAD_INTERFACE_H_


/*-------------- Includes ---------------*/
#include "../../../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../../KEYPAD/V1.0/KEYPAD_confg.h"

/*-------------- Macro Declarations ---------------*/


/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/

/*-------------- Software Interfaces Declarations "APIs" ---------------*/
void Keypad_voidInit(void);
uint_8 Keypad_u8GetPressed(const uint_8 Copy_u8Keypad[KEYPAD_ROW][KEYPAD_COL]);

/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/








#endif /* KEYPAD_INTERFACE_H_ */
