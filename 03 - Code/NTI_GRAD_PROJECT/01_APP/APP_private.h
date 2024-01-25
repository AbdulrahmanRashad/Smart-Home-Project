/*
 * APP_private.h
 *
 *  Created on: Oct 29, 2023
 *      Author: Dell
 */

#ifndef APP_PRIVATE_H_
#define APP_PRIVATE_H_



static uint_8 APP_Start_Choose_login(void);
static Std_Return APP_SetAdmin_and_Pass_First(void);

static state APP_LoginSystem(Login_t Copy_LoginType);
static Std_Return Check_AdminName(uint_8 *Copy_add);
static Std_Return Check_AdminPass(uint_8 *Copy_add);
static Std_Return Check_UsernameRemote(uint_8 *Copy_add);
static Std_Return Check_PassRemote(uint_8 *Copy_add);
static Std_Return Check_Usernamekeypad(uint_8 *Copy_add);
static Std_Return Check_PassKeypad(uint_8 *Copy_add);

static state APP_HomePageMode(Login_t Copy_LoginType);
static state APP_ControlMenu(Login_t Copy_LoginType);
static state APP_StatusMenu(Login_t Copy_LoginType);
static state APP_RoomsControlMenu(Login_t Copy_LoginType);
static state APP_DimmerControl(Login_t Copy_LoginType);
static state APP_AC_Control(Login_t Copy_LoginType);
static state APP_DoorControl(void);
static state APP_RoomsStatusMenu(Login_t Copy_LoginType);
static state APP_DimmerStatus(Login_t Copy_LoginType);
static state APP_AC_Status(Login_t Copy_LoginType);
static state APP_DoorStatus(Login_t Copy_LoginType);
static state APP_ROOM1_Control(Login_t Copy_LoginType);
static state APP_ROOM2_Control(Login_t Copy_LoginType);
static state APP_ROOM3_Control(Login_t Copy_LoginType);
static state APP_ROOM4_Control(Login_t Copy_LoginType);
static state APP_ROOM5_Control(Login_t Copy_LoginType);
static state APP_ROOM1_Status(Login_t Copy_LoginType);
static state APP_ROOM2_Status(Login_t Copy_LoginType);
static state APP_ROOM3_Status(Login_t Copy_LoginType);
static state APP_ROOM4_Status(Login_t Copy_LoginType);
static state APP_ROOM5_Status(Login_t Copy_LoginType);

static state APP_Admin_ADD_User(void);
static state APP_Admin_Remove_User(void);
static state APP_ChangeAdmin_Pass(void);

static void APP_SystemState(void);
static void APP_TempToDigit(void);




#endif /* 01_APP_APP_PRIVATE_H_ */
