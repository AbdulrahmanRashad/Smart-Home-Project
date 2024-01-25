/*
 * ADC_Progam.c
 *
 *  Created on: Oct 20, 2023
 *      Author: Dell
 */

/*****************************************************************************/
/* Title        	: 	ADC Driver					                         */
/* File Name    	: 	ADC_program.c   		                             */
/* Author       	: 	Fahd Badi                                            */
/* Version      	: 	1.0.0                                                */
/* Origin Date  	: 	06/11/2020                                           */
/* Notes        	: 	None                                                 */
/*****************************************************************************/

/******************************************************************************
******************************* STD LIB DIRECTIVES ****************************
******************************************************************************/

#include "../../../00_LBS/BIT_MATH.h"
#include "../../../00_LBS/STD_TYPES.h"

/******************************************************************************
****************************** Component DIRECTIVES ***************************
******************************************************************************/

#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"

/******************************************************************************
* !comment  :  Pointer to function for callback.  							  *
******************************************************************************/

void (*ADC_CallBack)(void) = NULL;

/******************************************************************************
**************************** Function Implementation **************************
******************************************************************************/


/******************************************************************************
* Description 	: ADC Initialization Function.				          	      *
******************************************************************************/

void ADC_voidInit(void){

	#if VOLTAGE_REF == AREF

		/* Select Verf = AREF */
		CLR_BIT(ADMUX, REFS0);
		CLR_BIT(ADMUX, REFS1);

	#elif VOLTAGE_REF == AVCC

		/* Select Verf = AVCC */
		SET_BIT(ADMUX, REFS0);
		CLR_BIT(ADMUX, REFS1);

	#elif VOLTAGE_REF == INTERNAL

		/* Select Verf = INTERNAL */
		SET_BIT(ADMUX, REFS0);
		SET_BIT(ADMUX, REFS1);

	#else
		#error("You Chose Wrong Voltage Reference Selections for ADC!")
	#endif



	#if ADJUST_RESULT == RIGHT_ADJUST

		/* select right adjust result */
		CLR_BIT(ADMUX, ADLAR);

	#elif ADJUST_RESULT == LEFT_ADJUST

		/* select left adjust result */
		SET_BIT(ADMUX, ADLAR);

	#else
		#error("You Chose Wrong ADC Adjust Result!")
	#endif



	#if PRESCALER == DIV_FACTOR_2

		/* Prescaler Select DIV 2 */
		SET_BIT(ADCSRA, ADPS0);
		CLR_BIT(ADCSRA, ADPS1);
		CLR_BIT(ADCSRA, ADPS2);

	#elif PRESCALER == DIV_FACTOR_4

		/* Prescaler Select DIV 4 */
		CLR_BIT(ADCSRA, ADPS0);
		SET_BIT(ADCSRA, ADPS1);
		CLR_BIT(ADCSRA, ADPS2);

	#elif PRESCALER == DIV_FACTOR_8

		/* Prescaler Select DIV 8 */
		SET_BIT(ADCSRA, ADPS0);
		SET_BIT(ADCSRA, ADPS1);
		CLR_BIT(ADCSRA, ADPS2);

	#elif PRESCALER == DIV_FACTOR_16

		/* Prescaler Select DIV 16 */
		CLR_BIT(ADCSRA, ADPS0);
		CLR_BIT(ADCSRA, ADPS1);
		SET_BIT(ADCSRA, ADPS2);

	#elif PRESCALER == DIV_FACTOR_32

		/* Prescaler Select DIV 32 */
		SET_BIT(ADCSRA, ADPS0);
		CLR_BIT(ADCSRA, ADPS1);
		SET_BIT(ADCSRA, ADPS2);

	#elif PRESCALER == DIV_FACTOR_64

		/* Prescaler Select DIV 64 */
		CLR_BIT(ADCSRA, ADPS0);
		SET_BIT(ADCSRA, ADPS1);
		SET_BIT(ADCSRA, ADPS2);

	#elif PRESCALER == DIV_FACTOR_128

		/* Prescaler Select DIV 128 */
		SET_BIT(ADCSRA, ADPS0);
		SET_BIT(ADCSRA, ADPS1);
		SET_BIT(ADCSRA, ADPS2);

	#else
		#error("You Chose Wrong ADC Prescaler Value!")
	#endif


	#if AUTO_TRIGGER == AUTO_TRIGGER_ENABLE

		/* Enable of trigger */
		SET_BIT(ADCSRA, ADATE);

		#if AUTO_TRIGGER_SOURCE == FREE_RUNNING

			CLR_BIT(SFIOR, ADTS0);
			CLR_BIT(SFIOR, ADTS1);
			CLR_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == ANALOG_COMPARATOR

			SET_BIT(SFIOR, ADTS0);
			CLR_BIT(SFIOR, ADTS1);
			CLR_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == EXTERNAL_INTERRUPT_REQUEST0

			CLR_BIT(SFIOR, ADTS0);
			SET_BIT(SFIOR, ADTS1);
			CLR_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == TIMER0_COMPARE_MATCH

			SET_BIT(SFIOR, ADTS0);
			SET_BIT(SFIOR, ADTS1);
			CLR_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == TIMER0_OVERFLOW

			CLR_BIT(SFIOR, ADTS0);
			CLR_BIT(SFIOR, ADTS1);
			SET_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == TIMER_COMPARE_MATCH_B

			SET_BIT(SFIOR, ADTS0);
			CLR_BIT(SFIOR, ADTS1);
			SET_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == TIMER1_OVERFLOW

			CLR_BIT(SFIOR, ADTS0);
			SET_BIT(SFIOR, ADTS1);
			SET_BIT(SFIOR, ADTS2);

		#elif AUTO_TRIGGER_SOURCE == TIMER1_CAPTURE_EVENT

			SET_BIT(SFIOR, ADTS0);
			SET_BIT(SFIOR, ADTS1);
			SET_BIT(SFIOR, ADTS2);

		#else
			#error("You Chose Wrong ADC Auto Trigger Source!")
		#endif

	#elif AUTO_TRIGGER == AUTO_TRIGGER_DISABLE

		/* disable of trigger & Select single conversion */
		CLR_BIT(ADCSRA, ADATE);

	#else
		#error("You Chose Wrong ADC Auto Trigger!")
	#endif

	/* ADC interrupt disable */
	CLR_BIT(ADCSRA, ADIE);
	//timer1 OVERFLOW
	SET_BIT(SFIOR,ADTS2);
	SET_BIT(SFIOR,ADTS1);
	CLR_BIT(SFIOR,ADTS0);

	/* Enable ADC */
	SET_BIT(ADCSRA, ADEN);

}



/******************************************************************************
* Description 	: ADC Read Value (Pooling) Function .						  *
******************************************************************************/

uint_32 ADC_u16ReadValue(uint_8 Copy_u8ChannelNumber){

	/* channel number must be from A0 --> A7 */
	Copy_u8ChannelNumber &= 0x07;

	/* clear first 5 bits in the ADMUX (channel number MUX4:0 bits)
	   before set the required channel */
	ADMUX &= 0xE0;

	/* choose the correct channel by setting the channel number in MUX4:0 bits */
	ADMUX |= Copy_u8ChannelNumber;

	/* Start conversion */
	SET_BIT(ADCSRA, ADSC);

	/* Wait until end conversion */
	while(0 == GET_BIT(ADCSRA, ADIF));

	/* Clear ADC interrupt flag */
	SET_BIT(ADCSRA, ADIF);
#if ADJUST_RESULT == RIGHT_ADJUST
		/* select right adjust result */
	  return ADC;

	#elif ADJUST_RESULT == LEFT_ADJUST

		/* select left adjust result */
		return ADCH;
	#else
		#error("You Chose Wrong ADC Adjust Result!")
	#endif
	/* return data */


}

uint_16 ADC_u8StartConversion(uint_8 Copy_u8ChannelNumber)
{
	ADMUX &= ADC_CHANNEL_MASK;
	ADMUX |= Copy_u8ChannelNumber;

	/*Start Conversion*/
	SET_BIT(ADCSRA, ADSC);

	/* Wait for Conversion Complete falg */
	while (GET_BIT(ADCSRA, ADIF) == 0 );


	return ADC;
}


/******************************************************************************
* Description 	: ADC Interrupt Enable Function.				          	  *
******************************************************************************/

void ADC_voidInterruptEnable2(void){

	/* ADC interrupt Enable */
	SET_BIT(ADCSRA, ADIE);
}


/******************************************************************************
* Description 	: ADC Interrupt Disable Function.				          	  *
******************************************************************************/

void ADC_voidInterruptDisable2(void){

	/* ADC interrupt Disable */
	CLR_BIT(ADCSRA, ADIE);
}



/******************************************************************************
* Description 	: This function used to	communicate with the function in App  *
*                 Layer(The Callback function ADC) -                          *
*		          [ Layer Architecture Problem Solved ].                      *
******************************************************************************/

void ADC_voidSetCallBack(void(*Local_ptr)(void)){

	ADC_CallBack = Local_ptr;

}


/******************************************************************************
* !comment  :  ISR Function ADC.  							 			      *
******************************************************************************/

/*ISR( ADC_VECTOR ){

	if(ADC_CallBack != NULL){

		ADC_CallBack();
	}
	else{  }

}
*/


/******************************************************************************
********************************* END OF PROGRAM ******************************
******************************************************************************/
