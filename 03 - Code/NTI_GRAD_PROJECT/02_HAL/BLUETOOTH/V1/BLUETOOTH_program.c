/*****************************************************************************/
/* Title        	: 	BLUETOOTH Driver									 */
/* File Name    	: 	BLUETOOTH_program.c   		                         */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	03/02/2021                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
******************************* STD LIB DIRECTIVES ****************************
******************************************************************************/
#include "../../../00_LBS/BIT_MATH.h"
#include "../../../00_LBS/STD_TYPES.h"

/******************************************************************************
********************************* DIO DIRECTIVES ******************************
******************************************************************************/
#include "../../../03_MCAL/01-DIO/V1.0/DIO_interface.h"

/******************************************************************************
********************************* UART DIRECTIVES *****************************
******************************************************************************/
#include "../../../03_MCAL/UART/UartInterface.h"
/******************************************************************************
****************************** Component DIRECTIVES ***************************
******************************************************************************/
#include "BLUETOOTH_interface.h"
#include "BLUETOOTH_private.h"
#include "BLUETOOTH_config.h"


/******************************************************************************
***************************** Function Implementation *************************
******************************************************************************/

void BLUETOOTH_voidInit(void){

	/* RX Set the direction input for the Pin connected to the bluetooth */
	DIO_voidSetPinDirection(DIO_u8PORTD, DIO_PIN0, Input);
	
	/* TX Set the direction output for the Pin connected to the bluetooth */
	DIO_voidSetPinDirection(DIO_u8PORTD, DIO_PIN1, Output);

	/* init uart protocol */
	UART_voidInit();

}



uint_8 BLUETOOTH_u8Receive(void){

	/* Receive data char */
	return UART_u8ReceiveByte();

}



void BLUETOOTH_voidSendChar(uint_8 Copy_u8Data){

	/* Send data char */
	UART_voidSendByte(Copy_u8Data);

}



void BLUETOOTH_voidSendString(uint_8 *Copy_u8Data){

	/* Send string data */
	UART_voidSendStringBlocking(Copy_u8Data);

}



void BLUETOOTH_voidWriteNumber(uint_32 Copy_u32Number){

	uint_32 Local_u8Reversed = 1;

	if(0 == Copy_u32Number){

		BLUETOOTH_voidSendChar('0');
	}
	while(0 != Copy_u32Number){

		Local_u8Reversed = ((Local_u8Reversed * 10) + (Copy_u32Number % 10));
		Copy_u32Number /= 10;
	}
	while(1 != Local_u8Reversed){

		BLUETOOTH_voidSendChar((Local_u8Reversed % 10 ) + 48);
		Local_u8Reversed /= 10;
	}

}


// Function to initialize UART communication
void UART_Init(unsigned int baud_rate) {
  unsigned int ubrr_value = F_CPU / (16UL * baud_rate) - 1;

  // Set baud rate
  UBRRH = (unsigned char)(ubrr_value >> 8);
  UBRRL = (unsigned char)ubrr_value;

  // Enable receiver and transmitter
  UCSRB = (1 << RXEN) | (1 << TXEN);

  // Set frame format: 8 data bits, 1 stop bit
  UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

// Function to transmit a single character over UART
void UART_TransmitChar(unsigned char data) {
  while (!(UCSRA & (1 << UDRE)))
    ;
  UDR = data;
}

// Function to receive a single character over UART
unsigned char UART_ReceiveChar() {
  while (!(UCSRA & (1 << RXC)));
  return UDR;
}

// Function to receive a single character over UART
unsigned char UART_ReceiveCharfrist() {
	uint_16 cou =0;
  while ((!(UCSRA & (1 << RXC)))  && (cou < 8000 ))
  {
	  cou++;
  }
  return UDR;
}

// Function to transmit a block of data over Bluetooth
void transmitDataBlockOverBluetooth(uint8_t* data, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    UART_TransmitChar(data[i]);
    _delay_ms(10); // Delay to allow time for transmission
  }
}

// Function to receive a block of data over Bluetooth
void receiveDataBlockOverBluetooth(uint8_t* data, uint8_t length) {
  for (uint8_t i = 0; i < length; i++) {
    data[i] = UART_ReceiveChar();
    _delay_ms(10); // Delay to allow time for reception
  }
}



/******************************************************************************
********************************* END OF PROGRAM ******************************
******************************************************************************/
