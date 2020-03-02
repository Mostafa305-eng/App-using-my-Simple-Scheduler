/*
 * Keypad.c
 *
 * Created: 1/20/2020 7:46:51 PM
 *  Author: Mostafa Metwaly
 */ 
/******************************************************************************************
*                                                                                         *
*                                        INCLUDES                                         *
*																						  *
*																						  *
*******************************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "retval.h"
#include "DIO.h"
#include "Keypad_Cfg.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define FIRSTSAMPLE		10
#define SECONDSAMPLE	20
#define ZEROVALUE		0
#define NOTFOUND		5
#define FOUND			6

/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/


uint8 u8_SampleState;

uint8 au8_keypadValueTable[KEYPAD_ROWS][KEYPAD_COLS]= \
{	\
	{'1','2','3'}, \
	{'4','5','6'}, \
	{'7','8','9'}  \
};

uint8 au8_keypadFirstSample[KEYPAD_ROWS][KEYPAD_COLS];
uint8 u8_KeyPressed;
uint8 au8_keypadRowsMask[KEYPAD_ROWS]={KEYPAD_ROW1,KEYPAD_ROW2,KEYPAD_ROW3};
uint8 au8_keypadColsMask[KEYPAD_COLS]={KEYPAD_COL1,KEYPAD_COL2,KEYPAD_COL3};

/************************************************************************/
/*						FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/



/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the keypad module.
 * 							
 **************************************************************************/
extern ERROR_STATUS Keypad_init()
{
	DIO_Cfg_st KeypadDIO_Cfg_s={KEYPAD_PORT,KEYPAD_ROW1|KEYPAD_ROW2|KEYPAD_ROW3,HIGH};
	DIO_init(&KeypadDIO_Cfg_s);
	DIO_Write(KEYPAD_PORT,KEYPAD_ROW1|KEYPAD_ROW2|KEYPAD_ROW3,KEYPAD_PULL_MODE);
	
	KeypadDIO_Cfg_s.GPIO=KEYPAD_PORT;
	KeypadDIO_Cfg_s.pins=KEYPAD_COL1|KEYPAD_COL2|KEYPAD_COL3;
	KeypadDIO_Cfg_s.dir=LOW;
	DIO_init(&KeypadDIO_Cfg_s);
	DIO_Write(KEYPAD_PORT,KEYPAD_COL1|KEYPAD_COL2|KEYPAD_COL3,KEYPAD_PULL_MODE);
	
	u8_KeyPressed='0';
	u8_SampleState=FIRSTSAMPLE;
	
	return KEYPAD_BASE_ERR+SUCCESS;
}



/**********************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function updates the keypad pressed key.
 * 							
 **********************************************************/
extern ERROR_STATUS Keypad_update()
{
	uint8 u8_RowCounter,u8_ColCounter,u8_TempRead,u8_FoundFlag;	
	switch (u8_SampleState)
	{
	case FIRSTSAMPLE:
		for (u8_RowCounter=0;u8_RowCounter<KEYPAD_ROWS;u8_RowCounter++)
		{
			/*I write in DIO with pull up or down mode to works in both modes*/
			DIO_Write(KEYPAD_PORT,au8_keypadRowsMask[u8_RowCounter],!KEYPAD_PULL_MODE);
			for (u8_ColCounter=0;u8_ColCounter<KEYPAD_ROWS;u8_ColCounter++)
			{
				DIO_Read(KEYPAD_PORT,au8_keypadColsMask[u8_ColCounter],&u8_TempRead);
				au8_keypadFirstSample[u8_RowCounter][u8_ColCounter]=u8_TempRead;
				if (u8_TempRead==LOW)
				{
					u8_SampleState=SECONDSAMPLE;		
				} 
				else
				{
				}
				
			}
			DIO_Write(KEYPAD_PORT,au8_keypadRowsMask[u8_RowCounter],KEYPAD_PULL_MODE);
		}
		break;
	
	case SECONDSAMPLE:
		u8_FoundFlag=NOTFOUND;
		for (u8_RowCounter=0;u8_RowCounter<KEYPAD_ROWS;u8_RowCounter++)
		{
			DIO_Write(KEYPAD_PORT,au8_keypadRowsMask[u8_RowCounter],!KEYPAD_PULL_MODE);
			for (u8_ColCounter=0;u8_ColCounter<KEYPAD_ROWS;u8_ColCounter++)
			{
				DIO_Read(KEYPAD_PORT,au8_keypadColsMask[u8_ColCounter],&u8_TempRead);
				if (au8_keypadFirstSample[u8_RowCounter][u8_ColCounter]==u8_TempRead && \
					u8_TempRead==0 )
				{
					u8_KeyPressed=au8_keypadValueTable[u8_RowCounter][u8_ColCounter];
					u8_FoundFlag=FOUND;
					break;
				}
			}
			DIO_Write(KEYPAD_PORT,au8_keypadRowsMask[u8_RowCounter],KEYPAD_PULL_MODE);
			if (u8_FoundFlag == FOUND)
			{
				break;
			}
		}
		u8_SampleState=FIRSTSAMPLE;
		break;
	}
	
	
	return KEYPAD_BASE_ERR+SUCCESS;
}




/***********************************************************
 * Input: 
 * Output:
 *		  u8_keypadValue: the value of the pressed key.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function returns the keypad pressed key.
 * 							
 *********************************************************/
extern ERROR_STATUS Keypad_getPressedKey(uint8 * u8_keypadValue)
{
	*u8_keypadValue=u8_KeyPressed;
	u8_KeyPressed='0';
return KEYPAD_BASE_ERR+SUCCESS;
}