/*****************************************************************************/
/* Title        	: 	UART Driver									         */
/* File Name    	: 	UartPrivate.h   		                             */
/* Author       	: 	Ahmed Ashraf                                         */
/* Version      	: 	1.0.0                                                */
/*****************************************************************************/
#ifndef UART_UARTPRIVATE_H_
#define UART_UARTPRIVATE_H_

/******************************************************************************
* !comment : UART Registers Addresses Definition.  			         	      *
******************************************************************************/

#define UDR       *((volatile uint_8 *) 0x2C)
#define UCSRA     *((volatile uint_8 *) 0x2B)
#define UCSRB     *((volatile uint_8 *) 0x2A)
#define UBRRL     *((volatile uint_8 *) 0x29)
#define UCSRC     *((volatile uint_8 *) 0x40)
#define UBRRH     *((volatile uint_8 *) 0x40)


/******************************************************************************
* !comment : Control and Status Register-A (UCSRA) PINS Definition.           *
******************************************************************************/

#define MPCM     (uint_8)0
#define U2X      (uint_8)1
#define PE       (uint_8)2
#define DOR      (uint_8)3
#define FE       (uint_8)4
#define UDRE     (uint_8)5
#define TXC      (uint_8)6
#define RXC      (uint_8)7

/******************************************************************************
* !comment : Control and Status Register-B (UCSRB) PINS Definition.       	  *
******************************************************************************/

#define TXB8     0
#define RXB8     1
#define UCSZ2    2
#define TXEN     3
#define RXEN     4
#define UDRIE    5
#define TXCIE    6
#define RXCIE    7

/******************************************************************************
* !comment : Control and Status Register-C (UCSRC) PINS Definition.       	  *
******************************************************************************/

#define UCPOL    (uint_8)0
#define UCSZ0    (uint_8)1
#define UCSZ1    (uint_8)2
#define USBS     (uint_8)3
#define UPM0     (uint_8)4
#define UPM1     (uint_8)5
#define UMSEL    (uint_8)6
#define URSEL    (uint_8)7

/******************************************************************************
* !comment : Baud Rate Registers (UBRRL) and (UBRRH) PINS Definition.         *
******************************************************************************/

#define URSEL2   (uint8)15

/******************************************************************************
* !comment : UART Mode Option Definition.       	                          *
******************************************************************************/

#define ASYNCHRONOUS	0
#define SYNCHRONOUS		1

/******************************************************************************
* !comment : Parity Mode Option Definition.       	                          *
******************************************************************************/

#define DISABLE         0
#define EVEN_PARITY     1
#define ODD_PARITY      2

/******************************************************************************
* !comment : Stop Bit Option Definition.       	                              *
******************************************************************************/

#define ONE_BIT  	    0
#define TWO_BIT         1

/******************************************************************************
* !comment : Data Size Option Definition.       	                          *
******************************************************************************/

#define FIVE_BIT        5
#define SIX_BIT         6
#define SEVEN_BIT       7
#define EIGHT_BIT       8
#define NINE_BIT        9

/******************************************************************************
* !comment : UART ISR function prototype.  			             	          *
******************************************************************************/

#define ISR(vector, ...)\
void vector (void) __attribute__((signal));\
void vector (void)

/******************************************************************************
* !comment : UART Interrupt Vectors Definition.  			                  *
******************************************************************************/

#define USART_TX     __vector_15
#define USART_RX     __vector_13



#endif /* 03_MCAL_UART_UARTPRIVATE_H_ */

