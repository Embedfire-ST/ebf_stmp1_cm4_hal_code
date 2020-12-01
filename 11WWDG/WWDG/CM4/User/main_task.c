/*
 * main_task.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "main_task.h"


void Main_Config(void)      //配置函数
{
	LED_GPIO_Config();

}

//此函数的运行的时间大概为100-200ms，当函数不是在这个范围内运行则会触发窗口看门狗
void Wwdg_TestTask(uint16_t delay)
{
	HAL_Delay(delay);	//模拟需要执行特定时间的任务

}


void Main_Task(void)        //主要的任务函数
{

	while(1)
	{
		Wwdg_TestTask(90);			//模拟一个需要某个时间的函数
		HAL_WWDG_Refresh(&hwwdg1);  //喂狗

		LED1_TOGGLE;	//LED翻转


	}

}





