/*
 * App_config.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Dell
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#define NOT_PRESSED 0xff

#define NOT_STORED   0xFF
#define NOT_SELECTED 0xFF

#define BLOCK_MODE_TIME		   (uint32)20000
#define CHARACTER_PREVIEW_TIME (uint_16)300
#define DEGREES_SYMBOL		   (uint_8)0xDF

/*********************************** PIN Configuration ***********************************/
#define PASS_SIZE       (uint_8)4
#define TRIES_ALLOWED   (uint_8)3
#define PASSWORD_SYMBOL (uint_8)'*'
#define ASCII_ZERO      (uint_8)'0'

#define PASS_NOT_SET (uint_8)0xFF
#define PASS_SET     (uint_8)0x01

#define ADMIN_PASS_STATUS_ADDRESS (uint_16)0X100
#define GUEST_PASS_STATUS_ADDRESS (uint_16)0X101
#define EEPROM_ADMIN_ADDRESS      (uint_16)0X102
#define EEPROM_GUEST_ADDRESS      (uint_16)0X106
#define LOGIN_BLOCKED_ADDRESS     (uint_16)0X10A
/*****************************************************************************************/

/************************************   Login configurations *****************************/
#define NO_MODE (uint_8)0
#define ADMIN   (uint_8)1
#define GUEST   (uint_8)2
/*****************************************************************************************/

/************************************   Logic values *************************************/
#define FALSE   (uint_8)0
#define TRUE    (uint_8)1
/*****************************************************************************************/

#define CHECK_ADMIN_MODE        (uint_8)'0'
#define CHECK_GUEST_MODE        (uint_8)'1'

#define SELECT_ROOM1            (uint_8)'1'
#define SELECT_ROOM2            (uint_8)'2'
#define SELECT_ROOM3            (uint_8)'3'
#define SELECT_ROOM4            (uint_8)'4'
#define ADMIN_MORE_OPTION       (uint_8)'4'

#define SELECT_ROOM4_ADMIN      (uint_8)'1'
#define SELECT_TV               (uint_8)'2'
#define SELECT_AIR_CONDITIONING (uint_8)'3'
#define ADMIN_RET_OPTION        (uint_8)'4'

#define SELECT_SET_TEMPERATURE  (uint_8)'1'
#define SELECT_AIR_COND_CTRL    (uint_8)'2'
#define SELECT_AIR_COND_RET     (uint_8)'0'

/****************************   number of ticks to run timeout ***************************/
#define ADMIN_TIMEOUT (uint_16)3000
#define GUEST_TIMEOUT (uint_16)2000
/*****************************************************************************************/

/****************************   Show menu codes  *****************************************/
#define MAIN_MENU            (uint_8)1
#define MORE_MENU            (uint_8)2
#define ROOM1_MENU           (uint_8)3
#define ROOM2_MENU           (uint_8)4
#define ROOM3_MENU           (uint_8)5
#define ROOM4_MENU           (uint_8)6
#define TV_MENU              (uint_8)7
#define AIRCONDITIONING_MENU (uint_8)8
#define AIRCOND_CTRL_MENU    (uint_8)9
#define TEMPERATURE_MENU     (uint_8)10

#endif /* APP_CONFIG_H_ */
