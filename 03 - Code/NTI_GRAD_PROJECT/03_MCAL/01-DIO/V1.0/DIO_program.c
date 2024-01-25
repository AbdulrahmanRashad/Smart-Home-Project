



/*-------------- Includes ---------------*/
#include "DIO_interface.h"
#include "DIO_config.h"

/*-------------- Helper Functions Declarations ---------------*/
#include "DIO_private.h"

/*-------------- Global Variable Definitions  ---------------*/

/*-------------- APIs Definitions ---------------*/
void DIO_voidSetPinDirection(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum, Pin_Direction copy_u8Mode){

    if(Input == copy_u8Mode){
        switch(copy_u8PortNum){
            case DIO_u8PORTA : CLR_BIT(DDRA, copy_u8PinNum); break;
            case DIO_u8PORTB : CLR_BIT(DDRB, copy_u8PinNum); break;
            case DIO_u8PORTC : CLR_BIT(DDRC, copy_u8PinNum); break;
            case DIO_u8PORTD : CLR_BIT(DDRD, copy_u8PinNum); break;
            default : /* Error */ break;
        }
    }
    else if(Output == copy_u8Mode){
         switch(copy_u8PortNum){
            case DIO_u8PORTA : SET_BIT(DDRA, copy_u8PinNum); break;
            case DIO_u8PORTB : SET_BIT(DDRB, copy_u8PinNum); break;
            case DIO_u8PORTC : SET_BIT(DDRC, copy_u8PinNum); break;
            case DIO_u8PORTD : SET_BIT(DDRD, copy_u8PinNum); break;
            default : /* Error */ break;
        }
    }
    else{
        /* Error */
    }
}
void DIO_voidSetPinValue(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum, Pin_Logic copy_u8Logic){

    if(Low == copy_u8Logic){
        switch(copy_u8PortNum){
            case DIO_u8PORTA : CLR_BIT(PORTA, copy_u8PinNum); break;
            case DIO_u8PORTB : CLR_BIT(PORTB, copy_u8PinNum); break;
            case DIO_u8PORTC : CLR_BIT(PORTC, copy_u8PinNum); break;
            case DIO_u8PORTD : CLR_BIT(PORTD, copy_u8PinNum); break;
            default : /* Error */ break;
        }
    }
    else if(High == copy_u8Logic){
         switch(copy_u8PortNum){
            case DIO_u8PORTA : SET_BIT(PORTA, copy_u8PinNum); break;
            case DIO_u8PORTB : SET_BIT(PORTB, copy_u8PinNum); break;
            case DIO_u8PORTC : SET_BIT(PORTC, copy_u8PinNum); break;
            case DIO_u8PORTD : SET_BIT(PORTD, copy_u8PinNum); break;
            default : /* Error */ break;
        }
    }
    else{
        /* Error */
    }
}
Pin_Logic DIO_u8GetPinValue(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum){
    Pin_Logic Local_u8Result = 0;
    switch(copy_u8PortNum){
        case DIO_u8PORTA : Local_u8Result = GET_BIT(PINA, copy_u8PinNum); break;
        case DIO_u8PORTB : Local_u8Result = GET_BIT(PINB, copy_u8PinNum); break;
        case DIO_u8PORTC : Local_u8Result = GET_BIT(PINC, copy_u8PinNum); break;
        case DIO_u8PORTD : Local_u8Result = GET_BIT(PIND, copy_u8PinNum); break;
        default : /* Error */ break;
    }

    return Local_u8Result;
}
Pin_Logic DIO_u8GetPinDirection(Port_t copy_u8PortNum, Pin_Position copy_u8PinNum){
    Pin_Logic Local_u8Result = 0;
    switch(copy_u8PortNum){
        case DIO_u8PORTA : Local_u8Result = GET_BIT(DDRA, copy_u8PinNum); break;
        case DIO_u8PORTB : Local_u8Result = GET_BIT(DDRB, copy_u8PinNum); break;
        case DIO_u8PORTC : Local_u8Result = GET_BIT(DDRC, copy_u8PinNum); break;
        case DIO_u8PORTD : Local_u8Result = GET_BIT(DDRD, copy_u8PinNum); break;
        default : /* Error */ break;
    }

    return Local_u8Result;
}
void DIO_voidTogglePin(uint_8 copy_u8PortNum, uint_8 copy_u8PinNum){

    switch(copy_u8PortNum){
        case DIO_u8PORTA : TOG_BIT(PORTA, copy_u8PinNum); break;
        case DIO_u8PORTB : TOG_BIT(PORTB, copy_u8PinNum); break;
        case DIO_u8PORTC : TOG_BIT(PORTC, copy_u8PinNum); break;
        case DIO_u8PORTD : TOG_BIT(PORTD, copy_u8PinNum); break;
        default : /* Error */ break;
    }
}
void DIO_voidSetPortDirection(Port_t copy_u8PortNum, uint_8 copy_u8Mode){
     
    switch(copy_u8PortNum){
        case DIO_u8PORTA : DDRA = copy_u8Mode; break;
        case DIO_u8PORTB : DDRB = copy_u8Mode; break;
        case DIO_u8PORTC : DDRC = copy_u8Mode; break;
        case DIO_u8PORTD : DDRD = copy_u8Mode; break;
        default : /* Error */ break;
    }

}
void DIO_voidSetPortValue(Port_t copy_u8PortNum, uint_8 copy_u8Logic){

    switch(copy_u8PortNum){
        case DIO_u8PORTA : PORTA = copy_u8Logic; break;
        case DIO_u8PORTB : PORTB = copy_u8Logic; break;
        case DIO_u8PORTC : PORTC = copy_u8Logic; break;
        case DIO_u8PORTD : PORTD = copy_u8Logic; break;
        default : /* Error */ break;
    }
}
/*-------------- ISRs Definitions ---------------*/

/*-------------- Helper or Static Functions Definitions ---------------*/

