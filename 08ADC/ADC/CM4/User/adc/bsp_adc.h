/*
 * bsp_adc.h
 *
 *  Created on: Nov 27, 2020
 *      Author: zhan
 */

#ifndef ADC_BSP_ADC_H_
#define ADC_BSP_ADC_H_


#include "main.h"

extern ADC_HandleTypeDef hadc1;  //需要使用到外部的ADC句柄


#define ADC_DMA_BUFMAX	 255		//DMA接收的数据数组做大值

extern uint16_t adcData[ADC_DMA_BUFMAX]; //接收adc原始数据数组
extern uint8_t adc_conv_flag;		 	 //当ADC转换完成置1

void Adc_Config(void);		//ADC配置函数
void Adc_StartDMA(void);	//启动adc的dma传输

#endif /* ADC_BSP_ADC_H_ */
