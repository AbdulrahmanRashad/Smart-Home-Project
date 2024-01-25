/*****************************************************************************/
/* Title        	: 	UART Driver									         */
/* File Name    	: 	UartInterface.h   		                             */
/* Author       	: 	Ahmed Ashraf                                         */
/* Version      	: 	1.0.0                                                */
/*****************************************************************************/
#ifndef UART_UARTINTERFACE_H_
#define UART_UARTINTERFACE_H_



#include "UartPrivate.h"
#include "UartConfig.h"
/******************************************************************************
* Description 	: UART initialization Function.				                  *
* Parameters  	: none   		                                    	 	  *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : none.                                                       *
******************************************************************************/

void UART_voidInit(void);

/******************************************************************************
* Description 	: UART Receive Byte Function.		                          *
* Parameters  	: none    		                  	 	                      *
* Return type 	: uint8                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function.                                                   *
******************************************************************************/

uint_8 UART_u8ReceiveByte(void);

/******************************************************************************
* Description 	: UART Send Char Blocking Function.		                      *
* Parameters  	: uint8 Data    		                  	 	              *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function.                                                   *
******************************************************************************/

void UART_voidSendByte(uint_8 Copy_u8Data);

/******************************************************************************
* Description 	: UART Send String Blocking Function.		                  *
* Parameters  	: uint8 Data    		                  	 	              *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function.                                                   *
******************************************************************************/

void UART_voidSendStringBlocking(uint_8 *Copy_u8Data);

/******************************************************************************
* Description 	: UART Write Decimal Number Blocking Function.		          *
* Parameters  	: uint32 Number    		                  	 	              *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function.                                                   *
******************************************************************************/

void UART_voidWriteDecimalNumber(uint_32 Copy_u32Number);

/******************************************************************************
* Description 	: UART Write Hexadecimal Number Blocking Function.		      *
* Parameters  	: uint32 Number    		                  	 	              *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function.                                                   *
******************************************************************************/

void UART_voidWriteHexadecimalNumber(uint_32 Copy_u32DecimalNumber);

/******************************************************************************
* Description 	: UART Send Byte Function.		                              *
* Parameters  	: uint8 Data    		                  	 	              *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function & UART TX Interrupt Enable Function.               *
******************************************************************************/

void UART_voidSendByteNoBlock(uint_8 Copy_u8Data);

/******************************************************************************
* Description 	: UART Receive Byte Function.		                          *
* Parameters  	: none    		                  	 	                      *
* Return type 	: uint8                                                  	  *
*******************************************************************************
* Pre_condition : This function must be used after UART initialization        *
*                 Function & UART RX Interrupt Enable Function.               *
******************************************************************************/

uint_8 UART_u8ReceiveByteNoBlock(void);

/******************************************************************************
* Description 	: UART RX Interrupt Enable Function.				          *
* Parameters  	: none    		                  	 	                      *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : none                                                        *
******************************************************************************/

void UART_voidEnableInterrupt_RX(void);

/******************************************************************************
* Description 	: UART RX Interrupt Disable Function.				          *
* Parameters  	: none    		                  	 	                      *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : none                                                        *
******************************************************************************/

void UART_voidDisableInterrupt_RX(void);

/******************************************************************************
* Description 	: UART TX Interrupt Enable Function.				          *
* Parameters  	: none    		                  	 	                      *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : none                                                        *
******************************************************************************/

void UART_voidEnableInterrupt_TX(void);

/******************************************************************************
* Description 	: UART TX Interrupt Disable Function.				          *
* Parameters  	: none    		                  	 	                      *
* Return type 	: void                                                  	  *
*******************************************************************************
* Pre_condition : none                                                        *
******************************************************************************/

void UART_voidDisableInterrupt_TX(void);

/******************************************************************************
* Description 	: This function used to	communicate with the function in App  *
*                 Layer(The Callback function UART TX) -                      *
*		          [ Layer Architecture Problem Solved ].                      *
* Parameters  	: Pointer to function that points to the first line           *
*				  of the function (ISR).                                      *
* Return type 	: void.                                                    	  *
*******************************************************************************
* Pre_condition : This function must be used after UART TX Interrupt Enable   *
*				  Function.                                         		  *
******************************************************************************/

void UART_voidSetCallBack_TX(void(*Local_ptr)(void));

/******************************************************************************
* Description 	: This function used to	communicate with the function in App  *
*                 Layer(The Callback function UART RX) -                      *
*		          [ Layer Architecture Problem Solved ].                      *
* Parameters  	: Pointer to function that points to the first line           *
*				  of the function (ISR).                                      *
* Return type 	: void.                                                    	  *
*******************************************************************************
* Pre_condition : This function must be used after UART RX Interrupt Enable   *
*				  Function.                                         		  *
******************************************************************************/

void UART_voidSetCallBack_RX(void(*Local_ptr)(void));



#endif /* UART_UARTINTERFACE_H_ */
