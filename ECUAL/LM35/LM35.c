/*
 * LM35.c
 *
 * Created: 1/24/2020 2:30:03 PM
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
#include "LM35.h"
#include "ADC.h"
#include "LM35_CFG.h"

/******************************************************************************************
*                                                                                         *
*                                        DEFINES                                          *
*																						  *
*																						  *
*******************************************************************************************/
#define CONVERT_TO_CELLICOUS		4.88/10

/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/
uint16 u16_ADCReading;


/************************************************************************/
/*						FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/


void LM_ReadingADC_CBK(void)
{
	ADC_ReadResult(&u16_ADCReading);
}
/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the LM35 module.
 * 							
 **************************************************************************/
extern ERROR_STATUS LM_init()
{
	str_ADC_Cfg_t str_ADC_Cfg={AREF,ADC_RIGHT_ADJUST,SINGLE_CONVERSION_MODE,ADC_INTERRUPT_MODE\
		,ADC_PRESCALER_128,ADC0,LM_ReadingADC_CBK};
	
	ADC_init(&str_ADC_Cfg);
	
	u16_ADCReading=0;	
return ADC_BASE_ERR+SUCCESS;
}

/**************************************************************************
 * Input: 
 * Output: pu8_temperature ==> value of reading
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function reads temperature from LM35 module.
 * 							
 **************************************************************************/
extern ERROR_STATUS LM_Read(uint8 *pu8_temperature)
{
	*pu8_temperature = (u16_ADCReading * CONVERT_TO_CELLICOUS);
	u16_ADCReading=0;
return ADC_BASE_ERR+SUCCESS;
}