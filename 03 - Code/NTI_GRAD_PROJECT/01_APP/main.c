/*
 * main.c
 *
 *  Created on: Oct 14, 2023
 *      Author: Dell
 */

#include "APP_interface.h"


extern state Current, Next;
extern uint_8 MotorFlag;
int main(){

	APP_voidIntialize();
	APP_voidWelcom();

	if(0 == readDataFromEEPROM(0x200)){
		APP_BlockingMode();
	}

	Next = RSt0;
	APP_voidFirstRun();

	while(1){
		if(1 == MotorFlag){
			APP_TEMP_MOTOR();
		}
		else{
			Motor_void_stop();
		}
		Current = Next;
		Next = APP_StateTrans(Current);
	}

	return 0;
}
