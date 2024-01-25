#ifndef DIO_PRIVATE_H
#define DIO_PRIVATE_H


/*-------------- Includes ---------------*/

/*-------------- Macro Declarations ---------------*/
#define DDRA        *((volatile uint_8*) 0x3A)
#define PORTA       *((volatile uint_8*) 0x3B)
#define PINA        *((volatile uint_8*) 0x39)

#define DDRB        *((volatile uint_8*) 0x37)
#define PORTB       *((volatile uint_8*) 0x38)
#define PINB        *((volatile uint_8*) 0x36)

#define DDRC        *((volatile uint_8*) 0x34)
#define PORTC       *((volatile uint_8*) 0x35)
#define PINC        *((volatile uint_8*) 0x33)

#define DDRD        *((volatile uint_8*) 0x31)
#define PORTD       *((volatile uint_8*) 0x32)
#define PIND        *((volatile uint_8*) 0x30)

/*-------------- Helper Functions Declarations ---------------*/


/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/

/*-------------- Software Interfaces Declarations "APIs" ---------------*/

/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/




#endif
