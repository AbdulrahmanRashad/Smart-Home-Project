/*****************************************************************************/
/* Title        	: 	UART Driver									         */
/* File Name    	: 	UartConfig.h   		                             */
/* Author       	: 	Ahmed Ashraf                                         */
/* Version      	: 	1.0.0                                                */
/*****************************************************************************/
#ifndef UART_UARTCONFIG_H_
#define UART_UARTCONFIG_H_

#define BAUD_RATE_VALUE     51

/******************************************************************************
* !comment  :  Select UART Mode Option:	  	      					          *
*   		   ASYNCHRONOUS    									              *
*			   SYNCHRONOUS                                                    *
******************************************************************************/

#define UART_MODE    ASYNCHRONOUS

/******************************************************************************
* !comment  :  Select Parity Mode Option:	  	      					      *
*   		   DISABLE    									                  *
*			   EVEN_PARITY                                                    *
*			   ODD_PARITY                                                     *
******************************************************************************/

#define PARITY_MODE    DISABLE

/******************************************************************************
* !comment  :  Select Stop Bit Option:	  	      					          *
*   		   ONE_BIT    									                  *
*			   TWO_BIT                                                        *
******************************************************************************/

#define STOP_BIT    ONE_BIT

/******************************************************************************
* !comment  :  Select Data Size Option:	  	      					          *
*   		   FIVE_BIT    									                  *
*			   SIX_BIT                                                        *
*   		   SEVEN_BIT    									              *
*			   EIGHT_BIT                                                      *
*   		   NINE_BIT    									                  *
******************************************************************************/

#define DATA_SIZE    EIGHT_BIT


#endif /* 03_MCAL_UART_UARTCONFIG_H_ */

