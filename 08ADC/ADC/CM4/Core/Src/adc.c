/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config
  */
  hadc1.Instance = ADC1;	//ADC寄存器基地址
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;	//时钟分频因子
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;		//ADC的分辨率
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;		//ADC扫描选择
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;	//转换完成标志位
  hadc1.Init.LowPowerAutoWait = DISABLE;		//低功耗自动延时
  hadc1.Init.ContinuousConvMode = ENABLE;		//ADC连续转化模式选择
  hadc1.Init.NbrOfConversion = 1;			//单次转化通道数目。
  hadc1.Init.DiscontinuousConvMode = DISABLE;		//ADC单次转化模式选择
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;	//ADC外部触发源选择，这里选择软件触发
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;	//ADC外部触发极性
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_ONESHOT; //数据管理地址
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;  //发生溢出时的选择保存数据
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;	//数据不进行位移。即使用第低十六位。
  hadc1.Init.OversamplingMode = DISABLE;	//过采样模式
  if (HAL_ADC_Init(&hadc1) != HAL_OK)  //初始化ADC外设和常规配置
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;  //ADC双模式被禁用，即使用ADC独立模式
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK) //配置ADC多模式参数
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;	//ADC转换通道
  sConfig.Rank = ADC_REGULAR_RANK_1;//ADC转换顺序
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5; //ADC采用周期
  sConfig.SingleDiff = ADC_SINGLE_ENDED; //输入信号线类型  ADC通道结束设置为单端
  sConfig.OffsetNumber = ADC_OFFSET_NONE;//禁用ADC偏移:对所选ADC通道没有偏移校正
  sConfig.Offset = 0;				//偏移量为0
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)  //ADC通道配置
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
  if(IS_ENGINEERING_BOOT_MODE())
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLL4;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

  }

    /* ADC1 clock enable */
    __HAL_RCC_ADC12_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    ANA0     ------> ADC1_INP0
    */
    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_OPEN);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC12_CLK_DISABLE();

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
