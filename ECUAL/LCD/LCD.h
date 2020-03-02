/*
 * LCD.h
 *
 * Created: 1/21/2020 4:22:11 PM
 *  Author: Mostafa Metwaly
 */ 


#ifndef LCD_H_
#define LCD_H_
/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/
#include "std_types.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/
/* LCD Commands definition */
#define LCD_CLEAR           0x01
#define RETURN_HOME         0x02
#define DECREMENT_CURSOR    0x04
#define SHIFT_DISPLAY_RIGHT 0x05
#define INCREMENT_CURSOR    0x06
#define SHIFT_DISPLAY_LEFT  0x07
#define SHIFT_CURSOR_LEFT   0x10
#define SHIFT_CURSOR_RIGHT  0x14
#define FOUR_BIT            0x28
#define EIGHT_BIT           0x38
#define CURSOR_OFF          0x0C
#define CURSOR_ON           0x0E

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
 * Description: This function initializes the  module.
 * 							
 **************************************************************************/

ERROR_STATUS LCD_init(void);


/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function toggles the enable bit of LCD to latch the data
 * 							
 **************************************************************************/
void LCD_Latch_Toggle(void);

/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function executes the main logic of the module in OS
 * 							
 **************************************************************************/
extern ERROR_STATUS LCD_Update(void);

/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the command required to be sent to 
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/

ERROR_STATUS LCD_SetCommand(uint8 u8_Data);


/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the character required to be sent to
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/

ERROR_STATUS LCD_PrintChar(uint8 u8_Data);



/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the string required to be sent to
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/
ERROR_STATUS LCD_PrintString(uint8 *pu8_Data);

/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the position command required to be sent to 
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/

ERROR_STATUS LCD_SetPosition(uint8 x,uint8 y);

#endif /* LCD_H_ */