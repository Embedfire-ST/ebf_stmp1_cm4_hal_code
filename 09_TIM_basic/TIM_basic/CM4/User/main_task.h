/*
 * main_task.h
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */

#ifndef MAIN_TASK_H_
#define MAIN_TASK_H_


#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./basic_tim/bsp_basic_tim.h"

void Main_Config(void);      //配置函数
void Main_Task(void);        //主要的任务函数


#endif /* MAIN_TASK_H_ */
