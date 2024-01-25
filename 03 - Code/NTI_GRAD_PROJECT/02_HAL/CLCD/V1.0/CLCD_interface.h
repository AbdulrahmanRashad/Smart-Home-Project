/*
 * CLCD_intrface.h
 *
 *  Created on: Oct 10, 2023
 *      Author: Tahoun
 */

#ifndef CLCD_INTRFACE_H_
#define CLCD_INTRFACE_H_



/*-------------- Includes ---------------*/
#include "../../../03_MCAL/01-DIO/V1.0/DIO_interface.h"
#include "../../CLCD/V1.0/CLCD_confg.h"
/*-------------- Macro Declarations ---------------*/

/*-------------- Macro Functions Declarations  ---------------*/

/*-------------- Data Type Declarations ---------------*/

/*-------------- Software Interfaces Declarations "APIs" ---------------*/
void CLCD_voidInit(void);
void CLCD_voidSendCommand(uint_8 Copy_u8Command);
void CLCD_voidWriteData(uint_8 Copy_u8Data);
void CLCD_voidWriteString(char String[]);
void CLCD_voidGoto(uint_8 Copy_u8Row, uint_8 Copy_u8Coloum);
void CLCD_voidWriteNumbers(uint_32 Copy_u32Number);
void CLCD_voidWriteNumbersInBinary(uint_32 Copy_u32Number);
void CLCD_voidStoreCustomChar(uint_8 Copy_u8CharAddress, uint_8 customChar[8]);
void CLCD_voidWriteCustomChar(uint_8 Copy_u8CharAddress, uint_8 customChar[8], uint_8 copy_u8Line);
void CLCD_voidWriteArabicNames(uint_8 Copy_u8Name[][8], uint_8 copy_u8CharNumbers, uint_8 copy_u8Line);
void CLCD_voidWriteNum(uint_32 num);
void CLCD_voidWriteNumbersInHexa(uint_32 Copy_u32Number);

/*-------------- Module Data Sharing "Global Variable Declarations" ---------------*/
















#endif /* 03_ECUAL_CLCD_CLCD_INTRFACE_H_ */
