/*
 * LM35.h
 *
 * Created: 1/24/2020 2:23:33 PM
 *  Author: Mostafa Metwaly
 */ 


#ifndef LM35_H_
#define LM35_H_

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
*                                 PROTOTYPES                                              *
*																						  *
*																						  *
*******************************************************************************************/


/**************************************************************************
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function initializes the LM35 module.
 * 							
 **************************************************************************/
extern ERROR_STATUS LM_init();


/**************************************************************************
 * Input: 
 * Output: pu8_temperature ==> value of reading
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function reads temperature from LM35 module.
 * 							
 **************************************************************************/
extern ERROR_STATUS LM_Read(uint8 *pu8_temperature);


#endif /* LM35_H_ */