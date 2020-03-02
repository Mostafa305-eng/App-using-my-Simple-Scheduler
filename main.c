/*
 * LCD.c
 *
 * Created: 1/21/2020 4:15:16 PM
 * Author : Mostafa Metwaly
 */ 

#include "LCD.h"
#include "SOS.h"
#include "softwareDelay.h"
#include "interrupt.h"
#include "Keypad.h"
#include "ADC.h"
#include "registers.h"
#include "LM35.h"
#include <stdlib.h>




uint8 LmTempReading=0;
uint8 keypadtempread=0;

void App()
{

	char buffer[4]={0,0,0,0};
	itoa(LmTempReading,buffer,10);
	//LCD_SetCommand(LCD_CLEAR);
	LCD_SetPosition(0,0);
	LCD_PrintString("temp is ");
	LCD_PrintString(buffer);

	LCD_PrintString(" C");	
	LM_Read(&LmTempReading);

}

void AppTWO()
{
	LCD_SetPosition(0,1);
	LCD_PrintString("key is ");
	LCD_PrintChar(keypadtempread);
	
}
void keypadreading()
{
	
	Keypad_getPressedKey(&keypadtempread);
}

void keypadLogic()
{
	Keypad_update();
}
int main(void)
{

	LM_init();
	LCD_init();
	Keypad_init();
	SOS_Init();
	sei();
	SOS_CreateTask(LCD_Update,0,2,5);
	SOS_CreateTask(LCD_Latch_Toggle,0,1,4);
	SOS_CreateTask(App,100,200,3);
	SOS_CreateTask(AppTWO,200,200,6);
	SOS_CreateTask(keypadLogic,0,20,7);
	SOS_CreateTask(keypadreading,0,40,8);
	SOS_RUN();




}


