/*
 * bsp_wwdg.c
 *
 *  Created on: Dec 1, 2020
 *      Author: zhan
 */

#include "./wwdg/bsp_wwdg.h"
#include "./led/bsp_led.h"


void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	if( hwwdg->Instance == WWDG1)
	{
		printf("系统即将复位\n");
	}

}
