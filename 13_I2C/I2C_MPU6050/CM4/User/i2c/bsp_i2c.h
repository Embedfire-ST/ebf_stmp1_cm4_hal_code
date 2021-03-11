/*********************************************************************
File    : i2c.h
Purpose : 
**********************************************************************/
#ifndef __I2C_H__
#define __I2C_H__
/****************************** Includes *****************************/
#include "main.h"
#include "i2c.h"

/****************************** Defines *******************************/

extern I2C_HandleTypeDef hi2c1;

#define I2C_Handle  hi2c1


#define I2C_OWN_ADDRESS           0x00


//毫秒级延时(需要定时器支持)，或者重写Delay宏
#define Delay 		HAL_Delay

 
#define I2Cx_FLAG_TIMEOUT             ((uint32_t) 1000) //0x1100
#define I2Cx_LONG_TIMEOUT             ((uint32_t) (300 * I2Cx_FLAG_TIMEOUT)) //was300
 
 
/*引脚定义*/


/*信息输出*/
#define I2C_DEBUG_ON         1
#define I2C_DEBUG_FUNC_ON    0

#define I2C_INFO(fmt,arg...)           printf("<<-I2C-INFO->> "fmt"\n",##arg)
#define I2C_ERROR(fmt,arg...)          printf("<<-I2C-ERROR->> "fmt"\n",##arg)
#define I2C_DEBUG(fmt,arg...)          do{\
                                          if(I2C_DEBUG_ON)\
                                          printf("<<-I2C-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define I2C_DEBUG_FUNC()               do{\
                                         if(I2C_DEBUG_FUNC_ON)\
                                         printf("<<-I2C-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


																			 

//unsigned short Get_I2C_Retry(void);
																			 
int Sensors_I2C_ReadRegister(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len, 
                                       unsigned char *data_ptr);
int Sensors_I2C_WriteRegister(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len, 
                                        unsigned char *data_ptr);


#endif // __I2C_H__


