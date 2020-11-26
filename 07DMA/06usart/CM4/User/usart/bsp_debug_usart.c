/*
 * bsp_debug_usart.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "./usart/bsp_debug_usart.h"



uint8_t rx_buf[RX_MAX];	 //串口3接收数据存储
uint8_t rx_flag = 0;		 //当一帧数据接收完成之后置1


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //接收完成回调函数
{

	static uint8_t *ucTemp = rx_buf;
	uint16_t bufSize;

	//接收到字符串结束符时,并重新接收
	if( *ucTemp == '\n')
	{
		bufSize = ucTemp - rx_buf +1;	//接收的字符大小
		ucTemp = rx_buf;
		HAL_UART_Receive_IT(huart, ucTemp, 1);
		rx_flag = 1;

		//启动DMA传输
		HAL_UART_Transmit_DMA(huart, ucTemp, bufSize);
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
void Usart_SendString(uint8_t *str)
{
   unsigned int k=0;
   while (*(str + k)!='\0')
   {
      HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k) ,1,HAL_MAX_DELAY);
      k++;
   }
}

//DMA串口发送完成回调函数
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	Usart_SendString("DMA串口回调函数\n");
//
//}














