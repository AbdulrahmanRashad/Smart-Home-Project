/* 
 * File:   ecu_led.h
 * Author: Tahoun
 *
 * Created on June 15, 2023, 10:41 AM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/*-------------- Includes ---------------*/
#include "../../../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../../../03_MCAL/01-DIO/V1.0/DIO_config.h"

/*-------------- Macro Declarations ---------------*/
#define LED_SEQUENCE_LENGTH         8
#define LED_SEQUENCE_TIME_DELAY    10000

#define E_OK         0X0
#define E_NOT_OK     0X1


/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/
typedef uint_8 Std_ReturnType;

typedef enum{
    LedOff,
    LedOn
}Led_State;

typedef struct{
    unsigned Port : 3;
    unsigned Pin : 3;
    unsigned State : 1;
    unsigned Reserved : 1;
}Led_t;

/*-------------- Software Interfaces Declarations "APIs" ---------------*/
Std_ReturnType Ecual_Led_Intialize(const Led_t* Led);
Std_ReturnType Ecual_Led_TurnOn(const Led_t* Led);
Std_ReturnType Ecual_Led_TurnOff(const Led_t* Led);
Std_ReturnType Ecual_Led_Toggle(const Led_t* Led);
Std_ReturnType Ecual_Led_GetStatus(const Led_t* Led, Led_State *Led_Status);

//Std_ReturnType Ecual_Led_Sequence(const Led_t Led[], uint8 Seq[][LED_SEQUENCE_LENGTH], uint8 Seq_Length);

/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/


#endif	/* ECU_LED_H */

