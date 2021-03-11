

#ifndef KEY_BSP_KEY_H_
#define KEY_BSP_KEY_H_


#include "main.h"
//引脚定义
/*******************************************************/
#define KEY1_PIN                  KEY1_Pin
#define KEY1_GPIO_PORT            KEY1_GPIO_Port

#define KEY2_PIN                  KEY2_Pin
#define KEY2_GPIO_PORT            KEY2_GPIO_Port

/*******************************************************/

/** 按键按下标置宏
   * 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
   * 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
   */
#define KEY_ON       1
#define KEY_OFF      0

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* KEY_BSP_KEY_H_ */

