/*
 * bsp_led.h
 *
 *  Created on: Nov 17, 2020
 *      Author: zhan
 */

#ifndef LED_BSP_LED_H_
#define LED_BSP_LED_H_

#include "main.h"

//引脚定义 当替换引脚时，只需要替换以下宏
/*******************************************************/
//LED1
#define LED1_PIN                  LED1_Pin
#define LED1_GPIO_PORT            LED1_GPIO_Port


//LED2
#define LED2_PIN                  LED2_Pin
#define LED2_GPIO_PORT            LED2_GPIO_Port


/** 控制LED灯亮灭的宏，
   * LED低电平亮，设置ON=0，OFF=1
   * 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
   */
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)       HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,a)
#define LED2(a)       HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,a)

/* 定义控制IO的宏 */
#define LED1_TOGGLE          HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF             HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,OFF)
#define LED1_ON              HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,ON)

/* 定义控制IO的宏 */
#define LED2_TOGGLE          HAL_GPIO_TogglePin(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF             HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,OFF)
#define LED2_ON              HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,ON)


void LED_GPIO_Config(void);

#endif /* LED_BSP_LED_H_ */


