/*
 * SOS.h
 *
 * Created: 1/11/2020 6:21:48 PM
 *  Author: Mostafa Metwaly
 */ 

#ifndef SOS_H_
#define SOS_H_




/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/


#include "std_types.h"
#include "common_macros.h"


/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/
#define TIMER_ID0 0
#define TIMER_ID1 1
#define TIMER_ID2 2




typedef struct str_SOS_cfg_t
{
	uint8 u8_TimerId;
	uint16 u16_Resolution;
	}str_SOS_cfg_t; 



/************************************************************************/
/*						FUNCTIONS' PROTOTYPES		        */
/************************************************************************/



/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the OS.
 * 	cautions : if you choose timer 0 or 2 you have only range 1:16ms 
 * it is recommended to use timer 1 , it supports 1ms:4sec
 **************************************************************************/

ERROR_STATUS SOS_Init(void);







/******************************************************************************
 * fun
 * Input:  
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: call the functions when their time have come .
 * 							
 **************************************************************************/

void SOS_RUN( void );






/******************************************************************************
 * fun
 * Input:  
 *		----> Func_ptr	: pointer to task which required to excute
 *		----> period	: the time of the period between each call
 *							===>if the period is zero , it will excute the task only
 *								once	
 *		----> priority	: determine the pirioty of the task 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: create the task and make the OS sees it .
 * 							
 **************************************************************************/

ERROR_STATUS SOS_CreateTask( Ptrfunc Ptrfunc_Task,uint16 u16_StartTime, uint16 u16_Period ,\
						uint8 u8_Priority );





/******************************************************************************
 * fun
 * Input:  
 *		----> Func_ptr	: pointer to Task which required to deleted
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: delete the entered task from OS .
 * 							
 **************************************************************************/

ERROR_STATUS SOS_Delete( Ptrfunc Ptrfunc_Task  );





#endif /* SOS_H_ */