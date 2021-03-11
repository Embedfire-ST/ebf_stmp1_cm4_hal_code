/*
 * bsp_debug_usart.h
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */

#ifndef BSP_DEBUG_USART_H_
#define BSP_DEBUG_USART_H_


#include "main.h"
#include "string.h"


extern UART_HandleTypeDef huart3;

#define RX_MAX  255				//串口接收的最大长度
#define UartHandle  huart3

extern uint8_t rx_buf[RX_MAX];	 //接收数据数组
extern uint8_t rx_flag;		 //当一帧数据接收完成之后置1


void Usart_Config(void);     			//配置函数
void Usart_SendString(uint8_t *str);	//发送字符串



#endif /* BSP_DEBUG_USART_H_ */



