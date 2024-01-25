/*****************************************************************************/
/* Title        	: 	BLUETOOTH Driver									 */
/* File Name    	: 	BLUETOOTH_interface.h   		                     */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	03/02/2021                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
* Description 	: Guard to protect this File from include more 			      *
*                 than one time.                               	 	 		  *
******************************************************************************/

#ifndef BLUETOOTH_INTERFACE_H
#define BLUETOOTH_INTERFACE_H


/******************************************************************************
****************************** Functions Prototypes ***************************
******************************************************************************/

void BLUETOOTH_voidInit(void);
uint_8 BLUETOOTH_u8Receive(void);
void BLUETOOTH_voidSendChar(uint_8 Copy_u8Data);
void BLUETOOTH_voidSendString(uint_8 *Copy_u8Data);
void BLUETOOTH_voidWriteNumber(uint_32 Copy_u32Number);
unsigned char UART_ReceiveCharfrist();
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU     8000000UL
#define BAUD_RATE 9600
#define DATA_LENGTH2 10


#endif 
/*** !comment : End of gaurd [BLUETOOTH_INTERFACE_H] *************************/
