
/*
 * Keypad_Cfg.h
 *
 * Created: 1/20/2020 7:46:51 PM
 *  Author: Mostafa Metwaly
 */ 
#ifndef KEYPAD_CFG_H_
#define KEYPAD_CFG_H_
/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "retval.h"
#include "Keypad.h"


/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/


#define KEYPAD_PULL_MODE		KEYPAD_PULL_UP

#define KEYPAD_PORT			GPIOC

#define KEYPAD_ROW1			BIT2
#define KEYPAD_ROW2			BIT3
#define KEYPAD_ROW3			BIT4

#define KEYPAD_COL1			BIT5
#define KEYPAD_COL2			BIT6
#define KEYPAD_COL3			BIT7



#endif
