/*
 * main_task.c
 *
 *  Created on: Nov 17, 2020
 *      Author: zhan
 */



#include "main_task.h"

void Main_Config(void)
{
	LED_GPIO_Config();                //初始化LED状态为灭
}

void Main_Task(void)
{

	if(Key_Scan(KEY1_GPIO_Port,KEY1_Pin) == KEY_ON)
	{
		LED1_TOGGLE;
	}

	if(Key_Scan(KEY2_GPIO_Port,KEY2_Pin) == KEY_ON)
	{
		LED2_TOGGLE;
	}

}

