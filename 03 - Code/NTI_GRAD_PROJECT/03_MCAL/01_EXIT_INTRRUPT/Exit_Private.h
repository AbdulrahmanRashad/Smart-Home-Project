/*
 * Exit_Private.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Dell
 */

#ifndef EXIT_INTRRUPT_EXIT_PRIVATE_H_
#define EXIT_INTRRUPT_EXIT_PRIVATE_H_


#define MCUCR       *((volatile uint_8 *)0x55) //GIE INTERUPT1
#define MCUCUR      *((volatile uint_8 *)0x54) //GIE INTERUPT2
#define GICR        *((volatile uint_8 *)0x5B) //PIE
#define GIFR        *((volatile uint_8 *)0x5A) //PIF
#define SREG        *((volatile uint_8 *)0x5F) //GIE

#define ISC00   0
#define ISC01   1
#define ISC10   2
#define ISC11   3
#define INT0    6
#define INT1    7
#define INT2    5
#define ISC2    6

void __vector_1 (void) __attribute__((signal));
void __vector_2 (void) __attribute__((signal));
void __vector_3 (void) __attribute__((signal));


#endif /* 03_MCAL_01_EXIT_INTRRUPT_EXIT_PRIVATE_H_ */
