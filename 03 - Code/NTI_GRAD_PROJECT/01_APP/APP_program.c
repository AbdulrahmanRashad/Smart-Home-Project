/*
 * APP_program.c
 *
 *  Created on: Oct 23, 2023
 *      Author: Dell
 */


/*-------------- Includes ---------------*/
//#include <avr/eeprom.h>
#include "../00_LBS/BIT_MATH.h"
#include "../00_LBS/STD_TYPES.h"
#include "APP_interface.h"
#include "APP_config.h"
#include "APP_private.h"
#include "../03_MCAL/ADC/V1.0/ADC_Private.h"
#include "../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../02_HAL/BUZZER/V1/BUZZER_interface.h"
#include "../02_HAL/MOTOR/MOTOR_interface.h"
#include "../02_HAL/KEYPAD/V1.0/KEYPAD_interface.h"
#include "../02_HAL/CLCD/V1.0/CLCD_interface.h"
#include "../02_HAL/LED/V1.0/LED_interface.h"
#include "../02_HAL/SERVO/V1/SERVO_interface.h"
#include "../02_HAL/TEMP/V1/TEMP_interface.h"
#include "../03_MCAL/EEPROM/EEPROM_interface.h"
#include "../02_HAL/BLUETOOTH/V1/BLUETOOTH_interface.h"
#include "../03_MCAL/UART/UartInterface.h"
#include "../03_MCAL/ADC/V1.0/ADC_Interface.h"
#include "../03_MCAL/TIMER1/V1/TIMER1_interface.h"
/*-------------- Global Variable Definitions  ---------------*/

uint_8 x= 0;
uint_8 y= 0;
uint_8 MotorFlag = 0;
static uint_8 *LOCAL_u8NameAdd = &x;
static uint_8 *LOCAL_u8N = &y;
volatile uint64_t counter1=0;
uint_8 Local_u8Key = 0;
uint_8 NULLChar[8] = {0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
uint_8 Body[8] = {0X0E, 0X0E, 0X04, 0X1F, 0X04, 0X0E, 0X0A, 0X0A};
uint_8 DegMark[8] = {0X0E, 0X0A, 0X0E, 0X00, 0X00, 0X00, 0X00, 0X00};
const uint_8 Keypad_Array[KEYPAD_ROW][KEYPAD_COL] = {{'7', '8', '9', 'A'},
		{'4', '5', '6', 'B'},
		{'1', '2', '3', 'C'},
		{'C', '0', 'K', 'R'}};
uint_8 Motor1[2] = {DIO_PIN1};
//Led_t Dimmer = {.Port = DIO_u8PORTA, .Pin = PIN3, .State = Output};
Led_t LED1[5] = {{.Port = DIO_u8PORTA, .Pin = DIO_PIN3, .State = Output},
		{.Port = DIO_u8PORTA, .Pin = DIO_PIN4, .State = Output},
		{.Port = DIO_u8PORTA, .Pin = DIO_PIN5, .State = Output},
		{.Port = DIO_u8PORTA, .Pin = DIO_PIN6, .State = Output},
		{.Port = DIO_u8PORTA, .Pin = DIO_PIN7, .State = Output},};
state Current, Next;

/*-------------- APIs Definitions ---------------*/
/********************************************************************************** DONE**/
void APP_voidIntialize(void)
{
	uint_8 Counter = 0;
	BLUETOOTH_voidInit();
	CLCD_voidInit();
	Keypad_voidInit();
	APP_SERVO_INTILIZE();
	ADC_voidInit();
	BUZZER_voidInit();

	TIMER1_voidInit(TIMER1_FAST_PWM_10BIT_MODE,TIMER1_SCALER_64);
	TIMER1_voidOutputCompareInit(TIMER1_OC1_NON_INVERTING_MODE ,TIMER1_OCR1B);
	DIO_voidSetPinDirection(DIO_u8PORTD,DIO_PIN4,Output);
	DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Input);

	Motor_void_Intilize();
	//DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN2, Input);

	for(Counter = 0; Counter < 5; Counter++){
		Ecual_Led_Intialize(&LED1[Counter]);
	}
	CLCD_voidStoreCustomChar(0, NULLChar);
	CLCD_voidStoreCustomChar(1, Body);
	CLCD_voidStoreCustomChar(2, DegMark);
}
/********************************************************************************** DONE**/
void APP_voidWelcom(void)
{
	CLCD_voidGoto(1,6);
	CLCD_voidWriteString("WELCOM");

	for(uint_8 Counter = 0; Counter < 5; Counter++)
	{
		CLCD_voidGoto(2,7);
		CLCD_voidWriteData(1);
		CLCD_voidGoto(2,10);
		CLCD_voidWriteData(1);
		_delay_ms(100);
		CLCD_voidGoto(2,7);
		CLCD_voidWriteData(0);
		CLCD_voidGoto(2,10);
		CLCD_voidWriteData(0);
		_delay_ms(100);
	}
	CLCD_voidSendCommand(0X01);
}
/********************************************************************************** DONE**/
void APP_voidFirstRun(void)
{
	Std_Return Local_u8Ret = 0;
	uint_8 read = 0;
	read = readDataFromEEPROM(0x99);

	if(read == 0XFF)
	{
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("FIRST LOGIN.....");
		CLCD_voidGoto(4,14);
		CLCD_voidWriteString("NTI");

		transmitDataBlockOverBluetooth("FIRST LOGIN", 11);
		transmitDataBlockOverBluetooth("SET ADMIN NAME",14);
		transmitDataBlockOverBluetooth(" AND PASSWORD : \n",18);
		Local_u8Ret = APP_SetAdmin_and_Pass_First();
		if(R_OK == Local_u8Ret){
				writeDataToEEPROM(0x99,0XAA);
		}
		Next = RSt3;
	}

	CLCD_voidSendCommand(0X01);

}

void APP_BlockingMode(void)
{
	writeDataToEEPROM(0x200,0);//write false at blocked location in EEPROM
	BUZZER_voidToggle();
	if(0 == readDataFromEEPROM(0x200))//if the login process was blocked wait till the end of the block period
	{
		CLCD_voidSendCommand(0x01);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		CLCD_voidWriteString("Login blocked");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("wait 20 seconds");
		_delay_ms(20000);//Halt the system for the given time in (ms)

		writeDataToEEPROM(0x200,0XFF);//write false at blocked location in EEPROM
		CLCD_voidSendCommand(0x01);
	}
	BUZZER_voidToggle();
}
void APP_ADC_CONTROL_DIMMER(void){
	uint32_t reads_digital_value=0;

	//start convertion
	ADCSRA|=(1<<ADSC);
	while(( (ADCSRA & (1<<ADSC))) ==1); //WAIT CNVERTION
	reads_digital_value=(uint32_t)((((uint32_t)ADC_u16ReadValue(CH_A2)<<8))+(ADCL));
	TIMER1_voidSetTCNT1(0);
	for(uint16_t i=0;i<reads_digital_value;i++){
		if((GET_BIT(ADCSRA, ADIF))==0){
			TIMER1_voidSetOCR1B(i);
			_delay_ms(2);
		}
	}
	_delay_ms(2000);

}

void APP_TEMP_MOTOR(void){
	uint8_t Count_Tmperture=0;
	uint32_t reads_digital_value=0;
	uint16_t reads_analog_value=0;

	reads_digital_value=(ADC_u16ReadValue(CH_A0));

	reads_analog_value=(uint16_t)((((uint32_t)reads_digital_value*500UL)/256UL)+1U);
	if(reads_analog_value>21){
		Count_Tmperture=(uint8_t)reads_analog_value;
		Count_Tmperture++;
		if(Count_Tmperture>=28 && Count_Tmperture<50){
			Motor_void_ck();
			Count_Tmperture=0;
		}
	}
	else if(reads_analog_value<=21){
		reads_analog_value=0;
		Count_Tmperture=0;
		Motor_void_stop();
	}

}
void APP_SERVO_INTILIZE(void){
	SERVO_voidInit();

	TIMER1_voidSetICR1(0);
}

void APP_FAST_PWM2_TIMER1(void){

	TIMER1_voidSetOCR1A(2000U);
	_delay_ms(200);
	/* 2000us Ton to get angle zero */
	TIMER1_voidSetOCR1A(4000U);
	_delay_ms(200);
}

void APP_FAST_PWM3_TIMER1(void)
{
	TIMER1_voidSetICR1(40000U);
	TIMER1_voidSetOCR1A(2000U);
	_delay_ms(200);
	/* 2000us Ton to get angle zero */
	TIMER1_voidSetOCR1A(4000U);
	_delay_ms(200);

}

void APP_FAST_PWM_TIMER1(void)
{
	TIMER1_voidSetICR1(40000U);//START 180 THIS IS OVERFLOW VALUE & NEED COMPARE VALUE2
	_delay_ms(200);

}

state APP_StateTrans(state St){
	Local_u8Key = Keypad_u8GetPressed(Keypad_Array);

	switch(St)
	{
	case RSt0 :
		Next = APP_Start_Choose_login();
		break;
	case ESt0 :
		Next = APP_LoginSystem(User_Keypad);
		break;
	case RSt1 :
		Next = APP_LoginSystem(Admin);
		break;
	case RSt2 :
		Next = APP_LoginSystem(User_Remote);
		break;
	case ESt1 :
		Next = APP_HomePageMode(User_Keypad);
		break;
	case RSt3 :
		Next =  APP_HomePageMode(Admin);
		break;
	case RSt4 :
		Next = APP_HomePageMode(User_Remote);
		break;
	case ESt2 :
		Next = APP_StatusMenu(User_Keypad);
		break;
	case ESt3 :
		Next = APP_ControlMenu(User_Keypad);
		break;
	case RSt5 :
		Next = APP_ControlMenu(Admin);
		break;
	case RSt6 :
		Next = APP_StatusMenu(Admin);
		break;
	case RSt7 :
		Next = APP_Admin_ADD_User();
		break;
	case RSt8 :
		Next = APP_Admin_Remove_User();
		break;
	case RSt9 :
		Next = APP_ChangeAdmin_Pass();
		break;
	case RSt10 :
		Next = APP_ControlMenu(User_Remote);
		break;
	case RSt11 :
		Next = APP_StatusMenu(User_Remote);
		break;
	case ESt4 :
		Next = APP_RoomsStatusMenu(User_Keypad);
		break;
	case ESt5 :
		/*Door STATUS*/
		Next = APP_DoorStatus(User_Keypad);
		break;
	case ESt6 :
		/*AC STATUS*/
		Next = APP_AC_Status(User_Keypad);
		break;
	case ESt7 :
		/*DIMMIR STATUS*/
		Next = APP_DimmerStatus(User_Keypad);
		break;
	case ESt8 :
		Next = APP_RoomsControlMenu(User_Keypad);
		break;
	case ESt9 :
		/*Dimmer Control*/
		Next = APP_DimmerControl(User_Keypad);
		break;
	case ESt10 :
		/*AC Control*/
		Next = APP_AC_Control(User_Keypad);
		break;
	case RSt12 :
		/*Rooms Control Admin*/
		Next = APP_RoomsControlMenu(Admin);
		break;
	case RSt13 :
		/*Dimmer Control Admin*/
		Next = APP_DimmerControl(Admin);
		break;
	case RSt14 :
		/*AC Control Admin*/
		Next = APP_AC_Control(Admin);
		break;
	case RSt15 :
		/*Door Control*/
		Next = APP_DoorControl();
		break;
	case RSt16 :
		/*Rooms Status Admin*/
		Next = APP_RoomsStatusMenu(Admin);
		break;
	case RSt17 :
		/*Dimmer Status Admin*/
		Next = APP_DimmerStatus(Admin);
		break;
	case RSt18 :
		/*AC Status Admin*/
		Next = APP_AC_Status(Admin);

		break;
	case RSt19 :
		/*Door Status Admin*/
		Next = APP_DoorStatus(Admin);
		break;
	case RSt20 :
		Next = APP_RoomsControlMenu(User_Remote);
		break;
	case RSt21 :
		/*Dimmer Control*/
		Next = APP_DimmerControl(User_Remote);
		break;
	case RSt22 :
		/*AC Control*/
		Next = APP_AC_Control(User_Remote);
		break;
	case ESt11 :
		Next = APP_ROOM1_Control(User_Keypad);
		break;
	case ESt12 :
		Next = APP_ROOM2_Control(User_Keypad);
		break;
	case ESt13 :
		Next = APP_ROOM3_Control(User_Keypad);
		break;
	case ESt14 :
		Next = APP_ROOM4_Control(User_Keypad);
		break;
	case ESt15 :
		Next = APP_ROOM5_Control(User_Keypad);
		break;
	case ESt17 :
		Next = APP_ROOM1_Status(User_Keypad);
		break;
	case ESt18 :
		Next = APP_ROOM2_Status(User_Keypad);
		break;
	case ESt19 :
		Next = APP_ROOM3_Status(User_Keypad);
		break;
	case ESt20 :
		Next = APP_ROOM4_Status(User_Keypad);
		break;
	case ESt21 :
		Next = APP_ROOM5_Status(User_Keypad);
		break;
	case RSt23 :
		Next = APP_RoomsStatusMenu(User_Remote);
		break;
	case RSt24 :
		/*DIMMIR STATUS*/
		Next = APP_DimmerStatus(User_Remote);
		break;
	case RSt25 :
		/*AC STATUS*/
		Next = APP_AC_Status(User_Remote);
		break;
	case RSt26 :
		/*Door STATUS*/
		Next = APP_DoorStatus(User_Remote);
		break;
	case RSt27 :
		Next = APP_ROOM1_Control(Admin);
		break;
	case RSt28 :
		Next = APP_ROOM2_Control(Admin);
		break;
	case RSt29 :
		Next = APP_ROOM3_Control(Admin);
		break;
	case RSt30 :
		Next = APP_ROOM4_Control(Admin);
		break;
	case RSt31 :
		Next = APP_ROOM5_Control(Admin);
		break;
	case RSt32 :
		Next = APP_ROOM1_Status(Admin);
		break;
	case RSt33 :
		Next = APP_ROOM2_Status(Admin);
		break;
	case RSt34 :
		Next = APP_ROOM3_Status(Admin);
		break;
	case RSt35 :
		Next = APP_ROOM4_Status(Admin);
		break;
	case RSt36 :
		Next = APP_ROOM5_Status(Admin);
		break;
	case RSt37 :
		Next = APP_ROOM1_Control(User_Remote);
		break;
	case RSt38 :
		Next = APP_ROOM2_Control(User_Remote);
		break;
	case RSt39 :
		Next = APP_ROOM3_Control(User_Remote);
		break;
	case RSt40 :
		Next = APP_ROOM4_Control(User_Remote);
		break;
	case RSt41 :
		Next = APP_ROOM5_Control(User_Remote);
		break;
	case RSt42 :
		Next = APP_ROOM1_Status(User_Remote);
		break;
	case RSt43 :
		Next = APP_ROOM2_Status(User_Remote);
		break;
	case RSt44 :
		Next = APP_ROOM3_Status(User_Remote);
		break;
	case RSt45 :
		Next = APP_ROOM4_Status(User_Remote);
		break;
	case RSt46 :
		Next = APP_ROOM5_Status(User_Remote);
		break;

	}
	return Next;
}

/*-------------- ISRs Definitions ---------------*/

/*-------------- Helper or Static Functions Definitions ---------------*/
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static state APP_ChangeAdmin_Pass(void)
{
	uint8_t Local_u8password = 0, Local_u8Key = 0 ;
	//uint16_t number = 0;
	transmitDataBlockOverBluetooth("New Pass \n: ",11);
	while (Local_u8password < 4)
	{
		Local_u8Key = UART_ReceiveChar();
		if(Local_u8Key >= '0' && Local_u8Key <= '9')
		{
			//number = number * 10 + (Local_u8password-'0');
			Local_u8password++;
			eeprom_write_byte((uint_8*)(EEPROM_ADDR_F_Admin + Local_u8password), Local_u8Key - '0');
			//CLCD_voidWriteData(Local_u8Key);
			//CLCD_voidSendCommand(0x90 + 10 + Local_u8password);
			//_delay_ms(100);
			//CLCD_voidWriteData('*');
		}
	}
	Next = RSt0;
	return Next;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return APP_SetAdmin_and_Pass_First(void)
{
	Std_Return ret = R_NOT_OK;
	uint8_t Local_u8password = 0, Local_u8Ke = 0 ;
	uint16_t number = 0;

	transmitDataBlockOverBluetooth("Admin : \n",10);
	while(Local_u8password < 1)
	{
		Local_u8Ke = UART_ReceiveChar();
		if(Local_u8Ke >= '0' && Local_u8Ke <= '9')
		{
			eeprom_write_byte((uint_8*)(EEPROM_ADDR_F_Admin ), Local_u8Ke - '0');
			Local_u8password++;
		}
	}
	Local_u8Ke = 0;
	Local_u8password = 0;
	transmitDataBlockOverBluetooth("Pass : \n",9);
	while (Local_u8password < 4)
	{
		Local_u8Ke = UART_ReceiveChar();
		if(Local_u8Ke >= '0' && Local_u8Ke <= '9')
		{
			number = number * 10 + (Local_u8Ke-'0');

			Local_u8password++;

			eeprom_write_byte((uint_8*)(EEPROM_ADDR_F_Admin + Local_u8password), Local_u8Ke - '0');
			ret = R_OK;
		}

	}

	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static state APP_LoginSystem(Login_t Copy_LoginType)
{
	uint_8 Trial = 0;
	CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
	*LOCAL_u8N =0;
	switch (Copy_LoginType) {
	case User_Keypad:
		if(Check_Usernamekeypad(LOCAL_u8NameAdd) == OK )
		{
			while(Trial < 3)
			{
				CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
				CLCD_voidWriteString("UserName ");
				CLCD_voidWriteNumbers((*LOCAL_u8N));
				CLCD_voidGoto(2,1);
				CLCD_voidWriteString("Pass: ");
				if(Check_PassKeypad(LOCAL_u8NameAdd) == OK)
				{
					MotorFlag = 1;
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					CLCD_voidWriteString("Correct Pass ");
					_delay_ms(500);
					Next = ESt1;
					//*LOCAL_u8N =0;
					break;
				}
				else
				{
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					CLCD_voidWriteString("Try again ");
					_delay_ms(500);
					/*try again*/
					Trial++;
					//Next = St1;
				}
			}
			if(Trial ==3)
			{
				Trial=0;
				APP_BlockingMode();
				Next = ESt0;
			}
			*LOCAL_u8N =0;
		}
		else
		{
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			CLCD_voidWriteString("Try again ");
			Next = ESt0;
		}
		CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
		break;
	case User_Remote:
		if(Check_UsernameRemote(LOCAL_u8NameAdd) == OK )
		{
			while(Trial < 3)
			{
				transmitDataBlockOverBluetooth("UserName-\n",11);
				UART_TransmitChar((*LOCAL_u8N)+'0');
				*LOCAL_u8N =0;

				transmitDataBlockOverBluetooth("Pass \n",7);

				if(Check_PassRemote(LOCAL_u8NameAdd) == OK)
				{
					MotorFlag = 1;
					transmitDataBlockOverBluetooth("Correct Pass \n",15);
					Next = RSt4;
					break;
				}
				else{
					transmitDataBlockOverBluetooth("Try again \n",12);
					Trial++;
				}
			}
			if(Trial ==3)
			{
				Trial=0;
				APP_BlockingMode();

				Next = RSt2;
			}
		}
		else
		{
			transmitDataBlockOverBluetooth("Try again \n",12);
			Next = RSt2;
		}
		break;
	case Admin:
		if(Check_AdminName(LOCAL_u8NameAdd) == OK )
		{
			while(Trial < 3)
			{

				transmitDataBlockOverBluetooth("Admin Name \n",13);
				UART_TransmitChar((*LOCAL_u8N)+'0');
				*LOCAL_u8N =0;

				transmitDataBlockOverBluetooth("Pass \n",7);
				if(Check_AdminPass(LOCAL_u8NameAdd) == OK)
				{
					MotorFlag = 1;
					transmitDataBlockOverBluetooth("Correct Pass \n",15);
					Next = RSt3;
					break;
				}
				else
				{
					transmitDataBlockOverBluetooth("Try again \n",12);
					Trial++;
				}
			}
			if(Trial ==3)
			{
				Trial=0;
				APP_BlockingMode();
				Next = RSt1;
			}
		}
		else
		{
			transmitDataBlockOverBluetooth("Try again \n",12);
			Next = RSt1;
		}
		break;
	}

	return Next;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return Check_PassKeypad(uint_8 *Copy_add)
{
	Std_Return ret = R_NOT_OK;
	uint_32 LOCAL_u8pass = 0;
	uint_8 LOCAL_u8Counter1 = 0, LOCAL_u8TrueCounter = 0, LOCAL_u8Counter2 = 6;
	uint_8 X = '+';
	uint_8 Arr[4] ={0,0,0,0};
	while((LOCAL_u8Counter2 < 10)&& (X  !='K'))
	{
		X = Keypad_u8GetPressed(Keypad_Array);
		if(X != '+'){
			if((X >= '0') &&(X <= '9')){
				LOCAL_u8pass = X-'0';
				Arr[LOCAL_u8Counter1] =LOCAL_u8pass;
				CLCD_voidWriteNumbers(LOCAL_u8pass);
				_delay_ms(100);
				CLCD_voidGoto(2,LOCAL_u8Counter2+1);
				CLCD_voidWriteData('*');
				LOCAL_u8Counter2++;
				LOCAL_u8Counter1++;
			}
		}
	}


	for(LOCAL_u8Counter1 =0; LOCAL_u8Counter1 <4; LOCAL_u8Counter1++)
	{

		if(Arr[LOCAL_u8Counter1] == readDataFromEEPROM( (*Copy_add) + LOCAL_u8Counter1 + 1))
		{
			LOCAL_u8TrueCounter++;
		}
		else
		{
			break;
		}
	}
	if(LOCAL_u8TrueCounter == 4)
	{
		ret = R_OK;
	}

	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return Check_PassRemote(uint_8 *Copy_add)
{
	Std_Return ret = R_NOT_OK;
	uint_32 LOCAL_u8pass = 0;
	uint_8 LOCAL_u8Counter1 = 0, LOCAL_u8TrueCounter = 0, LOCAL_u8Counter2 = 6;
	uint_8 X = '+';
	uint_8 Arr[4] ={0,0,0,0};
	while((LOCAL_u8Counter2 < 10)&& (X  !='K'))
	{
		X = UART_ReceiveChar();
		if(X != '+'){
			if((X >= '0') &&(X <= '9')){
				LOCAL_u8pass = X-'0';
				Arr[LOCAL_u8Counter1] =X;
				LOCAL_u8Counter2++;
				LOCAL_u8Counter1++;
			}
		}
	}
	for(LOCAL_u8Counter1 =0; LOCAL_u8Counter1 <4; LOCAL_u8Counter1++)
	{
		;
		if(Arr[LOCAL_u8Counter1] - '0' == readDataFromEEPROM( (*Copy_add) + LOCAL_u8Counter1 + 1))
		{
			LOCAL_u8TrueCounter++;

		}
		else
		{
			break;
		}
	}
	if(LOCAL_u8TrueCounter == 4)
	{
		ret = R_OK;
	}

	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return Check_Usernamekeypad(uint_8 *Copy_add)
{
	Std_Return ret = R_NOT_OK;
	uint_32 LOCAL_u8Name = 0;
	uint_8 LOCAL_u8Counter2 = 11;
	uint_8 X = '+';
	CLCD_voidGoto(1,1);
	CLCD_voidWriteString("UserName: ");
	while((LOCAL_u8Counter2 < 12)&& (X  !='K')){
		X = Keypad_u8GetPressed(Keypad_Array);
		if(X != '+'){
			if((X >= '0') &&(X <= '9')){
				CLCD_voidWriteNumbers(X-'0');
				LOCAL_u8Name = X-'0';
				LOCAL_u8Counter2++;
			}
		}
	}
	for(LOCAL_u8Counter2 = 0; LOCAL_u8Counter2 <= 20; LOCAL_u8Counter2 +=5)
	{
		(*LOCAL_u8N)++;
		if(LOCAL_u8Name == readDataFromEEPROM(LOCAL_u8Counter2 + EEPROM_ADDR_F_UsersKeypad))
		{
			*Copy_add = LOCAL_u8Counter2 + EEPROM_ADDR_F_UsersKeypad;
			ret = R_OK;
			break;
		}
	}
	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return Check_UsernameRemote(uint_8 *Copy_add)
{
	Std_Return ret = R_NOT_OK;
	uint_32 LOCAL_u8Name = 0;
	uint_8 LOCAL_u8Counter2 = 11;
	uint_8 X = '+';
	transmitDataBlockOverBluetooth("UserName: \n",12);
	while((LOCAL_u8Counter2 < 12)&& (X  !='K')){
		X = UART_ReceiveChar();
		if(X != '+'){
			if((X >= '0') &&(X <= '9')){
				LOCAL_u8Name = X-'0';
				LOCAL_u8Counter2++;
			}
		}
	}

	for(LOCAL_u8Counter2 = 0; LOCAL_u8Counter2 <= 20; LOCAL_u8Counter2 +=5)
	{

		(*LOCAL_u8N)++;

		if(LOCAL_u8Name == readDataFromEEPROM(LOCAL_u8Counter2 + EEPROM_ADDR_F_UsersRemote))
		{

			*Copy_add = LOCAL_u8Counter2 + EEPROM_ADDR_F_UsersRemote;
			ret = R_OK;
			break;
		}
	}
	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return Check_AdminName(uint_8 *Copy_add)
{
	Std_Return ret = R_NOT_OK;
	uint_32 LOCAL_u8Name = 0;
	uint_8 LOCAL_u8Counter2 = 11;
	uint_8 X = '+';
	transmitDataBlockOverBluetooth("Admin Name: \n",14);
	while((LOCAL_u8Counter2 < 12)&& (X  !='K')){
		X = UART_ReceiveChar();
		if(X != '+'){
			if((X >= '0') &&(X <= '9')){
				LOCAL_u8Name = X-'0';
				LOCAL_u8Counter2++;
			}
		}
	}
	for(LOCAL_u8Counter2 = 0; LOCAL_u8Counter2 < 1; LOCAL_u8Counter2++)
	{
		(*LOCAL_u8N)++;
		if(LOCAL_u8Name == readDataFromEEPROM(LOCAL_u8Counter2 + EEPROM_ADDR_F_Admin))
		{
			*Copy_add = LOCAL_u8Counter2 + EEPROM_ADDR_F_Admin;

			ret = R_OK;
			break;
		}
	}
	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static Std_Return Check_AdminPass(uint_8 *Copy_add)
{
	Std_Return ret = R_NOT_OK;
	uint_32 LOCAL_u8pass = 0;
	uint_8 LOCAL_u8Counter1 = 0, LOCAL_u8TrueCounter = 0, LOCAL_u8Counter2 = 6;
	uint_8 X = '+';
	uint_8 Arr[4] ={0,0,0,0};
	while((LOCAL_u8Counter2 < 10)&& (X  !='K'))
	{
		X = UART_ReceiveChar();
		if(X != '+'){
			if((X >= '0') &&(X <= '9')){
				LOCAL_u8pass = X-'0';
				Arr[LOCAL_u8Counter1] =X;
				LOCAL_u8Counter2++;
				LOCAL_u8Counter1++;
			}
		}
	}
	for(LOCAL_u8Counter1 =0; LOCAL_u8Counter1 <4; LOCAL_u8Counter1++)
	{
		if(Arr[LOCAL_u8Counter1] - '0' == readDataFromEEPROM( (*Copy_add) + LOCAL_u8Counter1 + 1))
		{
			LOCAL_u8TrueCounter++;
		}
		else
		{
			break;
		}
	}
	if(LOCAL_u8TrueCounter == 4)
	{
		ret = R_OK;
	}

	return ret;
}
/*** Ahmed Ashraf___V1 ********************************************************  DONE**/
static state APP_Start_Choose_login(void)
{
	uint_8 read =1;
	uint_8 count=0;
	CLCD_voidGoto(2,5);
	CLCD_voidWriteString("LOGIN...");
	transmitDataBlockOverBluetooth("[1] Admin\n",11);
	transmitDataBlockOverBluetooth("[2] User",8);
	//read = UART_ReceiveCharfrist();
	while((read || Keypad_u8GetPressed(Keypad_Array) == 'K') && ! count)
	{
		read = UART_ReceiveCharfrist();
		if(read == '1')
		{
			Next = RSt1;
			count =1;
		}
		else if(Keypad_u8GetPressed(Keypad_Array) == 'K')
		{
			if(readDataFromEEPROM(EEPROM_ADDR_COUNTER_KU) <= 4)
			{
				Next = ESt0;
			}
			else
			{
				Next = RSt0;
			}
			count =1;
		}
		else if(read == '2')
		{
			if(readDataFromEEPROM(EEPROM_ADDR_COUNTER_RU) <= 4)
			{
				Next = RSt2;
			}
			else
			{
				Next = RSt0;
			}

			count =1;
		}
	}
	return Next;
}

static state APP_HomePageMode(Login_t Copy_LoginType){
	volatile uint_8 Read = 1;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] Control\n",13);
		transmitDataBlockOverBluetooth("[2] Status\n",12);
		transmitDataBlockOverBluetooth("[3] Add User\n",14);
		transmitDataBlockOverBluetooth("[4] Remove User\n",17);
		transmitDataBlockOverBluetooth("[5] Change Password\n",21);
		transmitDataBlockOverBluetooth("[6] Logout\n",12);

		while(Count){
			APP_SystemState();
			APP_TEMP_MOTOR();
			Read = UART_ReceiveCharfrist();
			switch (Read) {
			case '1':
				Count = 0;
				Next =RSt5;
				break;
			case '2':
				Count = 0;
				Next =RSt6;
				break;
			case '3':
				Count = 0;
				Next =RSt7;
				break;
			case '4':
				Count = 0;
				Next =RSt8;
				break;
			case '5':
				Count = 0;
				Next =RSt9;
				break;
			case '6':
				CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
				Count = 0;
				MotorFlag = 0;
				Next =RSt0;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] Control\n",13);
		transmitDataBlockOverBluetooth("[2] Status\n",12);
		transmitDataBlockOverBluetooth("[3] Logout\n",12);

		while(Count){
			APP_SystemState();
			APP_TEMP_MOTOR();
			Read = UART_ReceiveCharfrist();
			switch (Read) {
			case '1':
				Count = 0;
				Next =RSt10;
				break;
			case '2':
				Count = 0;
				Next =RSt11;
				break;
			case '3':
				CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
				MotorFlag = 0;
				Count = 0;
				Next =RSt0;
				break;
			}
		}
		break;

	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]CONTROL");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]STATUS");
		CLCD_voidGoto(3,1);
		CLCD_voidWriteString("[3]LOGOUT");

		switch(Local_u8Key)
		{
		case '1' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt3;
			break;
		case '2' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt2;
			break;
		case '3' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			MotorFlag = 0;
			Next = RSt0;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
		break;
	}

	return Next;

}

static state APP_ControlMenu(Login_t Copy_LoginType){
	volatile uint_8 Read = 1;
	uint_8 Count = 1;
	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOMS\n",11);
		transmitDataBlockOverBluetooth("[2] DIMMIR\n",12);
		transmitDataBlockOverBluetooth("[3] AC\n",8);
		transmitDataBlockOverBluetooth("[4] DOOR\n",10);
		while(Count){
			APP_SystemState();
			APP_TEMP_MOTOR();
			Read = UART_ReceiveCharfrist();

			switch (Read) {
			case '1':
				Next =RSt12;
				Count = 0;
				break;
			case '2':
				Next =RSt13;
				Count = 0;
				break;
			case '3':
				Next =RSt14;
				Count = 0;
				break;
			case '4':
				Next =RSt15;
				Count = 0;
				break;
			case '0':
				Next =RSt3;
				Count = 0;
				break;
			case 'C':
				Next =RSt3;
				Count = 0;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOMS\n",11);
		transmitDataBlockOverBluetooth("[2] DIMMIR\n",12);
		transmitDataBlockOverBluetooth("[3] AC\n",8);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch (Read) {
			case '1':
				Next =RSt20;
				Count = 0;
				break;
			case '2':
				Next =RSt21;
				Count = 0;
				break;
			case '3':
				Next =RSt22;
				Count = 0;
				break;
			case '0':
				Next =RSt4;
				Count = 0;
				break;
			case 'C':
				Next =RSt4;
				Count = 0;
				break;
			}

		}
		break;

	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOMS");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]DIMMER");
		CLCD_voidGoto(3,1);
		CLCD_voidWriteString("[3]AC");
		switch(Local_u8Key)
		{
		case '1' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt8;
			break;

		case '2' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt9;
			break;

		case '3' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt10;
			break;

		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
		break;
	}

	return Next;
}

static state APP_StatusMenu(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOMS\n",11);
		transmitDataBlockOverBluetooth("[2] DIMMIR\n",12);
		transmitDataBlockOverBluetooth("[3] AC\n",8);
		transmitDataBlockOverBluetooth("[4] DOOR\n",10);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch (Read) {
			case '1':
				Next =RSt16;
				Count = 0;
				break;
			case '2':
				Next =RSt17;
				Count = 0;
				break;
			case '3':
				Next =RSt18;
				Count = 0;
				break;
			case '4':
				Next =RSt19;
				Count = 0;
				break;
			case '0':
				Next =RSt3;
				Count = 0;
				break;
			case 'C':
				Next =RSt3;
				Count = 0;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOMS\n",11);
		transmitDataBlockOverBluetooth("[2] DIMMIR\n",12);
		transmitDataBlockOverBluetooth("[3] AC\n",8);
		transmitDataBlockOverBluetooth("[4] DOOR\n",10);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch (Read) {
			case '1':
				Next =RSt23;
				Count = 0;
				break;
			case '2':
				Next =RSt24;
				Count = 0;
				break;
			case '3':
				Next =RSt25;
				Count = 0;
				break;
			case '4':
				Next =RSt26;
				Count = 0;
				break;
			case '0':
				Next =RSt4;
				Count = 0;
				break;
			case 'C':
				Next =RSt4;
				Count = 0;
				break;
			}

		}
		break;

	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOMS");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]DOOR");
		CLCD_voidGoto(3,1);
		CLCD_voidWriteString("[3]AC");
		CLCD_voidGoto(4,1);
		CLCD_voidWriteString("[4]DIMMER");
		switch(Local_u8Key)
		{
		case '1' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt4;
			break;
		case '2' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt5;
			break;
		case '3' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt6;
			break;
		case '4' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt7;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
		break;
	}

	return Next;

}

static state APP_RoomsControlMenu(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1]R1\n",7);
		transmitDataBlockOverBluetooth("[2]R2\n",7);
		transmitDataBlockOverBluetooth("[3]R3\n",7);
		transmitDataBlockOverBluetooth("[4]R4\n",7);
		transmitDataBlockOverBluetooth("[5]R5\n",7);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Count = 0;
				Next = RSt27;
				break;

			case '2' :
				Count = 0;
				Next = RSt28;
				break;
			case '3' :
				Count = 0;
				Next = RSt29;
				break;
			case '4' :
				Count = 0;
				Next = RSt30;
				break;
			case '5' :
				Count = 0;
				Next = RSt31;
				break;
			case '0' :
				Count = 0;
				Next = RSt5;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1]R1\n",7);
		transmitDataBlockOverBluetooth("[2]R2\n",7);
		transmitDataBlockOverBluetooth("[3]R3\n",7);
		transmitDataBlockOverBluetooth("[4]R4\n",7);
		transmitDataBlockOverBluetooth("[5]R5\n",7);

		Read = UART_ReceiveChar();
		while(Count && Read){

			APP_SystemState();
			APP_TEMP_MOTOR();
			switch(Read)
			{
			case '1' :
				Count = 0;
				Next = RSt37;
				break;
			case '2' :
				Count = 0;
				Next = RSt38;
				break;
			case '3' :
				Count = 0;
				Next = RSt39;
				break;
			case '4' :
				Count = 0;
				Next = RSt40;
				break;
			case '5' :
				Count = 0;
				Next = RSt41;
				break;
			case '0' :
				Count = 0;
				Next = RSt10;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]R1");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]R2");
		CLCD_voidGoto(3,1);
		CLCD_voidWriteString("[3]R3");
		CLCD_voidGoto(4,1);
		CLCD_voidWriteString("[4]R4");
		CLCD_voidGoto(1,8);
		CLCD_voidWriteString("[5]R5");
		switch(Local_u8Key)
		{
		case '1' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt11;
			break;

		case '2' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt12;
			break;
		case '3' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt13;
			break;
		case '4' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt14;
			break;
		case '5' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt15;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt3;
			break;
		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
		break;
	}
	return Next;
}

static state APP_DimmerControl(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] DIMMER ON\n",15);
		transmitDataBlockOverBluetooth("[2] DIMMER OFF\n",16);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				APP_ADC_CONTROL_DIMMER();
				Count = 0;
				Next = RSt13;
				break;
			case '2' :
				TIMER1_voidSetOCR1B(0);
				Count = 0;
				Next = RSt13;
				break;
			case '0' :
				Count = 0;
				Next = RSt5;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] DIMMER ON\n",15);
		transmitDataBlockOverBluetooth("[2] DIMMER OFF\n",16);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				APP_ADC_CONTROL_DIMMER();
				Count = 0;
				Next = RSt21;
				break;
			case '2' :
				TIMER1_voidSetOCR1B(0);
				Count = 0;
				Next = RSt21;
				break;
			case '0' :
				Count = 0;
				Next = RSt10;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :

		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1] DIMMER ON");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2] DIMMER OFF");

		switch(Local_u8Key)
		{
		case '1' :
			APP_ADC_CONTROL_DIMMER();
			Count = 0;
			Next = ESt9;
			break;
		case '2' :
			TIMER1_voidSetOCR1B(0);
			Count = 0;
			Next = ESt9;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt3;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
	}
	return Next;
}

static state APP_AC_Control(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] AC Active\n",15);
		transmitDataBlockOverBluetooth("[2] AC OFF\n",12);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :

				DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Input);
				Ecual_DC_Motor_Intialize(MOTOR_PORT, Motor1, Output);
				Count = 0;
				Next = RSt14;
				break;
			case '2' :

				/* Set PIN DC MOTOR INPUT*/
				Ecual_DC_Motor_Intialize(MOTOR_PORT, Motor1, Input);
				DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Output);
				Count = 0;
				Next = RSt14;
				break;
			case '0' :
				Count = 0;
				Next = RSt5;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] AC Active\n",15);
		transmitDataBlockOverBluetooth("[2] AC OFF\n",12);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :

				DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Input);
				Ecual_DC_Motor_Intialize(MOTOR_PORT, Motor1, Output);
				Count = 0;
				Next = RSt22;
				break;
			case '2' :

				/* Set PIN Dimmer LOW*/
				Ecual_DC_Motor_Intialize(MOTOR_PORT, Motor1, Input);
				DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Output);
				Count = 0;
				Next = RSt22;
				break;
			case '0' :
				Count = 0;
				Next = RSt10;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1] AC Active");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2] AC OFF");
		switch(Local_u8Key)
		{
		case '1' :
			DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Input);
			Ecual_DC_Motor_Intialize(MOTOR_PORT, Motor1, Output);
			Next = ESt10;
			break;
		case '2' :
			Ecual_DC_Motor_Intialize(MOTOR_PORT, Motor1, Input);
			DIO_voidSetPinDirection(DIO_u8PORTA, DIO_PIN0, Output);
			/* Set PIN Dimmer LOW*/
			Next = ESt10;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt3;
			break;
		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
		break;
	}
	return Next;
}
static state APP_DoorControl(void){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;
	transmitDataBlockOverBluetooth("[1] DOOR OPEN \n",16);
	transmitDataBlockOverBluetooth("[2] DOOR CLOSE \n",17);
	while(Count){
		APP_SystemState();
		APP_TEMP_MOTOR();
		Read = UART_ReceiveCharfrist();
		switch(Read)
		{
		case '1' :
			DIO_voidSetPinDirection(DIO_u8PORTD,DIO_PIN5,Output);
			APP_FAST_PWM3_TIMER1();
			Next = RSt15;
			break;
		case '2' :
			DIO_voidSetPinDirection(DIO_u8PORTD, DIO_PIN5, Output);
			/* Set PIN SERVO MOTOR LOW*/
			TIMER1_voidSetICR1(0U);
			TIMER1_voidSetOCR1A(0U);
			Count = 0;
			Next = RSt15;
			break;
		case '0' :
			Count = 0;
			Next = RSt5;
			break;
		case 'C' :
			Count = 0;
			Next = RSt3;
			break;
		}
	}
	return Next;
}


static state APP_RoomsStatusMenu(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1]R1\n",7);
		transmitDataBlockOverBluetooth("[2]R2\n",7);
		transmitDataBlockOverBluetooth("[3]R3\n",7);
		transmitDataBlockOverBluetooth("[4]R4\n",7);
		transmitDataBlockOverBluetooth("[5]R5\n",7);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Count = 0;
				Next = RSt32;
				break;

			case '2' :
				Count = 0;
				Next = RSt33;
				break;
			case '3' :
				Count = 0;
				Next = RSt34;
				break;
			case '4' :
				Count = 0;
				Next = RSt35;
				break;
			case '5' :
				Count = 0;
				Next = RSt36;
				break;
			case '0' :
				Count = 0;
				Next = RSt6;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1]R1\n",7);
		transmitDataBlockOverBluetooth("[2]R2\n",7);
		transmitDataBlockOverBluetooth("[3]R3\n",7);
		transmitDataBlockOverBluetooth("[4]R4\n",7);
		transmitDataBlockOverBluetooth("[5]R5\n",7);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Count = 0;
				Next = RSt42;
				break;
			case '2' :
				Count = 0;
				Next = RSt43;
				break;
			case '3' :
				Count = 0;
				Next = RSt44;
				break;
			case '4' :
				Count = 0;
				Next = RSt45;
				break;
			case '5' :
				Count = 0;
				Next = RSt46;
				break;
			case '0' :
				Count = 0;
				Next = RSt11;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]R1");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]R2");
		CLCD_voidGoto(3,1);
		CLCD_voidWriteString("[3]R3");
		CLCD_voidGoto(4,1);
		CLCD_voidWriteString("[4]R4");
		CLCD_voidGoto(1,8);
		CLCD_voidWriteString("[5]R5");
		switch(Local_u8Key)
		{
		case '1' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt17;
			break;

		case '2' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt18;
			break;
		case '3' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt19;
			break;
		case '4' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt20;
			break;
		case '5' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt21;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt2;
			break;
		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
		break;
	}
	return Next;
}

static state APP_DimmerStatus(Login_t Copy_LoginType){
	Led_State Local_ReadDimmerState = 0;
	volatile uint_8 Read = 0;
	uint_8 Count = 1;
	switch(Copy_LoginType)
	{
	case Admin :
		/* Function to Read Dimmer State*/
		//Local_ReadDimmerState = GET_BIT(DDRD, DIO_PIN4);
		Local_ReadDimmerState = DIO_u8GetPinValue(DIO_u8PORTD, DIO_PIN4);
		//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
		switch(Local_ReadDimmerState)
		{
		case High :
			transmitDataBlockOverBluetooth("DIMMIR IS ON\n",14);
			break;
		case Low :
			transmitDataBlockOverBluetooth("DIMMIR IS OFF\n",15);
			break;
		}
		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt6;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			/* Function to Read Dimmer State*/
			//Local_ReadDimmerState = GET_BIT(DDRD, DIO_PIN4);
			Local_ReadDimmerState = DIO_u8GetPinValue(DIO_u8PORTD, DIO_PIN4);
			//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
			switch(Local_ReadDimmerState)
			{
			case High :
				transmitDataBlockOverBluetooth("DIMMIR IS ON\n",14);
				break;
			case Low :
				transmitDataBlockOverBluetooth("DIMMIR IS OFF\n",15);
				break;
			}
			Read = UART_ReceiveChar();
			while(Count && Read){

				APP_SystemState();
				APP_TEMP_MOTOR();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt11;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :
				/* Function to Read Dimmer State*/
				//Local_ReadDimmerState = GET_BIT(DDRD, DIO_PIN4);
				Local_ReadDimmerState = DIO_u8GetPinValue(DIO_u8PORTD, DIO_PIN4);
				//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
				switch(Local_ReadDimmerState)
				{
				case High :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("DIMMIR IS ON");
					break;
				case Low :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("DIMMIR IS OFF");
					break;
				}
				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt2;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}
static state APP_AC_Status(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;
	Motor_State  Local_u8DCMotorState = MotorStop;
	switch(Copy_LoginType)
	{
	case Admin :
		Local_u8DCMotorState = DIO_u8GetPinDirection(DIO_u8PORTD, DIO_PIN1);
		//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
		switch(Local_u8DCMotorState)
		{
		case Output :
			transmitDataBlockOverBluetooth("AC IS ACTIVE\n",11);

			break;
		case Input :
			transmitDataBlockOverBluetooth("AC IS OFF\n",10);
			break;
		default :
			transmitDataBlockOverBluetooth("AC IS OFF\n",10);
			break;
		}
		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt6;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			Local_u8DCMotorState = DIO_u8GetPinDirection(DIO_u8PORTD, DIO_PIN1);
			//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
			switch(Local_u8DCMotorState)
			{
			case Output :
				transmitDataBlockOverBluetooth("AC IS ACTIVE\n",11);
				break;
			case Input :
				transmitDataBlockOverBluetooth("AC IS OFF\n",10);
				break;
			default :
				transmitDataBlockOverBluetooth("AC IS OFF\n",10);
				break;
			}
			Read = UART_ReceiveChar();
			while(Count && Read){

				APP_SystemState();
				APP_TEMP_MOTOR();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt11;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :
				Local_u8DCMotorState = DIO_u8GetPinDirection(DIO_u8PORTD, DIO_PIN1);
				//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
				switch(Local_u8DCMotorState)
				{
				case Output :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("AC IS ACTIVE");
					break;
				case Input :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("AC IS OFF");
					break;
				default :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("AC IS OFF");
					break;
				}
				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt2;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}
static state APP_DoorStatus(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :

		/* Function to Read SERVO State*/
		Read = DIO_u8GetPinValue(DIO_u8PORTD, DIO_PIN5);
		//Local_ReadDimmerState = GET_BIT(PORTD, DIO_PIN4);
		//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
		switch(Read)
		{
		case High :
			transmitDataBlockOverBluetooth("DOOR IS OPEN\n",14);
			break;
		case Low :
			transmitDataBlockOverBluetooth("DOOR IS CLOSED\n",16);
			break;
		}
		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt6;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			/* Function to Read SERVO State*/
			Read = DIO_u8GetPinDirection(DIO_u8PORTD, DIO_PIN5);
			//Local_ReadDimmerState = GET_BIT(PORTD, DIO_PIN4);
			//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
			switch(Read)
			{
			case High :
				transmitDataBlockOverBluetooth("DOOR IS OPEN\n",14);
				break;
			case Low :
				transmitDataBlockOverBluetooth("DOOR IS CLOSED\n",10);
				break;
			}
			Read = UART_ReceiveChar();
			while(Count && Read){

				APP_SystemState();
				APP_TEMP_MOTOR();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt11;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :
				/* Function to Read SERVO State*/
				Read = DIO_u8GetPinDirection(DIO_u8PORTD, DIO_PIN5);
				//Local_ReadDimmerState = GET_BIT(PORTD, DIO_PIN4);
				//Ecual_DC_Motor_Status(MOTOR_PORT, Motor1);
				switch(Read)
				{
				case High :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("DOOR IS OPEN");
					break;
				case Low :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("DOOR IS CLOSED");
					break;
				}			switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt2;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}

static state APP_ROOM1_Control(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOM1 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM1 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[0]);
				Count = 0;
				Next = RSt27;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[0]);
				Count = 0;
				Next = RSt27;
				break;
			case '0' :
				Count = 0;
				Next = RSt12;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOM1 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM1 OFF\n",15);

		Read = UART_ReceiveChar();
		while(Count && Read){

			APP_SystemState();
			APP_TEMP_MOTOR();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[0]);
				Count = 0;
				Next = RSt37;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[0]);
				Count = 0;
				Next = RSt37;
				break;
			case '0' :
				Count = 0;
				Next = RSt20;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOM1 ON");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]ROOM1 OFF");
		switch(Local_u8Key)
		{
		case '1' :
			Ecual_Led_TurnOn(&LED1[0]);
			Next = ESt11;
			break;
		case '2' :
			Ecual_Led_TurnOff(&LED1[0]);
			Next = ESt11;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt8;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
	}
	return Next;
}
static state APP_ROOM2_Control(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOM2 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM2 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[1]);
				Count = 0;
				Next = RSt28;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[1]);
				Count = 0;
				Next = RSt28;
				break;
			case '0' :
				Count = 0;
				Next = RSt12;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOM2 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM2 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[1]);
				Count = 0;
				Next = RSt38;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[1]);
				Count = 0;
				Next = RSt38;
				break;
			case '0' :
				Count = 0;
				Next = RSt20;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOM2 ON");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]ROOM2 OFF");
		switch(Local_u8Key)
		{
		case '1' :
			Ecual_Led_TurnOn(&LED1[1]);
			Next = ESt11;
			break;
		case '2' :
			Ecual_Led_TurnOff(&LED1[1]);
			Next = ESt11;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt8;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
	}
	return Next;
}
static state APP_ROOM3_Control(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOM3 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM3 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[2]);
				Count = 0;
				Next = RSt29;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[2]);
				Count = 0;
				Next = RSt29;
				break;
			case '0' :
				Count = 0;
				Next = RSt12;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOM3 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM3 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[2]);
				Count = 0;
				Next = RSt39;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[2]);
				Count = 0;
				Next = RSt39;
				break;
			case '0' :
				Count = 0;
				Next = RSt20;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOM3 ON");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]ROOM3 OFF");
		switch(Local_u8Key)
		{
		case '1' :
			Ecual_Led_TurnOn(&LED1[2]);
			Next = ESt11;
			break;
		case '2' :
			Ecual_Led_TurnOff(&LED1[2]);
			Next = ESt11;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt8;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
	}
	return Next;
}
static state APP_ROOM4_Control(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOM4 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM4 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[3]);
				Count = 0;
				Next = RSt30;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[3]);
				Count = 0;
				Next = RSt30;
				break;
			case '0' :
				Count = 0;
				Next = RSt12;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOM4 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM4 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[3]);
				Count = 0;
				Next = RSt40;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[3]);
				Count = 0;
				Next = RSt40;
				break;
			case '0' :
				Count = 0;
				Next = RSt20;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOM4 ON");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]ROOM4 OFF");
		switch(Local_u8Key)
		{
		case '1' :
			Ecual_Led_TurnOn(&LED1[3]);
			Next = ESt11;
			break;
		case '2' :
			Ecual_Led_TurnOff(&LED1[3]);
			Next = ESt11;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt8;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
	}
	return Next;
}
static state APP_ROOM5_Control(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	switch(Copy_LoginType)
	{
	case Admin :
		transmitDataBlockOverBluetooth("[1] ROOM5 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM5 OFF\n",15);

		while(Count){
			APP_SystemState();
			APP_TEMP_MOTOR();
			Read = UART_ReceiveChar();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[4]);
				Count = 0;
				Next = RSt31;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[4]);
				Count = 0;
				Next = RSt31;
				break;
			case '0' :
				Count = 0;
				Next = RSt12;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

	case User_Remote :
		transmitDataBlockOverBluetooth("[1] ROOM5 ON\n",14);
		transmitDataBlockOverBluetooth("[2] ROOM5 OFF\n",15);

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '1' :
				Ecual_Led_TurnOn(&LED1[4]);
				Count = 0;
				Next = RSt41;
				break;
			case '2' :
				Ecual_Led_TurnOff(&LED1[4]);
				Count = 0;
				Next = RSt41;
				break;
			case '0' :
				Count = 0;
				Next = RSt20;
				break;
			case 'C' :
				Count = 0;
				Next = RSt4;
				break;
			}
		}
		break;
	case User_Keypad :
		CLCD_voidGoto(1,1);
		CLCD_voidWriteString("[1]ROOM5 ON");
		CLCD_voidGoto(2,1);
		CLCD_voidWriteString("[2]ROOM5 OFF");
		switch(Local_u8Key)
		{
		case '1' :
			Ecual_Led_TurnOn(&LED1[4]);
			Next = ESt11;
			break;
		case '2' :
			Ecual_Led_TurnOff(&LED1[4]);
			Next = ESt11;
			break;
		case 'R' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt8;
			break;

		case 'C' :
			CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
			Next = ESt1;
			break;
		}
	}
	return Next;
}

static state APP_ROOM1_Status(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	Led_State Local_u8LedState =LedOff;
	Ecual_Led_GetStatus(&LED1[0], &Local_u8LedState);

	switch(Copy_LoginType)
	{
	case Admin :

		switch(Local_u8LedState)
		{
		case LedOff :
			transmitDataBlockOverBluetooth("[1] ROOM1 OFF\n",14);
			break;
		case LedOn :
			transmitDataBlockOverBluetooth("[2] ROOM1 ON\n",15);
			break;
		}

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt16;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			switch(Local_u8LedState)
			{
			case LedOff :
				transmitDataBlockOverBluetooth("[1] ROOM1 OFF\n",14);
				break;
			case LedOn :
				transmitDataBlockOverBluetooth("[2] ROOM1 ON\n",15);
				break;
			}

			while(Count){
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt23;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :

				switch(Local_u8LedState)
				{
				case LedOff :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM1 IS OFF");
					break;
				case LedOn :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM1 IS ON");
					break;
				}

				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt4;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}
static state APP_ROOM2_Status(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	Led_State Local_u8LedState = LedOff;
	Ecual_Led_GetStatus(&LED1[1], &Local_u8LedState);

	switch(Copy_LoginType)
	{
	case Admin :

		switch(Local_u8LedState)
		{
		case LedOff :
			transmitDataBlockOverBluetooth("[1] ROOM2 OFF\n",14);
			break;
		case LedOn :
			transmitDataBlockOverBluetooth("[2] ROOM2 ON\n",15);
			break;
		}

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt16;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :

			switch(Local_u8LedState)
			{
			case LedOff :
				transmitDataBlockOverBluetooth("[1] ROOM2 OFF\n",14);
				break;
			case LedOn :
				transmitDataBlockOverBluetooth("[2] ROOM2 ON\n",15);
				break;
			}

			while(Count){
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt23;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :

				switch(Local_u8LedState)
				{
				case LedOff :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM2 IS OFF");
					break;
				case LedOn :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM2 IS ON");
					break;
				}

				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt4;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}

static state APP_ROOM3_Status(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	Led_State Local_u8LedState = LedOff;
	Ecual_Led_GetStatus(&LED1[2], &Local_u8LedState);

	switch(Copy_LoginType)
	{
	case Admin :

		switch(Local_u8LedState)
		{
		case LedOff :
			transmitDataBlockOverBluetooth("[1] ROOM3 OFF\n",14);
			break;
		case LedOn :
			transmitDataBlockOverBluetooth("[2] ROOM3 ON\n",15);
			break;
		}

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt16;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			switch(Local_u8LedState)
			{
			case LedOff :
				transmitDataBlockOverBluetooth("[1] ROOM3 OFF\n",14);
				break;
			case LedOn :
				transmitDataBlockOverBluetooth("[2] ROOM3 ON\n",15);
				break;
			}

			while(Count){
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt23;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :

				switch(Local_u8LedState)
				{
				case LedOff :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM3 IS OFF");
					break;
				case LedOn :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM3 IS ON");
					break;
				}

				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt4;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}
static state APP_ROOM4_Status(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	Led_State Local_u8LedState =LedOff;
	Ecual_Led_GetStatus(&LED1[3], &Local_u8LedState);

	switch(Copy_LoginType)
	{
	case Admin :

		switch(Local_u8LedState)
		{
		case LedOff :
			transmitDataBlockOverBluetooth("[1] ROOM4 OFF\n",14);
			break;
		case LedOn :
			transmitDataBlockOverBluetooth("[2] ROOM4 ON\n",15);
			break;
		}

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt16;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			switch(Local_u8LedState)
			{
			case LedOff :
				transmitDataBlockOverBluetooth("[1] ROOM4 OFF\n",14);
				break;
			case LedOn :
				transmitDataBlockOverBluetooth("[2] ROOM4 ON\n",15);
				break;
			}

			while(Count){
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt23;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :

				switch(Local_u8LedState)
				{
				case LedOff :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM4 IS OFF");
					break;
				case LedOn :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM4 IS ON");
					break;
				}

				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt4;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}


static state APP_ROOM5_Status(Login_t Copy_LoginType){
	volatile uint_8 Read = 0;
	uint_8 Count = 1;

	Led_State Local_u8LedState =LedOff;
	Ecual_Led_GetStatus(&LED1[4], &Local_u8LedState);

	switch(Copy_LoginType)
	{
	case Admin :

		switch(Local_u8LedState)
		{
		case LedOff :
			transmitDataBlockOverBluetooth("[1] ROOM5 OFF\n",14);
			break;
		case LedOn :
			transmitDataBlockOverBluetooth("[2] ROOM5 ON\n",15);
			break;
		}

		while(Count){
				APP_SystemState();
				APP_TEMP_MOTOR();
				Read = UART_ReceiveCharfrist();
			switch(Read)
			{
			case '0' :
				Count = 0;
				Next = RSt16;
				break;
			case 'C' :
				Count = 0;
				Next = RSt3;
				break;
			}
		}
		break;

		case User_Remote :
			switch(Local_u8LedState)
			{
			case LedOff :
				transmitDataBlockOverBluetooth("[1] ROOM5 OFF\n",14);
				break;
			case LedOn :
				transmitDataBlockOverBluetooth("[2] ROOM5 ON\n",15);
				break;
			}

			while(Count){
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
				switch(Read)
				{
				case '0' :
					Count = 0;
					Next = RSt23;
					break;
				case 'C' :
					Count = 0;
					Next = RSt4;
					break;
				}
			}
			break;
			case User_Keypad :

				switch(Local_u8LedState)
				{
				case LedOff :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM5 IS OFF");
					break;
				case LedOn :
					CLCD_voidGoto(1,1);
					CLCD_voidWriteString("ROOM5 IS ON");
					break;
				}

				switch(Local_u8Key)
				{
				case 'R' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt4;
					break;

				case 'C' :
					CLCD_voidSendCommand(CLCD_CLEAR_DIISPLAY);
					Next = ESt1;
					break;
				}
	}
	return Next;
}
/*** Ahmed Ashraf___V1 ********************************************************  **/
static state APP_Admin_ADD_User(void)
{
	volatile uint_8 ReadUserKeypad = readDataFromEEPROM(EEPROM_ADDR_COUNTER_KU);
	volatile uint_8 ReadUserReomte = readDataFromEEPROM(EEPROM_ADDR_COUNTER_RU);
	volatile static uint_8 count =0;
	volatile uint_8  Read =0;
	transmitDataBlockOverBluetooth("[1]Keypad User\n",16);
	transmitDataBlockOverBluetooth("[2]Remote User\n",16);
	transmitDataBlockOverBluetooth("[3]Back\n",9);
	transmitDataBlockOverBluetooth("[4]Main\n",9);
	uint_8 cou=0;
	while(!((Read >='0')&&(Read <='9'))){
		APP_SystemState();
		APP_TEMP_MOTOR();
		Read = UART_ReceiveCharfrist();
		switch(Read)
		{
		case '1':
			if((ReadUserKeypad == 0xff)  || ((ReadUserKeypad < 4)))
			{
				for(cou = 0; cou < 5; cou++)
				{
					if( 0xff == readDataFromEEPROM( (cou * 5) + EEPROM_ADDR_F_UsersKeypad))
					{
						break;
					}
				}
				transmitDataBlockOverBluetooth("User Name\n",11);
				count=0;
				while(count < 1)
				{
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
					if((Read >= '0')&&( Read <= '9'))
					{
						eeprom_write_byte(((uint_8*)(cou *5) + EEPROM_ADDR_F_UsersKeypad), Read -'0' );
						count++;
					}
				}
				transmitDataBlockOverBluetooth("Pass\n",6);
				count = 1;
				while((count < 5))
				{
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
					if((Read >= '0')&&( Read <= '9'))
					{
						eeprom_write_byte((uint_8*)(((cou *5)+ EEPROM_ADDR_F_UsersKeypad) +count), Read - '0');
						count++;
					}
				}

				ReadUserKeypad++;
				eeprom_write_byte((uint_8*)EEPROM_ADDR_COUNTER_KU, ReadUserKeypad);
				cou=0;
				Next = RSt7;
			}
			else
			{
				transmitDataBlockOverBluetooth("you have 5 Keypad Users\n",25);
				Next = RSt7;
			}
			break;
		case '2':
			if((ReadUserReomte == 0xff)  || ((ReadUserReomte < 4)))
			{
				for(cou = 0; cou < 5; cou++)
				{
					if( 0xff == readDataFromEEPROM( (cou * 5) + EEPROM_ADDR_F_UsersRemote))
					{
						break;
					}
				}
				transmitDataBlockOverBluetooth("User Name\n",11);
				count=0;
				while(count < 1)
				{
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
					if((Read >= '0')&&( Read <= '9'))
					{
						eeprom_write_byte(((uint_8*)(cou *5) + EEPROM_ADDR_F_UsersRemote), Read -'0' );
						count++;
					}
				}
				transmitDataBlockOverBluetooth("Pass\n",6);
				count = 1;
				while((count < 5))
				{
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
					if((Read >= '0')&&( Read <= '9'))
					{
						eeprom_write_byte((uint_8*)(((cou *5)+ EEPROM_ADDR_F_UsersRemote) +count), Read - '0');
						count++;
					}
				}
				ReadUserReomte++;
				eeprom_write_byte((uint_8*)EEPROM_ADDR_COUNTER_RU, ReadUserReomte);
				Next = RSt7;
			}
			else
			{
				transmitDataBlockOverBluetooth("you have 5 Remote Users\n",25);
				Next = RSt7;
			}
			cou=0;
			break;
		case '3':
			Next = RSt3;
			break;
		case '4':
			Next = RSt3;
			break;
		}
	}
	return Next;
}

static state APP_Admin_Remove_User(void)
{

	volatile uint_8 ReadUserKeypad = readDataFromEEPROM(EEPROM_ADDR_COUNTER_KU);
	volatile uint_8 ReadUserReomte = readDataFromEEPROM(EEPROM_ADDR_COUNTER_RU);
	volatile static uint_8 count =0;
	volatile uint_8  Read =0;
	uint_8 cou =0;
	transmitDataBlockOverBluetooth("[1]Keypad User\n",16);
	transmitDataBlockOverBluetooth("[2]Remote User\n",16);
	transmitDataBlockOverBluetooth("[3]Back\n",9);
	transmitDataBlockOverBluetooth("[4]Main\n",9);
	while(!((Read >='0')&&(Read <='9'))){
		APP_SystemState();
		APP_TEMP_MOTOR();
		Read = UART_ReceiveCharfrist();
		switch(Read)
		{
		case '1':
			if(ReadUserKeypad <=4)
			{
				transmitDataBlockOverBluetooth("User Name\n",11);
				count=0;
				while(count < 1)
				{
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
					if((Read >= '0')&&( Read <= '9'))
					{
						for(cou =0; cou < 5; cou++)
						{
							if((Read -'0') == readDataFromEEPROM( (cou * 5) + EEPROM_ADDR_F_UsersKeypad))
							{
								count++;
								ReadUserKeypad--;
								eeprom_write_byte((uint_8*)EEPROM_ADDR_COUNTER_KU, ReadUserKeypad);
								for(uint_8 cou2 =0; cou2 < 5; cou2++)
								{
									eeprom_write_byte((uint_8*)((cou * 5) + EEPROM_ADDR_F_UsersKeypad + cou2 ), 0xff);
								}
								break;
							}
						}
					}
				}
				Next = RSt8;
			}
			else
			{
				transmitDataBlockOverBluetooth("NO Keypad Users\n",25);
				Next = RSt8;
			}
			break;
		case '2':
			if(ReadUserReomte <=4)
			{
				transmitDataBlockOverBluetooth("User Name\n",11);
				count=0;
				while(count < 1)
				{
					APP_SystemState();
					APP_TEMP_MOTOR();
					Read = UART_ReceiveCharfrist();
					Read = UART_ReceiveChar();
					if((Read >= '0')&&( Read <= '9'))
					{
						for(uint_8 cou=0; cou <= 5; cou++)
						{
							if((Read -'0')  == readDataFromEEPROM( (cou * 5) + EEPROM_ADDR_F_UsersRemote))
							{
								count++;
								ReadUserReomte--;
								eeprom_write_byte((uint_8*)EEPROM_ADDR_COUNTER_RU, ReadUserReomte);
								for(uint_8 cou2 =0; cou2 < 5; cou2++)
								{
									eeprom_write_byte((uint_8*)( (cou * 5) + EEPROM_ADDR_F_UsersRemote + cou2), 0xff);
								}
								break;
							}
						}
					}
				}
				Next = RSt8;
			}
			else
			{
				transmitDataBlockOverBluetooth("NO Remote Users\n",25);
				Next = RSt8;
			}
			break;
		case '3':
			Next = RSt3;
			break;
		case '4':
			Next = RSt3;
			break;
		}
	}
	return Next;
}

static void APP_SystemState(void){
	APP_TempToDigit();

}
static void APP_TempToDigit(void){
	uint32_t reads_digital_value=0;
	uint16_t reads_analog_value=0;

	reads_digital_value=(ADC_u16ReadValue(CH_A0));

	reads_analog_value=(uint16_t)((((uint32_t)reads_digital_value*500UL)/256UL)+1U);

	CLCD_voidGoto(1,1);
	CLCD_voidWriteString("Temp = ");
	CLCD_voidWriteNumbers(reads_analog_value);
	CLCD_voidWriteData(2);

}
#ifdef EEPROM_

#endif
