
/*
 * ADC.h
 *
 * Created: 1/2/2020 12:01:24 AM
 *  Author: Mostafa Metwaly
 */ 
#ifndef ADC_H_
#define ADC_H_
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
#define AREF						(0x00)
#define AVCC						(0x40)
#define INTERNAL_2_56				(0xC0)

#define ADC_LEFT_ADJUST				(0x20)
#define ADC_RIGHT_ADJUST			(0x00)

#define ADC0						(0x00)
#define ADC1						(0x01)
#define ADC2						(0x02)
#define ADC3						(0x03)
#define ADC4						(0x04)
#define ADC5						(0x05)
#define ADC6						(0x06)
#define ADC7						(0x07)
#define ADC1_ADC0_10x				(0x09)
#define ADC1_ADC0_200x				(0x0B)
#define ADC3_ADC2_10x				(0x0D)
#define ADC3_ADC2_200x				(0x0F)
#define ADC0_ADC1_1x				(0x10)
#define ADC2_ADC1_1x				(0x12)
#define ADC3_ADC1_1x				(0x13)
#define ADC4_ADC1_1x				(0x14)
#define ADC5_ADC1_1x				(0x15)
#define ADC6_ADC1_1x				(0x16)
#define ADC7_ADC1_1x				(0x17)
#define ADC0_ADC2_1x				(0x18)
#define ADC1_ADC2_1x				(0x19)
#define ADC3_ADC2_1x				(0x1B)
#define ADC4_ADC2_1x				(0x1C)
#define ADC5_ADC2_1x				(0x1D)

#define SINGLE_CONVERSION_MODE		(0xFF)
#define FREE_RUNNING_MODE			(0x00)
#define ANALOG_COMPARATOR_MODE		(0x20)
#define EXTERNAL_INTRRUPT_0_MODE	(0x40)
#define TIMER0_COMP_MATCH_MODE		(0x60)
#define TIMER0_OVF_MODE				(0x80)
#define TIMER1_COMP_MATCH_B_MODE	(0xA0)
#define TIMER1_OVF_MODE				(0xC0)
#define TIMER1_CAPTURE_MODE			(0xE0)

#define ADC_INTERRUPT_MODE			(0x08)
#define ADC_POLLING_MODE			(0x00)

#define ADC_PRESCALER_2				(0x01)
#define ADC_PRESCALER_4				(0x02)
#define ADC_PRESCALER_8				(0x03)
#define ADC_PRESCALER_16			(0x04)
#define ADC_PRESCALER_32			(0x05)
#define ADC_PRESCALER_64			(0x06)
#define ADC_PRESCALER_128			(0x07)


/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*																						  *
*																						  *
*******************************************************************************************/
typedef struct {
	uint8 u8_ADC_voltageRef;
	uint8 u8_ADC_adjustResult;
	uint8 u8_ADC_mode;
	uint8 u8_ADC_interruptMode;
	uint8 u8_ADC_prescaler;
	uint8 u8_ADC_Channels;
	Ptrfunc ptrf_ADC_cbk;
	}str_ADC_Cfg_t;

/******************************************************************************************
*                                                                                         *
*                               EXTERN VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/





/******************************************************************************************
*                                                                                         *
*                                 PROTOTYPES                                              *
*																						  *
*																						  *
*******************************************************************************************/


/******************************************************************************************
 * Input: 
 *		  str_ADC_info: config struct
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the ADC module.
 * 							
 *********************************************************************************************/
extern ERROR_STATUS ADC_init(str_ADC_Cfg_t* pstr_ADC_info);


/**************************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts the conversion of ADC.
 * 							
 **************************************************************************************/
extern ERROR_STATUS ADC_startConversion(uint8 u8_channel);




/**************************************************************************************
 * Input: 
 *			ptru8_Result: the function will write the result in it.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function return the result of the  ADC conversion .
 * 							
 **************************************************************************************/

extern ERROR_STATUS ADC_ReadResult(uint16 *ptru16_Result);

#endif