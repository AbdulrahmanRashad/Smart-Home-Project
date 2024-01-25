/*
 * EEPROM_interface.h
 *
 *  Created on: Oct 23, 2023
 *      Author: Tahoun
 */
#ifndef EEPROM_INTERFACE_H
#define EEPROM_INTERFACE_H
#include <avr/io.h>
#include <avr/eeprom.h>


/******************************************************************************
****************************** Functions Prototypes ***************************
******************************************************************************/
void writeDataToEEPROM(uint16_t address, uint8_t data);
uint8_t readDataFromEEPROM(uint16_t address);
uint_8 EEPROM_u8ReadByte(uint_16 Copy_u16Address);
void EEPROM_voidReadeBlock(uint_16 Copy_u16Address, uint_8 *Ptr_u8Return, uint_16 Copy_u16Size);
void EEPROM_voidWriteByte(uint_16 Copy_u16Address, uint_8 Copy_u8Data);
void EEPROM_voidWriteBlock(uint_16 Copy_u16Address, uint_8 *Copy_u8Data, uint_16 Copy_u16Size);
void EEPROM_voidReadyInterruptEnable(void);
void EEPROM_voidReadyInterruptDisable(void);
void EEPROM_voidSetCallBack(void(*Local_ptr)(void));


#endif
/*** !comment : End of gaurd [EEPROM_INTERFACE_H] ****************************/
