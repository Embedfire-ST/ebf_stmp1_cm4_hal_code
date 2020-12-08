/*
 * bsp_debug_usart.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "./usart/bsp_debug_usart.h"



uint8_t rx_buf[RX_MAX];	 //串口3接收数据存储
uint8_t rx_flag = 0;		 //当一帧数据接收完成之后置1

//在STM32CubeIDe中使用的编译器的宏
#ifdef __GNUC__

//重定义printf函数
int __io_putchar(int ch)
{
   HAL_UART_Transmit(&UartHandle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
   return ch;
}

#endif


//在keil MDK上使用的编译器的宏
#ifdef __CC_ARM

//重定义printf函数
int fputc(int ch, FILE *f)
{
	 /* 发送一个字节数据到串口 USARTx */
	 HAL_UART_Transmit(&UartHandle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	 return (ch);
}

#endif


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //接收完成回调函数
{

	static uint8_t *ucTemp = rx_buf;

	//接收到字符串结束符时,并重新接收
	if( *ucTemp == '\n')
	{
		ucTemp++;
		*ucTemp = '\0';		//添加字符串结束符
		ucTemp = rx_buf;
		HAL_UART_Receive_IT(huart, ucTemp, 1);
		rx_flag = 1;
	}
	else
	{
		ucTemp++;
		HAL_UART_Receive_IT(huart, ucTemp, 1);
	}

}

void Usart_Config(void)      //配置函数
{
	//启用串口3接收
	HAL_UART_Receive_IT(&UartHandle, rx_buf, 1);
}


//串口发送函数
//串口发送函数
void Usart_SendString(uint8_t *str)
{
   unsigned int k=0;
   while (*(str + k)!='\0')
   {
      HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k) ,1,HAL_MAX_DELAY);
      k++;
   }
}

















