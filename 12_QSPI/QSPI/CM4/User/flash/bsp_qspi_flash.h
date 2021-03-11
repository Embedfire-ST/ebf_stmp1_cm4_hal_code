#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include <stdio.h>
#include "main.h"
#include "quadspi.h"


#define QSPIHandle hqspi 

/* Private typedef -----------------------------------------------------------*/
//#define  sFLASH_ID                         0XEF4017     //W25Q64JVEM
//#define  sFLASH_ID                         0XEF4019     //W25Q256JVEM
#define  sFLASH_ID                         0XEF4018     //W25Q128JVEM


/* QSPI Error codes */
#define QSPI_OK            ((uint8_t)0x00)
#define QSPI_ERROR         ((uint8_t)0x01)
#define QSPI_BUSY          ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED ((uint8_t)0x04)
#define QSPI_SUSPENDED     ((uint8_t)0x08)


/* W25Q128JV Micron memory */
/* Size of the flash */
#define QSPI_FLASH_SIZE            24     /* 地址总线宽度访问整个内存空间 */
#define QSPI_PAGE_SIZE             256

/* QSPI Info */
typedef struct {
  uint32_t FlashSize;          /*!< 闪存大小 */
  uint32_t EraseSectorSize;    /*!< 擦除操作的扇区大小 */
  uint32_t EraseSectorsNumber; /*!< 擦除操作的扇区数 */
  uint32_t ProgPageSize;       /*!< 编程操作的页面大小 */
  uint32_t ProgPagesNumber;    /*!< 编程操作的页面数 */
} QSPI_Info;

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
/** 
  * @brief  W25Q128JV配置
  */  
#define W25Q128JV_FLASH_SIZE                  (16*1024*1024) /*  总的内存大小为16M字节 */
#define W25Q128JV_SECTOR_SIZE                 (64*1024)   	 /*  每个块的大小为64k字节 */
#define W25Q128JV_SUBSECTOR_SIZE              (4*1024)    	 /*  每个扇区的大小为4k字节 */
#define W25Q128JV_PAGE_SIZE                   (256)     	 /*  每个页的大小为256字节  */

#define W25Q128JV_DUMMY_CYCLES_READ           4
#define W25Q128JV_DUMMY_CYCLES_READ_QUAD      10

//擦除超时时间
#define W25Q128JV_BULK_ERASE_MAX_TIME         250000	/*  擦除块   */
#define W25Q128JV_SECTOR_ERASE_MAX_TIME       3000		/*  擦除扇区 */
#define W25Q128JV_SUBSECTOR_ERASE_MAX_TIME    800 		/*  擦除页   */
/** 
  * @brief  W25Q256JV 指令
  */  
/* 复位操作 */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99


/* 识别操作 */
#define READ_ID_CMD                          0x90
#define READ_JEDEC_ID_CMD                    0x9F
//#define DUAL_READ_ID_CMD                     0x92
//#define QUAD_READ_ID_CMD                     0x94


/* 读操作 */
#define READ_CMD                       0x03
#define FAST_READ_CMD				   0x0B

/* 写操作 */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04


/* 状态寄存器 */
#define READ_STATUS_REG1_CMD                  0x05
#define READ_STATUS_REG2_CMD                  0x35
#define READ_STATUS_REG3_CMD                  0x15

#define WRITE_STATUS_REG1_CMD                 0x01
#define WRITE_STATUS_REG2_CMD                 0x31
#define WRITE_STATUS_REG3_CMD                 0x11

/* 擦除操作 */
#define SECTOR_ERASE_CMD		              0x20
#define CHIP_ERASE_CMD                        0xC7

/* 状态寄存器标志 */
#define W25Q256JV_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q256JV_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q256JV_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */


/* 编程操作 */
#define PAGE_PROG							  0x02
#define QUAD_INPUT_PAGE_PROG				  0x32

/* QPI模式 */
//#define ENTER_QPI_MODE_CMD                   0x38
//#define EXIT_QPI_MODE_CMD                    0xFF

//#define PROG_ERASE_RESUME_CMD                 0x7A
//#define PROG_ERASE_SUSPEND_CMD                0x75


/*命令定义-结尾*******************************/

uint8_t BSP_QSPI_FlASH_Init(void);			//初始化flash
uint8_t BSP_QSPI_Erase_Sector(uint32_t BlockAddress);
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);

uint8_t BSP_QSPI_FastRead(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);

uint8_t BSP_QSPI_Erase_Chip(void);
//static uint8_t QSPI_ResetMemory(void);
//static uint8_t QSPI_WriteEnable(void);
//static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout);


uint32_t QSPI_FLASH_ReadDeviceID(void);
uint32_t QSPI_FLASH_ReadID(void);
uint32_t QSPI_FLASH_ReadStatusReg(uint8_t reg);
uint32_t QSPI_FLASH_WriteStatusReg(uint8_t reg,uint8_t regvalue);
void QSPI_Set_WP_High(void);
void QSPI_Set_WP_TO_QSPI_IO(void);
void QSPI_FLASH_Wait_Busy(void);
#endif /* __SPI_FLASH_H */

