/*
 * APP_interface.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Dell
 */

#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

/*-------------- Includes ---------------*/
#include "../00_LBS/BIT_MATH.h"
#include "../00_LBS/STD_TYPES.h"
#include "APP_interface.h"
#include "APP_config.h"

#include "../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../02_HAL/BUZZER/V1/BUZZER_interface.h"
#include "../02_HAL/MOTOR/MOTOR_interface.h"
#include "../02_HAL/KEYPAD/V1.0/KEYPAD_interface.h"
#include "../02_HAL/CLCD/V1.0/CLCD_interface.h"
#include "../02_HAL/LED/V1.0/LED_interface.h"
#include "../02_HAL/SERVO/V1/SERVO_interface.h"
#include "../02_HAL/TEMP/V1/TEMP_interface.h"
#include "../03_MCAL/EEPROM/EEPROM_interface.h"

/*-------------- Macro Declarations ---------------*/
#define EEPROM_ADDR_F_UsersKeypad    0x00/*25 add to 5 userskeypad from 0x00 to 0x18 */
#define EEPROM_ADDR_F_UsersRemote    0x19 /*25 add to 5 usersremote from 0x19 to 0x32 */
#define EEPROM_ADDR_F_Admin 	     0x32 /*5 add to 1 admin from 0x33 to 0x37 */
#define EEPROM_ADDRESS0 0x00
#define EEPROM_ADDRESS1 0x05
#define EEPROM_ADDRESS2 0x0A
#define EEPROM_ADDRESS3 0x0F
#define EEPROM_ADDRESS4 0x14

#define EEPROM_ADDR_COUNTER_KU 0x40
#define EEPROM_ADDR_COUNTER_RU 0x41
// && (readDataFromEEPROM(EEPROM_ADDR_COUNTER_KU) <= 4)
/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/
typedef enum{
	R_OK,
	R_NOT_OK,
}Std_Return;

typedef enum{
	User_Keypad,
	User_Remote,
	Admin,
}Login_t;

typedef enum States
{
	/* For Emergency System */
    ESt0,  // Login as user keypad
    ESt1,  // Main Emergency
    ESt2,  // Home Status
	ESt3,  // Control
	ESt4,  // Rooms Select (Status)
	ESt5,  // Door (Status)
	ESt6,  // AC (Status)
	ESt7,  // Dimmer (Status)
	ESt8,  // Rooms Select (Control)
	ESt9,  // Dimmer (Control)
	ESt10, // AC (Control)
	ESt11, // Room1 (Control)
	ESt12, // Room2 (Control)
	ESt13, // Room3 (Control)
	ESt14, // Room4 (Control)
	ESt15, // Room5 (Control)
	//ESt16, // Dimmer Intensity (Control)
	ESt17, // Room1 (Status)
	ESt18, // Room2 (Status)
	ESt19, // Room3 (Status)
	ESt20, // Room4 (Status)
	ESt21, // Room5 (Status)

	/* For Remote System */
	RSt0, // Start Choose
	RSt1, // Login As Admin
	RSt2, // Login as Remote user
	RSt3, // Admin mode
	RSt4, // user Remote Mode
	RSt5, // Control Admin
	RSt6, // All Remote Status ------> Status Admin
	RSt7, // Add user
	RSt8, // Remove User
	RSt9, // Change Password
	RSt10, // Control Remote user
	RSt11, // all Remote status -----> Remote user
	RSt12, // Rooms Control by Admin
	RSt13, // Dimmer Control by Admin
	RSt14, // AC Control by Admin
	RSt15, // Door Control by Admin
	RSt16, // Rooms Status Admin
	RSt17, // Dimmer Status Admin
	RSt18, // AC Status Admin
	RSt19, // Door Status Admin
	RSt20, // Rooms control Remote
	RSt21, // Dimmer control Remote
	RSt22, // AC control Remote
	RSt23, // Rooms Status Remote
	RSt24, // Dimmer Status Remote
	RSt25, // AC Status Remote
	RSt26, // Door Status Remote
	RSt27, // Room1 control Admin
	RSt28, // Room2 control Admin
	RSt29, // Room3 control Admin
	RSt30, // Room4 control Admin
	RSt31, // Room5 control Admin
	RSt32, // Room1 Status Admin
	RSt33, // Room2 Status Admin
	RSt34, // Room3 Status Admin
	RSt35, // Room4 Status Admin
	RSt36, // Room5 Status Admin
	RSt37, // Room1 control Remote
	RSt38, // Room2 control Remote
	RSt39, // Room3 control Remote
	RSt40, // Room4 control Remote
	RSt41, // Room5 control Remote
	RSt42, // Room1 Status Remote
	RSt43, // Room2 Status Remote
	RSt44, // Room3 Status Remote
	RSt45, // Room4 Status Remote
	RSt46, // Room5 Status Remote
}state;
/*-------------- Software Interfaces Declarations "APIs" ---------------*/
void APP_voidIntialize(void);
void APP_voidWelcom(void);
void APP_voidFirstRun(void);
void APP_ADC_CONTROL_DIMMER(void);
void APP_SERVO_INTILIZE(void);
void APP_FAST_PWM_TIMER1(void);
void APP_FAST_PWM2_TIMER1(void);
void APP_TEMP_MOTOR(void);
state APP_StateTrans(state St);
void APP_BlockingMode(void);
void APP_FAST_PWM3_TIMER1(void);
/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/


#endif /* APP_INTERFACE_H_ */
