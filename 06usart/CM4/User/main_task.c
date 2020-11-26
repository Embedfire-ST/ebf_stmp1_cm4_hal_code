/*
 * main_task.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "main_task.h"



void Main_Config(void)      //配置函数
{
	//启动串口接收函数
	Usart_Config();

}


void Main_Task(void)        //主要的任务函数
{


	if(rx_flag == 1)
	{
		printf((char *)rx_buf);
		//HAL_UART_Transmit_IT(&UartHandle, rx_buf, strlen((char *)rx_buf));
		rx_flag = 0;
	}


}















