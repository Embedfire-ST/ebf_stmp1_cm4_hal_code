/*
 * bsp_basic_tim.c
 *
 *  Created on: Nov 30, 2020
 *      Author: zhan
 */

#include "./basic_tim/bsp_basic_tim.h"
#include "./led/bsp_led.h"



void BasicTim_Config(void)
{
	//启动TIM6
	HAL_TIM_Base_Start_IT(&htim6);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6 )
	{
		LED1_TOGGLE;
		//LED2_TOGGLE;
		//printf("HAL_TIM_PeriodElapsedCallback\n");
	}

}

