 /**
  ******************************************************************************
  * @file    bsp_qspi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   qspi flash 底层应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32MP157 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./flash/bsp_qspi_flash.h"

//QSPI_HandleTypeDef QSPIHandle;


/**
  * @brief  读取存储器的SR并等待EOP
  * @param  QSPIHandle: QSPI句柄
  * @param  Timeout 超时
  * @retval 无
  */
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	/* 配置自动轮询模式等待存储器准备就绪 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	s_config.Match           = 0x00;
	s_config.Mask            = W25Q256JV_FSR_BUSY;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&QSPIHandle, &s_command, &s_config, Timeout) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  复位QSPI存储器。
  * @param  QSPIHandle: QSPI句柄
  * @retval 无
  */
static uint8_t QSPI_ResetMemory(void)
{
	QSPI_CommandTypeDef s_command;
	/* 初始化复位使能命令 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = RESET_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* 发送命令 */
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* 发送复位存储器命令 */
	s_command.Instruction = RESET_MEMORY_CMD;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* 配置自动轮询模式等待存储器就绪 */
	if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}


/**
  * @brief  发送写入使能，等待它有效.
  * @param  QSPIHandle: QSPI句柄
  * @retval 无
  */
static uint8_t QSPI_WriteEnable(void)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	/* 启用写操作 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* 配置自动轮询模式等待写启用 */
	s_config.Match           = W25Q256JV_FSR_WREN;
	s_config.Mask            = W25Q256JV_FSR_WREN;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	s_command.Instruction    = READ_STATUS_REG1_CMD;
	s_command.DataMode       = QSPI_DATA_1_LINE;
	s_command.NbData         = 1;

	if (HAL_QSPI_AutoPolling(&QSPIHandle, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}



/**
  * @brief  初始化QSPI存储器
  * @retval QSPI存储器状态
  */
uint8_t BSP_QSPI_FlASH_Init(void)
{ 
	
	/* QSPI存储器复位 */
	if (QSPI_ResetMemory() != QSPI_OK)
	{
		return QSPI_NOT_SUPPORTED;
	}

	return QSPI_OK;
}

/**
  * @brief  从QSPI存储器中读取大量数据.
  * @param  pData: 指向要读取的数据的指针
  * @param  ReadAddr: 读取起始地址
  * @param  Size: 要读取的数据大小
  * @retval QSPI存储器状态
  */
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
  
  if(Size == 0)
  {
    printf("BSP_QSPI_Read Size = 0");
    return QSPI_OK;
  }

	/* 初始化读命令 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = ReadAddr;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = Size;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* 配置命令 */
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{      
		return QSPI_ERROR;
	}

	/* 接收数据 */
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}
/**
  * @brief  从QSPI存储器中读取大量数据.
  * @param  pData: 指向要读取的数据的指针
  * @param  ReadAddr: 读取起始地址
  * @param  Size: 要读取的数据大小
  * @retval QSPI存储器状态
  */
//uint8_t BSP_QSPI_FastRead(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
//{
//	QSPI_CommandTypeDef s_command;
//
//  if(Size == 0)
//  {
//    printf("BSP_QSPI_FastRead Size = 0");
//    return QSPI_OK;
//  }
//	/* 初始化读命令 */
//	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
//	s_command.Instruction       = FAST_READ_CMD;
//	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
//	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
//	s_command.Address           = ReadAddr;
//	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//	s_command.DataMode          = QSPI_DATA_1_LINE;
//	s_command.DummyCycles       = 6;
//	s_command.NbData            = Size;
//	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
//	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
//	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
//
//	/* 配置命令 */
//	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//	{
//		return QSPI_ERROR;
//	}
//
//	/* 接收数据 */
//	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//	{
//		return QSPI_ERROR;
//	}
//	return QSPI_OK;
//}

/**
  * @brief  将大量数据写入QSPI存储器
  * @param  pData: 指向要写入数据的指针
  * @param  WriteAddr: 写起始地址
  * @param  Size: 要写入的数据大小
  * @retval QSPI存储器状态
  */
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
	uint32_t end_addr, current_size, current_addr;
  
	if(Size == 0)
	{
		printf("BSP_QSPI_Write Size = 0");
		return QSPI_OK;
	}

	/* 计算写入地址和页面末尾之间的大小 */
	current_addr = 0;

	while (current_addr <= WriteAddr)
	{
		current_addr += W25Q128JV_PAGE_SIZE;
	}
	current_size = current_addr - WriteAddr;

	/* 检查数据的大小是否小于页面中的剩余位置 */
	if (current_size > Size)
	{
		current_size = Size;
	}

	/* 初始化地址变量 */
	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;

	/* 初始化程序命令 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = PAGE_PROG;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* 逐页执行写入 */
	do
	{
		s_command.Address = current_addr;
    if(current_size == 0)   
    {
      printf("BSP_QSPI_Write current_size = 0");
      return QSPI_OK;
    }

		s_command.NbData  = current_size;

		/* 启用写操作 */
		if (QSPI_WriteEnable() != QSPI_OK)
		{
			return QSPI_ERROR;
		}

		/* 配置命令 */
		if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		/* 传输数据 */
		if (HAL_QSPI_Transmit(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		/* 配置自动轮询模式等待程序结束 */
		if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
		{
			return QSPI_ERROR;
		}

		/* 更新下一页编程的地址和大小变量 */
		current_addr += current_size;
		pData += current_size;
		current_size = ((current_addr + W25Q128JV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128JV_PAGE_SIZE;
	} while (current_addr < end_addr);
	return QSPI_OK;
}


/**
* @brief  擦除QSPI存储器的指定扇区
* @param  SectorAddress: 需要擦除的扇区地址
* @retval QSPI存储器状态
*/
uint8_t BSP_QSPI_Erase_Sector(uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;
    /* 初始化擦除命令 */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = SECTOR_ERASE_CMD;
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
    s_command.Address           = SectorAddress;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_NONE;
    s_command.DummyCycles       = 0;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* 启用写操作 */
    if (QSPI_WriteEnable() != QSPI_OK) {
        return QSPI_ERROR;
    }

    /* 发送命令 */
    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    /* 配置自动轮询模式等待擦除结束 */
    if (QSPI_AutoPollingMemReady(W25Q128JV_SECTOR_ERASE_MAX_TIME) != QSPI_OK) {
        return QSPI_ERROR;
    }
    return QSPI_OK;
}




/**
  * @brief  擦除整个QSPI存储器
  * @retval QSPI存储器状态
  */
uint8_t BSP_QSPI_Erase_Chip(void)
{
	QSPI_CommandTypeDef s_command;
	/* 初始化擦除命令 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = CHIP_ERASE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* 启用写操作 */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	//debug
	printf("\r\nFlash Status Reg1 is 0x%02X,\r\n", QSPI_FLASH_ReadStatusReg(1));

	/* 发送命令 */
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	} 
	/* 配置自动轮询模式等待擦除结束 */
	if (QSPI_AutoPollingMemReady(W25Q128JV_BULK_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  读取QSPI存储器的当前状态
  * @retval QSPI存储器状态
  */
uint8_t BSP_QSPI_GetStatus(void)
{
	QSPI_CommandTypeDef s_command;
	uint8_t reg;
	/* 初始化读取状态寄存器命令 */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* 配置命令 */
	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* 接收数据 */
	if (HAL_QSPI_Receive(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	} 
	/* 检查寄存器的值 */
	if((reg & W25Q256JV_FSR_BUSY) != 0)
	{
		return QSPI_BUSY;
	}
	else
	{
		return QSPI_OK;
	}
}

/**
  * @brief  返回QSPI存储器的配置
  * @param  pInfo: 在配置结构上的指针
  * @retval QSPI存储器状态

uint8_t BSP_QSPI_GetInfo(QSPI_Info* pInfo)
{
	// 配置存储器配置结构
	pInfo->FlashSize          = W25Q128JV_FLASH_SIZE;
	pInfo->EraseSectorSize    = W25Q128JV_SUBSECTOR_SIZE;
	pInfo->EraseSectorsNumber = (W25Q128JV_FLASH_SIZE/W25Q128JV_SUBSECTOR_SIZE);
	pInfo->ProgPageSize       = W25Q128JV_PAGE_SIZE;
	pInfo->ProgPagesNumber    = (W25Q128JV_FLASH_SIZE/W25Q128JV_PAGE_SIZE);
	return QSPI_OK;
}
*/


 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t QSPI_FLASH_ReadID(void)
{
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[3];
	/* 读取JEDEC ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_JEDEC_ID_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	  s_command.AddressMode     = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 3;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}

	Temp = ( pData[2] | pData[1]<<8 )| ( pData[0]<<16 );

	return Temp;
}

 /**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
uint32_t QSPI_FLASH_ReadDeviceID(void)
{  
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[3];
	/*##-2-读取设备ID测试    ###########################################*/
	/* 读取制造/设备 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_ID_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 2;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}

	Temp = pData[1] |( pData[0]<<8 ) ;

	return Temp;
}

 /**
  * @brief  读取ReadStatusReg
  * @param 	无
  * @retval ReadStatusReg
  */
uint32_t QSPI_FLASH_ReadStatusReg(uint8_t reg)
{  
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[10];

	/* 读取制造/设备 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	if(reg == 1)
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	else if(reg == 2)
	s_command.Instruction       = READ_STATUS_REG2_CMD;
	else if(reg == 3)
	s_command.Instruction       = READ_STATUS_REG3_CMD;
	
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}

	//单flash时读取的字节数为pData[0]，pData[1]
	//Temp = pData[0] |( pData[1]<<8 ) ;
	
	//双flash时读取的字节数为pData[0]，pData[2]
	//Temp = pData[1] |( pData[0]<<8 ) ;
		Temp = pData[0] ;
	return Temp;
}

 /**
  * @brief  读取ReadStatusReg
  * @param 	无
  * @retval ReadStatusReg
  */
uint32_t QSPI_FLASH_WriteStatusReg(uint8_t reg,uint8_t regvalue)
{  
	QSPI_CommandTypeDef s_command;
	
	/* 使能写操作 */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	/* 读取制造/设备 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	if(reg == 1)
	s_command.Instruction       = WRITE_STATUS_REG1_CMD;
	else if(reg == 2)
	s_command.Instruction       = WRITE_STATUS_REG2_CMD;
	else if(reg == 3)
	s_command.Instruction       = WRITE_STATUS_REG3_CMD;
	else
	s_command.Instruction       = reg;
	
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AddressSize       = QSPI_ADDRESS_8_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Transmit(&QSPIHandle, &regvalue, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* 用户可以在这里添加一些代码来处理这个错误 */
		while(1)
		{
			
		}
	}
	/* 自动轮询模式等待存储器就绪 */
	if (QSPI_AutoPollingMemReady(W25Q128JV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


//等待空闲
void QSPI_FLASH_Wait_Busy(void)   
{   
	while((QSPI_FLASH_ReadStatusReg(1)&0x01)==0x01);   // 等待BUSY位清空
}   

/*********************************************END OF FILE**********************/
