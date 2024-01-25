/*
 * ADC_Privet.h
 *
 *  Created on: Oct 20, 2023
 *      Author: Dell
 */

/*****************************************************************************/
/* Title        	: 	ADC Driver					     				     */
/* File Name    	: 	ADC_private.h      	                                 */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	06/11/2020                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
* Description 	: Guard to protect this File from include more 			      *
*                 than one time.                               	 	 		  *
******************************************************************************/

#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H


/******************************************************************************
* !comment : ADC Registers Addesses Definition.  			         	      *
******************************************************************************/

#define ADMUX       *((volatile uint_8  *) 0x27)
#define ADCSRA      *((volatile uint_8  *) 0x26)
#define ADCH        *((volatile uint_8  *) 0x25)
#define ADCL        *((volatile uint_8  *) 0x24)
#define ADC         *((volatile uint_16 *) 0x24)
#define SFIOR       *((volatile uint_8  *) 0x50)


/******************************************************************************
* !comment : ADMUX Register PINS Definition.  			         	          *
******************************************************************************/

#define MUX0      (uint_8)0
#define MUX1      (uint_8)1
#define MUX2      (uint_8)2
#define MUX3      (uint_8)3
#define MUX4      (uint_8)4
#define ADLAR     (uint_8)5
#define REFS0     (uint_8)6
#define REFS1     (uint_8)7


/******************************************************************************
* !comment : ADCSRA Register PINS Definition.  			         	 		  *
******************************************************************************/

#define ADPS0     (uint_8)0
#define ADPS1     (uint_8)1
#define ADPS2     (uint_8)2
#define ADIE      (uint_8)3
#define ADIF      (uint_8)4
#define ADATE     (uint_8)5
#define ADSC      (uint_8)6
#define ADEN      (uint_8)7


/******************************************************************************
* !comment : SFIOR Register PINS Definition.  			         	 		  *
******************************************************************************/

#define ADTS0     (uint_8)5
#define ADTS1     (uint_8)6
#define ADTS2     (uint_8)7


/******************************************************************************
* !comment : Voltage Reference Selections for ADC Definition.  			      *
******************************************************************************/

#define AREF        0
#define AVCC        1
#define INTERNAL    2

#define ADC_PRESC_MASK     0b11111000
#define ADC_CHANNEL_MASK   0b11100000
/******************************************************************************
* !comment : ADC Adjust Result Definition.  			         	          *
******************************************************************************/

#define LEFT_ADJUST      	0
#define RIGHT_ADJUST     	1


/******************************************************************************
* !comment : ADC Prescaler Selections Definition.  			         	      *
******************************************************************************/

#define DIV_FACTOR_2        1
#define DIV_FACTOR_4   	    2
#define DIV_FACTOR_8        3
#define DIV_FACTOR_16       4
#define DIV_FACTOR_32       5
#define DIV_FACTOR_64       6
#define DIV_FACTOR_128      7


/******************************************************************************
* !comment : ADC Auto Trigger Enable/ Desable Definition.  			     	  *
******************************************************************************/

#define AUTO_TRIGGER_ENABLE      	0
#define AUTO_TRIGGER_DISABLE     	1


/******************************************************************************
* !comment : ADC Auto Trigger Source Selections Definition.  			      *
******************************************************************************/

#define FREE_RUNNING 					0
#define ANALOG_COMPARATOR				1
#define EXTERNAL_INTERRUPT_REQUEST0		2
#define TIMER0_COMPARE_MATCH			3
#define TIMER0_OVERFLOW					4
#define TIMER_COMPARE_MATCH_B			5
#define TIMER1_OVERFLOW					6
#define TIMER1_CAPTURE_EVENT			7


/******************************************************************************
* !comment : ADC ISR function prototype.  			         	              *
******************************************************************************/

#define ISR(vector)\
void vector (void) __attribute__((signal));\
void vector (void)


/******************************************************************************
* !comment : ADC Interrupt Vectors Number Definition.  			              *
******************************************************************************/

//#define ADC_VECTOR     __vector_16



#endif
/*** !comment : End of gaurd [ADC_PRIVATE_H] *********************************/
