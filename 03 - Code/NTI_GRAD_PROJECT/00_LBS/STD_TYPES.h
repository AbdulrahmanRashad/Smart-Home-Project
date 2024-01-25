/*****************************************************************************/
/* Author       	: 	AHMED ASHRAF                                         */
/* Origin Date  	: 	05/OCT/2023                                          */
/* Layer    	    : 	MCA Layer	                                         */
/* SWC	            : 	STD TYPES		                                     */
/* Version      	: 	1.0.0                                                */
/*****************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef unsigned char           uint_8  ; /*FOR ONE UNS BYTE*/
typedef signed char             sint_8  ;	/*FOR ONE SIN BYTE*/
typedef unsigned short          uint_16 ;	/*FOR TWO UNS BYTE*/
typedef signed short            sint_16 ;	/*FOR TWO SIN BYTE*/
typedef unsigned long    		uint_32 ;	/*FOR FOUR UNS BYTE*/
typedef signed long             sint_32 ;	/*FOR FOUR SIN BYTE*/
typedef unsigned long long      uint_64 ;	/*FOR EGHIT UNS BYTE*/
typedef signed long long        sint_64 ;	/*FOR EGHIT SIN BYTE*/
typedef float     		 		float_32;	/*FOR FOUR FLOAT BYTE*/
typedef double       	        float_64;	/*FOR EGHIT FLOAT BYTE*/


#define NULL	(void *)0   /* FOR NULL POINTER*/

#define OK 				0
#define NOK 			1
#define BUSY_FUNC 		1
#define NULL_PTR   		1

#endif /*STD_TYPES_H*/
