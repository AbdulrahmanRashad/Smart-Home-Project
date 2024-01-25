


#ifndef DIO_INTRFACE_H
#define DIO_INTRFACE_H
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <util/delay.h>


/*-------------- Includes ---------------*/
#include "../../../00_LBS/BIT_MATH.h"
#include "../../../00_LBS/STD_TYPES.h"

/*-------------- Macro Declarations ---------------*/
typedef enum{
    DIO_PIN0,
	DIO_PIN1,
	DIO_PIN2,
	DIO_PIN3,
	DIO_PIN4,
	DIO_PIN5,
	DIO_PIN6,
	DIO_PIN7,
}Pin_Position;

typedef enum{
    DIO_u8PORTA,
    DIO_u8PORTB,
    DIO_u8PORTC,
    DIO_u8PORTD
}Port_t;

/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/
typedef enum{
    Input,
    Output
} Pin_Direction;

typedef enum{
    Low,
    High
} Pin_Logic;

/*-------------- Software Interfaces Declarations "APIs" ---------------*/
void DIO_voidSetPinDirection(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum, Pin_Direction copy_u8Mode);
void DIO_voidSetPinValue(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum, Pin_Logic copy_u8Logic);
Pin_Logic DIO_u8GetPinValue(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum);
Pin_Logic DIO_u8GetPinDirection(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum);
void DIO_voidTogglePin(uint_8 copy_u8PortNum, uint_8 copy_u8PinNum);
void DIO_voidSetPortDirection(Port_t copy_u8PortNum, Pin_Direction copy_u8Mode);
void DIO_voidSetPortValue(Port_t copy_u8PortNum, Pin_Logic copy_u8Logic);


/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/










#endif
















