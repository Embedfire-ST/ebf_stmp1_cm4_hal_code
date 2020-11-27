/*
 * main_task.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "main_task.h"


void Main_Config(void)      //配置函数
{
	//启动ADC DMA传输
	Adc_StartDMA();

}

static uint16_t i;
static uint32_t temp;
static float val;

void Main_Task(void)        //主要的任务函数
{

	if(adc_conv_flag == 1)
	{
		//计算得到电压值
		for(i = 0;i <ADC_DMA_BUFMAX;i++)
		{
			temp +=  adcData[i];
		}
		val = (float)temp/ADC_DMA_BUFMAX/65535*3.3;
		printf("原始值为:%lu  电压值为：%f\n",temp/ADC_DMA_BUFMAX,val);

		//重新开始转换
		adc_conv_flag = 0;
		temp = 0;
		Adc_StartDMA();

		HAL_Delay(500);
	}

}





