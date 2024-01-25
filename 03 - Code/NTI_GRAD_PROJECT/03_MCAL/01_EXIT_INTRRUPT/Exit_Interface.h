/*
 * Exit_Interface.h
 *
 *  Created on: Oct 19, 2023
 *      Author: Dell
 */

#ifndef EXIT_INTRRUPT_EXIT_INTERFACE_H_
#define EXIT_INTRRUPT_EXIT_INTERFACE_H_


#include "../../00_LBS/BIT_MATH.h"
#include "../../00_LBS/STD_TYPES.h"


void EXTI_init(void);
void EXTI_SetCallBack(void (*ptr_to_fun)(void),uint_8 IntID);

#endif /* EXIT_INTRRUPT_EXIT_INTERFACE_H_ */
