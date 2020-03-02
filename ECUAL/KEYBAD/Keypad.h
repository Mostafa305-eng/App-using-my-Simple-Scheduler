/*
 * Keypad.h
 *
 * Created: 1/20/2020 7:46:51 PM
 *  Author: Mostafa Metwaly
 */ 

#ifndef KEYPAD_H_
#define KEYPAD_H_
/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "retval.h"



/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/


#define KEYPAD_ROWS			(3U)
#define KEYPAD_COLS			(3U)



#define KEYPAD_PULL_UP		(0xFFU)
#define KEYPAD_PULL_DOWN	(0x00U)



/******************************************************************************************
*                                                                                         *
*                                 PROTOTYPES                                              *
*																						  *
*																						  *
*******************************************************************************************/


/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the keypad module.
 * 							
 **************************************************************************/
extern ERROR_STATUS Keypad_init();

/**********************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function updates the keypad pressed key.
 * 							
 **********************************************************/
extern ERROR_STATUS Keypad_update();

/***********************************************************
 * Input: 
 * Output:
 *		  u8_keypadValue: the value of the pressed key.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function returns the keypad pressed key.
 * 							
 *********************************************************/
extern ERROR_STATUS Keypad_getPressedKey(uint8 * u8_keypadValue);
#endif