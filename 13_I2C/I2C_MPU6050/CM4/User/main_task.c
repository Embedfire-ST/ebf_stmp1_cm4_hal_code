/*
 * main_task.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "main_task.h"



void Main_Config(void)      //配置函数
{
 	//MPU6050初始化
	MPU6050_Init();

	printf("I2C MPU6050 获取原始数据实验\n");
}



void Main_Task(void)        //主要的任务函数
{
	static short Acel[3];
	static short Gyro[3];
	static float Temp;

	printf("\r\n 这是一个I2C外设(MPU6050)读写测试例程 \r\n");



	//检测MPU6050
	if (MPU6050ReadID() == 1)
	{
		while(1)
		{
            MPU6050ReadAcc(Acel);
            printf("加速度：%8d %8d %8d",Acel[0],Acel[1],Acel[2]);
            MPU6050ReadGyro(Gyro);
            printf("    陀螺仪%8d %8d %8d",Gyro[0],Gyro[1],Gyro[2]);
            MPU6050_ReturnTemp(&Temp);
            printf("    温度%8.2f\r\n",Temp);

			HAL_Delay(500);
		}
	}
	else
	{
		printf("\r\n没有检测到MPU6050传感器！\r\n");
		while(1);
	}
}

