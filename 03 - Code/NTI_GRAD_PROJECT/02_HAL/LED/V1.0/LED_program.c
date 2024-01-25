/* 
 * File:   ecu_led.h
 * Author: Tahoun
 *
 * Created on June 15, 2023, 10:41 AM
 */

/*-------------- Includes ---------------*/
#include "../../LED/V1.0/LED_interface.h"

/*-------------- Helper Functions Declarations ---------------*/

/*-------------- Global Variable Definitions  ---------------*/

/*-------------- APIs Definitions ---------------*/

/**
 * 
 * @param Led
 * @return 
 */
uint_8 Ecual_Led_Intialize(const Led_t* Led){
    Std_ReturnType ret = E_OK;
    if(0 == Led)
    {
        ret = E_NOT_OK;
    }
    else
    {
    	DIO_voidSetPinDirection(Led->Port, Led->Pin, Led->State);
    }
    return ret;
}
Std_ReturnType Ecual_Led_TurnOn(const Led_t* Led){
    Std_ReturnType ret = E_OK;
    if(0 == Led)
    {
        ret = E_NOT_OK;
    }
    else
    {
    	DIO_voidSetPinValue(Led->Port, Led->Pin, High);
    }
    return ret;
}
Std_ReturnType Ecual_Led_TurnOff(const Led_t* Led){
    Std_ReturnType ret = E_OK;
    if(0 == Led)
    {
        ret = E_NOT_OK;
    }
    else
    {
    	DIO_voidSetPinValue(Led->Port, Led->Pin, Low);
    }
    return ret;
}
Std_ReturnType Ecual_Led_Toggle(const Led_t* Led){
    Std_ReturnType ret = E_OK;
    if(0 == Led)
    {
        ret = E_NOT_OK;
    }
    else
    {
    	DIO_voidTogglePin(Led->Port, Led->Pin);
    }
    return ret;
}

Std_ReturnType Ecual_Led_GetStatus(const Led_t* Led, Led_State *Led_Status){
	Std_ReturnType ret = E_OK;

	    if(0 == Led)
	    {
	        ret = E_NOT_OK;
	    }
	    else
	    {
	    	*Led_Status = DIO_u8GetPinValue(Led->Port, Led->Pin);
	    	switch(*Led_Status)
	    	{
	    		case Low:
	    			*Led_Status = LedOff;
	    			break;
	    		case High:
	    			*Led_Status = LedOn;
	    			 break;
	    	}
	    }
	    return ret;
}

/*Std_ReturnType Ecual_Led_Sequence(const Led_t Led[], uint8 Seq[][LED_SEQUENCE_LENGTH], uint8 Seq_Length){
    uint8 Counter1 = 0, Counter2 = 0;
    uint16 Counter3 = 0; //Used as a Timer
    Std_ReturnType ret = E_OK;
    if(NULL == Led)
    {
        ret = E_NOT_OK;
    }
    else
    { 
        for(Counter2 = 0; Counter2 <= Seq_Length-1; Counter2++)
        {
            
           for(Counter1 = 0; Counter1 <= LED_SEQUENCE_LENGTH-1; Counter1++)
            {
               Pin_Config_t Pin_Led = {.Port = Led[Counter1].Port, .Positison = Led[Counter1].Pin,
                                .Logic = Led[Counter1].State ,.Direction = GPIO_Output};
                hal_GPIO_Pin_WriteLogic(&Pin_Led, Seq[Counter2][Counter1]);
            }
            for(Counter3 = 0; Counter3 <= LED_SEQUENCE_TIME_DELAY; Counter3++);
            //__delay_ms(LED_SEQUENCE_TIME_DELAY);
        }
    }
    return ret;  
}*/
/*-------------- ISRs Definitions ---------------*/

/*-------------- Helper or Static Functions Definitions ---------------*/
