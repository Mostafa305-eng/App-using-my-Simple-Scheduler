/*
 * ADC.c
 *
 * Created: 1/24/2020 12:01:24 AM
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
#include "registers.h"
#include "ADC.h"
#include "interrupt.h"
#include "DIO.h"

/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/
uint8 u8_InterruptMode;
Ptrfunc ptrf_ADC_cbk;

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define ADC_VOLTAGE_REF_MASK			(0xC0)
#define ADC_ADJUST_RS_MASK				(0x20)
#define ADC_CHANNEL_MASK				(0x1F)
#define MODE_MASK						(0xE0)
#define PRESCALER_MASK					(0x07)

#define ADC_ENABLE					(0x80)
#define ADSC						(0x40)
#define ADATE						(0x20)
#define ADIF						(0x10)
#define ADIE						(0x08)







/************************************************************************/
/*						FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/


/******************************************************************************************
 * Input: 
 *		  str_ADC_info: config struct
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the ADC module.
 * 							
 *********************************************************************************************/
extern ERROR_STATUS ADC_init(str_ADC_Cfg_t* pstr_ADC_info)
{
	uint8 u8_retval=ADC_BASE_ERR+SUCCESS;
	CLR_BIT(ADMUX,ADC_VOLTAGE_REF_MASK);
	switch (pstr_ADC_info->u8_ADC_voltageRef)
	{
	case AREF:
		SET_BIT(ADMUX,AREF);
		break;
	case AVCC:
		SET_BIT(ADMUX,AVCC);
		break;
	case INTERNAL_2_56:
		SET_BIT(ADMUX,INTERNAL_2_56);
		break;
	default:
		u8_retval=ADC_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
		break;
	}
	
	
	CLR_BIT(ADMUX,ADC_ADJUST_RS_MASK);
	switch (pstr_ADC_info->u8_ADC_adjustResult)
	{
	case ADC_LEFT_ADJUST:
		SET_BIT(ADMUX,ADC_LEFT_ADJUST);
		break;
	case ADC_RIGHT_ADJUST:
		SET_BIT(ADMUX,ADC_RIGHT_ADJUST);
		break;
	default:
		u8_retval=ADC_BASE_ERR+INVALID_ARGU_ERR;
		break;
	}
	
	CLR_BIT(ADMUX,ADATE);
	CLR_BIT(SFIOR,MODE_MASK);
	switch (pstr_ADC_info->u8_ADC_mode)
	{
	case SINGLE_CONVERSION_MODE	:
		break;
	case FREE_RUNNING_MODE:
		SET_BIT(ADMUX,ADATE);	
		break;
	case ANALOG_COMPARATOR_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,ANALOG_COMPARATOR_MODE);
		break;		
	case EXTERNAL_INTRRUPT_0_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,EXTERNAL_INTRRUPT_0_MODE);
		break;		
	case TIMER0_COMP_MATCH_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,TIMER0_COMP_MATCH_MODE);
		break;		
	case TIMER0_OVF_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,TIMER0_OVF_MODE);
		break;		
	case TIMER1_COMP_MATCH_B_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,TIMER1_COMP_MATCH_B_MODE);
		break;
	case TIMER1_OVF_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,TIMER1_OVF_MODE);
		break;
	case TIMER1_CAPTURE_MODE:
		SET_BIT(ADMUX,ADATE);
		SET_BIT(SFIOR,TIMER1_CAPTURE_MODE);
		break;
	default:
		u8_retval=ADC_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
		break;
	}
	
	CLR_BIT(ADCSRA,ADIE);
	switch (pstr_ADC_info->u8_ADC_interruptMode)
	{
	case ADC_POLLING_MODE:
		u8_InterruptMode=ADC_POLLING_MODE;
		SET_BIT(ADCSRA,ADC_POLLING_MODE);
		break;
	case ADC_INTERRUPT_MODE:
		u8_InterruptMode=ADC_INTERRUPT_MODE;		
		SET_BIT(ADCSRA,ADC_INTERRUPT_MODE);
		break;
	default :
		u8_retval=ADC_BASE_ERR+INVALID_ARGU_ERR;
		break;
	}
	
	CLR_BIT(ADCSRA,PRESCALER_MASK);
	switch (pstr_ADC_info->u8_ADC_prescaler)
	{
	case ADC_PRESCALER_2:
		SET_BIT(ADCSRA,ADC_PRESCALER_2);
		break;
	case ADC_PRESCALER_4:
		SET_BIT(ADCSRA,ADC_PRESCALER_4);
		break;
	case ADC_PRESCALER_8:
		SET_BIT(ADCSRA,ADC_PRESCALER_8);
		break;
	case ADC_PRESCALER_16:
		SET_BIT(ADCSRA,ADC_PRESCALER_16);
		break;
	case ADC_PRESCALER_32:
		SET_BIT(ADCSRA,ADC_PRESCALER_32);
		break;
	case ADC_PRESCALER_64:
		SET_BIT(ADCSRA,ADC_PRESCALER_64);
		break;
	case ADC_PRESCALER_128:
		SET_BIT(ADCSRA,ADC_PRESCALER_128);
		break;
	default:
		u8_retval=ADC_BASE_ERR+INVALID_ARGU_ERR;
		break;
	}
	
	
	
	if (pstr_ADC_info->u8_ADC_Channels<ADC1_ADC0_10x)
	{
			DIO_Cfg_st ADC_DIO={GPIOA,(pstr_ADC_info->u8_ADC_Channels+1),LOW};
			DIO_init(&ADC_DIO);
	}
	else
	{
		if (pstr_ADC_info->u8_ADC_Channels == ADC1_ADC0_10x || \
			pstr_ADC_info->u8_ADC_Channels == ADC1_ADC0_200x ||\
			pstr_ADC_info->u8_ADC_Channels == ADC0_ADC1_1x  )
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT0|BIT1,LOW};
			DIO_init(&ADC_DIO);			
		} 
		else if (pstr_ADC_info->u8_ADC_Channels == ADC3_ADC2_10x ||\
				pstr_ADC_info->u8_ADC_Channels == ADC3_ADC2_200x ||\
				pstr_ADC_info->u8_ADC_Channels == ADC3_ADC2_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT2|BIT3,LOW};
			DIO_init(&ADC_DIO);			
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC2_ADC1_1x ||\
				pstr_ADC_info->u8_ADC_Channels == ADC1_ADC2_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT2|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC3_ADC1_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT3|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC4_ADC1_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT4|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC5_ADC1_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT5|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC6_ADC1_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT6|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC7_ADC1_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT7|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC7_ADC1_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT7|BIT1,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC0_ADC2_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT2|BIT0,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC4_ADC2_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT2|BIT4,LOW};
			DIO_init(&ADC_DIO);
		}
		else if (pstr_ADC_info->u8_ADC_Channels == ADC5_ADC2_1x)
		{
			DIO_Cfg_st ADC_DIO={GPIOA,BIT2|BIT5,LOW};
			DIO_init(&ADC_DIO);
		}		
		else
		{
			u8_retval=ADC_BASE_ERR+RESOURCE_NOT_FOUND_ERR;
		}
	}
	
	if (u8_retval == ADC_BASE_ERR+SUCCESS)
	{
		SET_BIT(ADCSRA,ADC_ENABLE);
	}
	else
	{
		
	}
	
	
	if (NULL!= pstr_ADC_info->ptrf_ADC_cbk)
	{
		ptrf_ADC_cbk=pstr_ADC_info->ptrf_ADC_cbk;
	} 
	else
	{
		u8_retval=ADC_BASE_ERR+NULL_PTR_ERR;
	}
	return u8_retval;
}


/**************************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts the conversion of ADC.
 * 							
 **************************************************************************************/
extern ERROR_STATUS ADC_startConversion(uint8 u8_channel)
{
	uint8 u8_retval;	
	if (u8_channel > ADC5_ADC2_1x)
	{
		u8_retval=ADC_BASE_ERR+INVALID_ARGU_ERR;
	} 
	else
	{
		CLR_BIT(ADMUX,ADC_CHANNEL_MASK);
		SET_BIT(ADMUX,u8_channel);
	
		SET_BIT(ADCSRA,ADSC);

		u8_retval=ADC_BASE_ERR+SUCCESS;
	}
	return u8_retval;
	
}

/**************************************************************************************
 * Input: 
 *			ptru8_Result: the function will write the result in it.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function return the result of the  ADC conversion .
 * 							
 **************************************************************************************/

extern ERROR_STATUS ADC_ReadResult(uint16 *ptru16_Result)
{
	*ptru16_Result = 0;
	if (u8_InterruptMode == ADC_POLLING_MODE)
	{
			while(0 == GET_BIT(ADCSRA,ADIF));
			SET_BIT(ADCSRA,ADIF);
	}
	else
	{
		
	}
	*ptru16_Result |= ADCL;

	*ptru16_Result |= (ADCH<<8);
	
	return (ADC_BASE_ERR+SUCCESS);
}



/************************************************************************/
/*					ISR FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/


ISR(ADC_vect)
{
	if ( NULL != ptrf_ADC_cbk )
	{
		ptrf_ADC_cbk();
	}
	
}
