/*
 * LCD.c
 *
 * Created: 1/21/2020 4:22:25 PM
 *  Author: Mostafa Metwaly
 */ 

/************************************************************************/
/*				 I				INCLUDES						        */
/************************************************************************/
#include "DIO.h"
#include "softwareDelay.h"
#include "LCD.h"
#include "common_macros.h"
#include "retval.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define LCD_RS_PIN	BIT1 	//define MCU pin connected to LCD RS
#define LCD_RS_PORT	GPIOA

#define LCD_RW_PIN	BIT2 	//define MCU pin connected to LCD R/W
#define LCD_RW_PORT	GPIOA

#define LCD_EN_PIN	BIT3	//define MCU pin connected to LCD E
#define LCD_EN_PORT	GPIOA

#define LCD_D4_PIN	BIT4	//define MCU pin connected to LCD D4
#define LCD_D4_PORT	GPIOA

#define LCD_D5_PIN	BIT5	//define MCU pin connected to LCD D5
#define LCD_D5_PORT	GPIOA

#define LCD_D6_PIN	BIT6	//define MCU pin connected to LCD D6
#define LCD_D6_PORT	GPIOA

#define LCD_D7_PIN	BIT7	//define MCU pin connected to LCD D7
#define LCD_D7_PORT	GPIOA



/*
// cursor position to DDRAM mapping
//DB7: set DD RAM address*/

#define LCD_LINE0_DDRAMADDR		0x00
#define LCD_LINE1_DDRAMADDR		0x40
#define LCD_LINE2_DDRAMADDR		0x14
#define LCD_LINE3_DDRAMADDR		0x54
#define LCD_DDRAM_BASE_ADDRESSE	0x80

#define INITIALIZED					1
#define NOTINITIALIZED				0

#define UNLOCKING_LACTCH		1
#define LOCKING_LATCH			2


#define LOCKED					8
#define UNLOCKED				9
/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/

uint8 au8_CommandData[10];
uint8 u8_commandState;
uint8 au8_PrintedData[16];
uint8 u8_PrintedState;
volatile uint8 u8_PrintedDataCounter;
uint8 u8_CommandBufferCounter;
uint8 u8_DataBufferCounter;
uint8 u8_LatchState;
uint8 u8_CommandCounter;
uint8 u8_InitState;



/************************************************************************/
/*						 FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/
static void LCD_send_nibble(uint8 data){

	if(data&LCD_D4_PIN)
	{
		DIO_Write(LCD_D4_PORT, LCD_D4_PIN, HIGH);
	}
	else
	{
	DIO_Write(LCD_D4_PORT, LCD_D4_PIN, LOW);		
	}

	if(data&LCD_D5_PIN)
	{
		DIO_Write(LCD_D5_PORT, LCD_D5_PIN, HIGH);		
	}
	else
	{
		DIO_Write(LCD_D5_PORT, LCD_D5_PIN, LOW);
	}
	if(data&LCD_D6_PIN)
	{
		DIO_Write(LCD_D6_PORT, LCD_D6_PIN, HIGH);
	}
	else
	{
		DIO_Write(LCD_D6_PORT, LCD_D6_PIN, LOW);
	}
	if(data&LCD_D7_PIN)
	{
		DIO_Write(LCD_D7_PORT, LCD_D7_PIN, HIGH);
	}
	else
	{
		DIO_Write(LCD_D7_PORT, LCD_D7_PIN, LOW);
	}
}


static void LCD_config_pins(void){

	/* set LCD pins as outputs */
	DIO_Cfg_st str_LCDDio ={LCD_RS_PORT, LCD_RS_PIN, OUTPUT};
	DIO_init(&str_LCDDio);

	str_LCDDio.GPIO=LCD_RW_PORT;
	str_LCDDio.pins=LCD_RW_PIN;
	str_LCDDio.dir=OUTPUT;
	DIO_init(&str_LCDDio);
	
	str_LCDDio.GPIO=LCD_EN_PORT;
	str_LCDDio.pins=LCD_EN_PIN;
	str_LCDDio.dir=OUTPUT;
	DIO_init(&str_LCDDio);

	str_LCDDio.GPIO=LCD_D4_PORT;
	str_LCDDio.pins=LCD_D4_PIN;
	str_LCDDio.dir=OUTPUT;
	DIO_init(&str_LCDDio);

	str_LCDDio.GPIO=LCD_D5_PORT;
	str_LCDDio.pins=LCD_D5_PIN;
	str_LCDDio.dir=OUTPUT;
	DIO_init(&str_LCDDio);
	
	str_LCDDio.GPIO=LCD_D6_PORT;
	str_LCDDio.pins=LCD_D6_PIN;
	str_LCDDio.dir=OUTPUT;
	DIO_init(&str_LCDDio);

	str_LCDDio.GPIO=LCD_D7_PORT;
	str_LCDDio.pins=LCD_D7_PIN;
	str_LCDDio.dir=OUTPUT;
	DIO_init(&str_LCDDio);
}



ERROR_STATUS LCD_Send_Command(uint8 u8_data)
{

	DIO_Write(LCD_RS_PORT, LCD_RS_PIN, LOW);

	LCD_send_nibble(u8_data); // Higher nibble first
	
	LCD_Latch_Toggle();
	SwDelay_ms(1);
	LCD_Latch_Toggle();
	SwDelay_ms(1);

	LCD_send_nibble(u8_data<<4);
	LCD_Latch_Toggle();
	SwDelay_ms(1);
	LCD_Latch_Toggle();
	SwDelay_ms(1);
return LCD_BASE_ERR+SUCCESS;
}

ERROR_STATUS LCD_Send_Data(uint8 u8_data)
{

	DIO_Write(LCD_RS_PORT, LCD_RS_PIN, HIGH);

	LCD_send_nibble(u8_data); // Higher nibble first
	
	LCD_Latch_Toggle();
	SwDelay_ms(1);
	LCD_Latch_Toggle();
	SwDelay_ms(1);

	LCD_send_nibble(u8_data<<4);
	LCD_Latch_Toggle();
	SwDelay_ms(1);
	LCD_Latch_Toggle();
	SwDelay_ms(1);
return LCD_BASE_ERR+SUCCESS;
}


/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the  module.
 * 							
 **************************************************************************/

ERROR_STATUS LCD_init(void)
{
	LCD_config_pins();

	u8_LatchState=UNLOCKED;


	au8_CommandData[0]=(0x32);	
	LCD_Send_Command(0x32);

		
	au8_CommandData[0]=(FOUR_BIT);
	LCD_Send_Command(FOUR_BIT);

	

	au8_CommandData[0]=(CURSOR_ON);
	LCD_Send_Command(CURSOR_ON);



	au8_CommandData[0]=(INCREMENT_CURSOR);
	LCD_Send_Command(INCREMENT_CURSOR);
	


	u8_LatchState=LOCKED;
	u8_commandState=UPPER_NIBBLE;
	u8_PrintedState=UPPER_NIBBLE;
	au8_CommandData[0]=0;
	u8_CommandBufferCounter=0;
	u8_CommandCounter=0;	
	au8_PrintedData[0]=0;
	u8_PrintedDataCounter=0;
	u8_DataBufferCounter=0;
	u8_InitState=INITIALIZED;
	return LCD_BASE_ERR+SUCCESS;
}



/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function toggles the enable bit of LCD to latch the data
 * 							
 **************************************************************************/
void LCD_Latch_Toggle(void)
{
	if (u8_LatchState == UNLOCKED)
	{
		DIO_Toggle(LCD_EN_PORT, LCD_EN_PIN);
	}
}


/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function executes the main logic of the module in OS
 * 							
 **************************************************************************/
extern ERROR_STATUS LCD_Update(void)
{
	uint8 u8_retval=LCD_BASE_ERR+SUCCESS;
	u8_CommandBufferCounter=0;
	if (u8_InitState == NOTINITIALIZED)
	{
		u8_retval=LCD_BASE_ERR+NOT_INITIALIZED_ERR ;
	}
	else if (au8_CommandData[u8_CommandCounter] != 0 )
	{
		DIO_Write(LCD_RS_PORT, LCD_RS_PIN, LOW);
		switch (u8_commandState)
		{
		case UPPER_NIBBLE:
			u8_LatchState=UNLOCKED;
			LCD_send_nibble(au8_CommandData[u8_CommandCounter]); // Higher nibble first
			u8_commandState=LOWER_NIBBLE;
			break;
		case LOWER_NIBBLE:
			LCD_send_nibble((au8_CommandData[u8_CommandCounter])<<4);
			au8_CommandData[u8_CommandCounter]=0;
			u8_CommandCounter++;
			u8_commandState=LOCKING_LATCH;
			break;
		case LOCKING_LATCH :
			u8_LatchState=LOCKED;
			u8_commandState=UPPER_NIBBLE;
			break;
		default:
			break;
		}
	} 
	else if (au8_PrintedData[u8_PrintedDataCounter] !=0)
	{
		DIO_Write(LCD_RS_PORT, LCD_RS_PIN, HIGH);
		switch (u8_PrintedState)
		{
		case UPPER_NIBBLE:
			u8_LatchState=UNLOCKED;
			LCD_send_nibble(au8_PrintedData[u8_PrintedDataCounter]); // Higher nibble first
			u8_PrintedState=LOWER_NIBBLE;
			break;
		case LOWER_NIBBLE:
			LCD_send_nibble(((au8_PrintedData[u8_PrintedDataCounter])<<4));
			
			u8_PrintedState=LOCKING_LATCH;	
			break;
		case LOCKING_LATCH :
			u8_LatchState=LOCKED;
			u8_PrintedState=UPPER_NIBBLE;
			au8_PrintedData[u8_PrintedDataCounter]=0;
			u8_PrintedDataCounter++;		
		default:
			break;
		}
	} 
	else
	{
		u8_PrintedDataCounter=0;
		u8_CommandBufferCounter=0;
		u8_CommandCounter=0;
		u8_DataBufferCounter=0;
	}
return u8_retval;
}




/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the command required to be sent to 
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/
ERROR_STATUS LCD_SetCommand(uint8 u8_Data)
{
	uint8 u8_retval=LCD_BASE_ERR+SUCCESS;
	if (u8_CommandBufferCounter<10)
	{
	au8_CommandData[u8_CommandBufferCounter]=u8_Data;
	u8_CommandBufferCounter++;		
	} 
	else
	{
		u8_retval=LCD_BASE_ERR+BUFFER_FULL_ERR;
	}
	return u8_retval;
}

/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the character required to be sent to
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/

ERROR_STATUS LCD_PrintChar(uint8 u8_Data)
{
	uint8 u8_retval=LCD_BASE_ERR+SUCCESS;
	if (u8_DataBufferCounter<16)
	{
		au8_PrintedData[u8_DataBufferCounter]=u8_Data;
		u8_DataBufferCounter++;
	}
	else
	{
		u8_retval=LCD_BASE_ERR+BUFFER_FULL_ERR;
	}
	return u8_retval;
}

/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the string required to be sent to
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/
ERROR_STATUS LCD_PrintString(uint8 *pu8_Data)
{
	uint8 u8_loopCounter;
		uint8 u8_retval=LCD_BASE_ERR+SUCCESS;
	for (u8_loopCounter=0;u8_loopCounter<16;u8_loopCounter++)
	{
			if ('\0' == pu8_Data[u8_loopCounter])
			{
				break;
			}
			else
			{
				if (u8_DataBufferCounter<16)
				{
					au8_PrintedData[u8_DataBufferCounter]=pu8_Data[u8_loopCounter];
					u8_DataBufferCounter++;
				}
				else
				{
					u8_retval=LCD_BASE_ERR+BUFFER_FULL_ERR;
					break;
				}
			}
	}
		return u8_retval;
}

/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function write the position command required to be sent to 
 *				LCD in an array(buffer) and the update function will send it
 * 							
 **************************************************************************/

ERROR_STATUS LCD_SetPosition(uint8 x,uint8 y)
{
	uint8 u8_retval=LCD_BASE_ERR+SUCCESS;
	uint8 u8_PostionAddresse=0;
	// remap lines into proper order
	switch(y)
	{
		case 0:
			u8_PostionAddresse = LCD_LINE0_DDRAMADDR+x;
			break;
		case 1: 
			u8_PostionAddresse = LCD_LINE1_DDRAMADDR+x;
			break;
		case 2: 
			u8_PostionAddresse = LCD_LINE2_DDRAMADDR+x;
			break;
		case 3: 
			u8_PostionAddresse = LCD_LINE3_DDRAMADDR+x;
			break;
		default: 
			u8_retval=LCD_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
			break;
	}

	LCD_SetCommand((LCD_DDRAM_BASE_ADDRESSE | u8_PostionAddresse));
	return u8_retval;
}




