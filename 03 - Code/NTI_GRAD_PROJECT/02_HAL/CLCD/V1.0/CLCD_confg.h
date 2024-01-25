/*
 * CLCD_confg.h
 *
 *  Created on: Oct 12, 2023
 *      Author: Tahoun
 */

#ifndef CLCD_CONFG_H
#define CLCD_CONFG_H


#define CLCD_DATA_PORT			DIO_u8PORTC
#define CLCD_CONTROL_PORT		DIO_u8PORTD
#define CLCD_EN_PIN             DIO_PIN7
#define CLCD_RW_PIN             DIO_PIN1
#define CLCD_RS_PIN             DIO_PIN6

#define CLCD_FUNCTION_SET       0X38
#define CLCD_CLEAR_DIISPLAY     0X01

#endif /* CLCD_CONFG_H */
