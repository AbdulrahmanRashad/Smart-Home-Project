/*****************************************************************************/
/* Title        	: 	TIMER1 Driver					         			 */
/* File Name    	: 	TIMER1_private.h      	                             */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	15/11/2020                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
* Description 	: Guard to protect this File from include more 			      *
*                 than one time.                               	 	 		  *
******************************************************************************/

#ifndef TIMER1_PRIVATE_H
#define TIMER1_PRIVATE_H

/******************************************************************************
* !comment : TIMER1 Registers Addresses Definition.  			         	  *
******************************************************************************/

#define TIMSK     *((volatile uint_8 *) 0x59)
#define TIFR      *((volatile uint_8 *) 0x58)
#define TCCR1A    *((volatile uint_8 *) 0x4F)
#define TCCR1B    *((volatile uint_8 *) 0x4E)
#define TCNT1H    *((volatile uint_8 *) 0x4D)
#define TCNT1L    *((volatile uint_8 *) 0x4C)
#define OCR1AH    *((volatile uint_8 *) 0x4B)
#define OCR1AL    *((volatile uint_8 *) 0x4A)
#define OCR1BH    *((volatile uint_8 *) 0x49)
#define OCR1BL    *((volatile uint_8 *) 0x48)
#define ICR1H     *((volatile uint_8 *) 0x47)
#define ICR1L     *((volatile uint_8 *) 0x46)

#define TCNT1     *((volatile uint_16 *) 0x4C)
#define OCR1A     *((volatile uint_16 *) 0x4A)
#define OCR1B     *((volatile uint_16 *) 0x48)
#define ICR1      *((volatile uint_16 *) 0x46)

/******************************************************************************
* !comment : TCCR1A Register PINS Definition.  			         	          *
******************************************************************************/

#define WGM10      (uint_8)0
#define WGM11      (uint_8)1
#define FOC1B      (uint_8)2
#define FOC1A      (uint_8)3
#define COM1B0     (uint_8)4
#define COM1B1     (uint_8)5
#define COM1A0     (uint_8)6
#define COM1A1     (uint_8)7

/******************************************************************************
* !comment : TCCR1B Register PINS Definition.  			         	          *
******************************************************************************/

#define CS10       (uint_8)0
#define CS11       (uint_8)1
#define CS12       (uint_8)2
#define WGM12      (uint_8)3
#define WGM13      (uint_8)4
#define ICES1      (uint_8)6
#define ICNC1      (uint_8)7

/******************************************************************************
* !comment : TIMSK Register PINS Definition.  			         	          *
******************************************************************************/

#define TOIE1      (uint_8)2
#define OCIE1B     (uint_8)3
#define OCIE1A     (uint_8)4
#define TICIE1     (uint_8)5

/******************************************************************************
* !comment : TIFR Register PINS Definition.  			         	          *
******************************************************************************/

#define TOV1       (uint_8)2
#define OCF1B      (uint_8)3
#define OCF1A      (uint_8)4
#define ICF1       (uint_8)5

/******************************************************************************
* !comment : TIMER1 maximum count they could reach.  			         	  *
******************************************************************************/

#define TIMER1_MAX_COUNT    65535
    
/******************************************************************************
* !comment : TIMER1 Prescaler mask.  			         	                  *
******************************************************************************/	

#define PRESCALER_MASK		0xF8

/******************************************************************************
* !comment : TIMER1 ISR __attribute__.  			         	              *
******************************************************************************/

#define ISR(vector, ...)\
void vector (void) __attribute__((signal));\
void vector (void)

/******************************************************************************
* !comment : TIMER1 Interrupt Vectors Numbers Definition.  			          *
******************************************************************************/

#define TIMER1_ICU_VECT     __vector_6
#define TIMER1_OCA_VECT     __vector_7
#define TIMER1_OCB_VECT     __vector_8
#define TIMER1_OVF_VECT     __vector_9


/******************************************************************************
************************ Prototype Private Function ***************************
******************************************************************************/

/******************************************************************************
* Description 	: TIMER1 ICU Interrupt Action Function.				          *
* Parameters  	: none.                                                       *
* Return type 	: void.                                                  	  *
*******************************************************************************
* Pre_condition : This function Private Function Used With PWM Read Function. *
******************************************************************************/

static void Timer1ICU_InterruptAction(void);


#endif 
/*** !comment : End of gaurd [TIMER1_PRIVATE_H] ******************************/
