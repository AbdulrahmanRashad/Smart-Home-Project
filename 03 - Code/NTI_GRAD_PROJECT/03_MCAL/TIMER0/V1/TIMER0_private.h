/*****************************************************************************/
/* Title        	: 	TIMER0 Driver					        	         */
/* File Name    	: 	TIMER0_private.h      	                             */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	13/11/2020                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
* Description 	: Guard to protect this File from include more 			      *
*                 than one time.                               	 	 		  *
******************************************************************************/

#ifndef TIMER0_PRIVATE_H
#define TIMER0_PRIVATE_H


/******************************************************************************
* !comment : TIMER0 Registers Addresses Definition.  			         	  *
******************************************************************************/

#define TCCR0    *((volatile uint_8 *) 0X53)
#define TCNT0    *((volatile uint_8 *) 0x52)
#define OCR0     *((volatile uint_8 *) 0x5C)
#define TIMSK    *((volatile uint_8 *) 0x59)
#define TIFR     *((volatile uint_8 *) 0x58)


/******************************************************************************
* !comment : TCCR0 Register PINS Definition.  			         	          *
******************************************************************************/

#define CS00      (uint_8)0
#define CS01      (uint_8)1
#define CS02      (uint_8)2
#define WGM01     (uint_8)3
#define COM00     (uint_8)4
#define COM01     (uint_8)5
#define WGM00     (uint_8)6
#define FOC0      (uint_8)7


/******************************************************************************
* !comment : TIMSK Register PINS Definition.  			         	          *
******************************************************************************/

#define TOIE0     (uint_8)0
#define OCIE0     (uint_8)1


/******************************************************************************
* !comment : TIFR Register PINS Definition.  			         	          *
******************************************************************************/

#define TOV0      (uint_8)0
#define OCF0      (uint_8)1


/******************************************************************************
* !comment : TIMER0 ISR attribute.  			         	                  *
******************************************************************************/

#define ISR(vector, ...)\
void vector (void) __attribute__((signal));\
void vector (void)


/******************************************************************************
* !comment : TIMER0 Interrupt Vectors Numbers Definition.  			          *
******************************************************************************/

#define TIMER0_OC0      __vector_10
#define TIMER0_OVF      __vector_11


/******************************************************************************
* !comment : TIMER0 maximum count they could reach.  			              *
******************************************************************************/

#define TIMER0_MAX_COUNT    255

#define TIMER_PRES_MUSK_COMP 0b11111000
#define TIMER_PRES_MUSK_PWM 0b11100000
#define TIMER_DIV_BY_64 0b00000011
#define TIMER_DIV_BY_8 0b00000010
/******************************************************************************
* !comment : TIMER0 Prescaler mask.  			                              *
******************************************************************************/

#define PRESCALER_MASK		0xF8



#endif 
/*** !comment : End of gaurd [TIMER0_PRIVATE_H] ******************************/
