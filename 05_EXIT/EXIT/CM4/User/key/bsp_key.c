/*
 * bsp_key.c
 *
 *  Created on: Nov 17, 2020
 *      Author: zhan
 */


#include <./key/bsp_key.h>

void Key_GPIO_Config(void)
{

}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{

   /*检测是否有按键按下 */
   if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON )
   {

      /*等待按键释放 */
      while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON);
      return         KEY_ON;

   }

   else
      return KEY_OFF;

}



