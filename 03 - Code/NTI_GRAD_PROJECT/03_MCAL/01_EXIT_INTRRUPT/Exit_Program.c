#include "Exit_Private.h"
#include "Exit_Config.h"
#include "Exit_Interface.h"

void (*Ptr_To_Int0)(void) = NULL;
void (*Ptr_To_Int1)(void) = NULL;
void (*Ptr_To_Int2)(void) = NULL;

void EXTI_init(void)
{
#if (INT0_ENABLE == ENABLE)
	/* SET INTERRUPT 0 TO FALLING EDGE */
	SET_BIT(MCUCR,ISC01);
	CLR_BIT(MCUCR,ISC00);
	/* Enable INT0 */
	SET_BIT(GICR,INT0); //PIE
	SET_BIT(SREG, 7);  //GIE
#endif
#if (INT1_ENABLE == ENABLE)
	/* SET INTERRUPT 1 TO FALLING EDGE */
	SET_BIT(MCUCR,ISC11);
	CLR_BIT(MCUCR,ISC10);
	/* Enable INT1 */
	SET_BIT(GICR,INT1);
#endif
#if (INT2_ENABLE == ENABLE)
	/* SET INTERRUPT 2 TO FALLING EDGE */
	SET_BIT(MCUCR,ISC2);
	/* Enable INT2 */
	SET_BIT(GICR,INT2);
#endif
}

void EXTI_SetCallBack(void (*ptr_to_fun)(void),uint_8 A_IntID){
	if (A_IntID == 0)
	{
		Ptr_To_Int0 = ptr_to_fun;
	}
	else if (A_IntID == 1)
	{
		Ptr_To_Int1 = ptr_to_fun;
	}
	else if (A_IntID == 2)
	{
		Ptr_To_Int2 = ptr_to_fun;
	}
}

// INT 0
void __vector_1 (void){
	if (Ptr_To_Int0 != NULL){
		Ptr_To_Int0();
	}
}

// INT 1
void __vector_2 (void){
	if (Ptr_To_Int1 != NULL){
		Ptr_To_Int1();
	}
}

// INT 2
void __vector_3 (void){
	if (Ptr_To_Int2 != NULL){
		Ptr_To_Int2();
	}
}
