/*
 * bsp_led.c
 *
 *  Created on: Nov 17, 2020
 *      Author: zhan
 */

#include "./led/bsp_led.h"


void LED_GPIO_Config(void)
{
   LED1_OFF;         //配置LED的初始状态为灭
   LED2_OFF;
}
