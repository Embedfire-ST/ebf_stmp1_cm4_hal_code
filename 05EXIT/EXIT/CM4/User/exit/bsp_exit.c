/*
 * user_it.c
 *
 *  Created on: Nov 18, 2020
 *      Author: zhan
 */



#include "./exit/bsp_exit.h"

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	//判断是按键1产生的中断还是按键2

	if(KEY1_Pin == GPIO_Pin)
	{
		LED1_TOGGLE;
	}

	if(KEY2_Pin == GPIO_Pin)
	{
		LED2_TOGGLE;
	}

}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

	if(KEY2_Pin == GPIO_Pin)
	{
		LED2_TOGGLE;
	}
}







