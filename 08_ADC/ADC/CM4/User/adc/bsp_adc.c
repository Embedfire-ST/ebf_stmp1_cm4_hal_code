/*
 * bsp_adc.c
 *
 *  Created on: Nov 27, 2020
 *      Author: zhan
 */


#include "./adc/bsp_adc.h"



uint16_t adcData[ADC_DMA_BUFMAX];
uint8_t adc_conv_flag = 0;		 //当ADC转换完成置1



//转换完成回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
    {
    	adc_conv_flag = 1;	//ADC转换标志位值1
    }
}

void Adc_Config(void)
{
	//启动ADC DMA传输
	//HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adcData,(uint32_t)255);
}

void Adc_StartDMA(void)
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adcData,(uint32_t)ADC_DMA_BUFMAX);
}






