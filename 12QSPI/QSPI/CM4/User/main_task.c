/*
 * main_task.c
 *
 *  Created on: Nov 24, 2020
 *      Author: zhan
 */


#include "main_task.h"

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(Tx_Buffer) - 1)
#define RxBufferSize1   (countof(Tx_Buffer) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize     (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress



uint8_t Tx_Buffer[] = "stm32mp157 test test\n";
//uint8_t Tx_Buffer[18];
uint8_t Rx_Buffer[BufferSize];

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

uint8_t state = QSPI_ERROR;

void Main_Config(void)      //配置函数
{

	BSP_QSPI_FlASH_Init();		//板载flash初始化


	//将hold及wp引脚设置为高
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_SET);


}



void Main_Task(void)        //主要的任务函数
{

	uint32_t addr = FLASH_WriteAddress ;
	int state = QSPI_ERROR;

	printf("\r\n这是一个32M串行flash(W25Q128)间接模式读写实验(QSPI驱动) \r\n");

	/* 获取 Flash Device ID */
	DeviceID = QSPI_FLASH_ReadDeviceID();
	Delay( 200 );
	/* 获取 SPI Flash ID */
	FlashID = QSPI_FLASH_ReadID();
	//QSPI_Set_WP_High();

//	/*写状态寄存器*/

//	QSPI_FLASH_WriteStatusReg(1,0X00);
//	QSPI_FLASH_WriteStatusReg(2,0X00);
//	QSPI_FLASH_WriteStatusReg(3,0X00);

//	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
//	printf("\r\nFlash Status Reg1 is 0x%02X,\r\n", QSPI_FLASH_ReadStatusReg(1));
//	printf("\r\nFlash Status Reg2 is 0x%02X,\r\n", QSPI_FLASH_ReadStatusReg(2));
//	printf("\r\nFlash Status Reg3 is 0x%02X,\r\n", QSPI_FLASH_ReadStatusReg(3));
//	QSPI_Set_WP_TO_QSPI_IO();

	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{
		printf("\r\n检测到QSPI FLASH W25Q128 !\r\n");
		printf("\r\n正在擦除芯片的%d~%d的内容!\r\n", addr, addr+W25Q128JV_PAGE_SIZE);
		//state = BSP_QSPI_Erase_Chip();
		state = BSP_QSPI_Erase_Sector(addr);
		if(state == QSPI_OK)
		  printf("\r\n擦除成功!\r\n");
		else
		{
		  printf("\r\n擦除失败!\r\n");
		  while(1);
		}

		printf("\r\n正在向芯片%d地址写入数据，大小为%d!\r\n", addr, BufferSize);
			/* 将发送缓冲区的数据写到flash中 */
		BSP_QSPI_Write(Tx_Buffer, addr, BufferSize);



		printf("\r\n正在向芯片%d地址读取大小为%d的数据!\r\n", addr, BufferSize);
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		//if(QSPI_OK == BSP_QSPI_FastRead(Rx_Buffer, addr, BufferSize))
		if(QSPI_OK == BSP_QSPI_Read(Rx_Buffer, addr, BufferSize))
		{
			printf("\r\n读取成功!\r\n");
		}
		else
		{
			printf("\r\n读取失败!\r\n");
		}


		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

		if( PASSED == TransferStatus1 )
		{
			printf("\r\n读写%d地址测试成功!\r\n", addr);
		}
		else
		{
			printf("\r\n读写%d地址测试失败!\n\r", addr);
		}
	}// if (FlashID == sFLASH_ID)
	else
	{
		//LED_RED;
		printf("\r\n获取不到 W25Q256 ID!\n\r");
	}

	while(1);

}

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


