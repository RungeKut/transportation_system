//**************************************************************
//РћСЂРіР°РЅРёР·Р°С†РёСЏ:            РєР°С„РµРґСЂР° Р�РўР¤Р� РќРќР“РЈ, Рі. РќРёР¶РЅРёР№ РќРѕРІРіРѕСЂРѕРґ
//РџСЂРѕРµРєС‚:                 pulse
//Р’РµСЂСЃРёСЏ РїСЂРѕРµРєС‚Р°: 1.0
//РЎРїРµС†РёС„РёРєР°С†РёСЏ:   ANSI C++ 98
//РЎРѕР·РґР°РЅ:                 25.11.2017
//Р�Р·РјРµРЅРµРЅ:                25.11.2017
//**************************************************************/

/**\author РќРѕРІРёРєРѕРІ Р’. Рђ.*/
/**\file proto.h*/
/**\brief Р РµР°Р»РёР·Р°С†РёСЏ С‡С‚РµРЅРёСЏ-Р·Р°РїРёСЃРё РІ EPROM.*/

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "eprom.h"
#include "bitbanding.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEVICE_ADDRESS		(0xA0)
#define MEM_BASE_ADDRESS	(0x00)
#define PAGE_SIZE			(8)
#define I2C_TIMEOUT			(100U)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
EPROM_Driver_ConfigTypeDef EPROM_Data;
static uint8_t eprom_ready;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef EPROM_Init(void)
{
	HAL_StatusTypeDef status;

	status = EPROM_Read (offsetof(EPROM_Driver_ConfigTypeDef, GroupCmdTimeOut), (uint8_t *)(&(EPROM_Data.GroupCmdTimeOut)), sizeof(EPROM_Data.GroupCmdTimeOut));
	if (status != HAL_OK)
	{
		return status;
	}
	status = EPROM_Read (offsetof(EPROM_Driver_ConfigTypeDef, Acceleration), (uint8_t *)(&(EPROM_Data.Acceleration)), sizeof(EPROM_Data.Acceleration));
	if (status != HAL_OK)
	{
		return status;
	}
	status = EPROM_Read (offsetof(EPROM_Driver_ConfigTypeDef, PoleNum), (uint8_t *)(&(EPROM_Data.PoleNum)), sizeof(EPROM_Data.PoleNum));
	if (status != HAL_OK)
	{
		return status;
	}
	status = EPROM_Read (offsetof(EPROM_Driver_ConfigTypeDef, FeedBackMode), (uint8_t *)(&(EPROM_Data.FeedBackMode)), sizeof(EPROM_Data.FeedBackMode));
	if (status != HAL_OK)
	{
		return status;
	}
	status = EPROM_Read (offsetof(EPROM_Driver_ConfigTypeDef, CurrentLimit), (uint8_t *)(&(EPROM_Data.CurrentLimit)), sizeof(EPROM_Data.CurrentLimit));
	if (status != HAL_OK)
	{
		return status;
	}
	status = EPROM_Read (offsetof(EPROM_Driver_ConfigTypeDef, TempearatureLimit), (uint8_t *)(&(EPROM_Data.TempearatureLimit)), sizeof(EPROM_Data.TempearatureLimit));
	if (status != HAL_OK)
	{
		return status;
	}
	BIT_BAND_SRAM(eprom_ready,0) = 1;
	return status;
}

HAL_StatusTypeDef EPROM_Write(uint16_t MemAddress, uint8_t *pdata, uint16_t size)
{
	uint8_t page_indx = 0;
	uint8_t size_to_write;
	HAL_StatusTypeDef result;

	do
	{
		if (size >= PAGE_SIZE)
		{
			size_to_write = PAGE_SIZE;
		}
		else
		{
			size_to_write = size;
		}
		if (MemAddress + size_to_write <= PAGE_SIZE)
		{
			result = HAL_I2C_Mem_Write(&hi2c1, DEVICE_ADDRESS,
					MemAddress + page_indx * PAGE_SIZE, I2C_MEMADD_SIZE_8BIT,
					pdata + page_indx * PAGE_SIZE, size_to_write, I2C_TIMEOUT);
			if (result != HAL_OK)
			{
				break;
			}
		}
		else
		{
			uint8_t remainder = MemAddress + size_to_write - PAGE_SIZE;

			result = HAL_I2C_Mem_Write(&hi2c1, DEVICE_ADDRESS,
					MemAddress + page_indx * PAGE_SIZE, I2C_MEMADD_SIZE_8BIT,
					pdata + page_indx * PAGE_SIZE, size_to_write - remainder, I2C_TIMEOUT);
			if (result != HAL_OK)
			{
				break;
			}
			result = HAL_I2C_Mem_Write(&hi2c1, DEVICE_ADDRESS,
					MemAddress + page_indx * PAGE_SIZE + size_to_write - remainder, I2C_MEMADD_SIZE_8BIT,
					pdata + page_indx * PAGE_SIZE + size_to_write - remainder, remainder, I2C_TIMEOUT);
			if (result != HAL_OK)
			{
				break;
			}
		}
		page_indx++;
		size -= size_to_write;
	}
	while (size > 0);
	return result;
}

bool EPROM_IsInitialized(void)
{
	return BIT_BAND_SRAM(eprom_ready,0) != 0;
}

HAL_StatusTypeDef EPROM_Read(uint16_t MemAddress, uint8_t *pdata, uint16_t size)
{
	HAL_StatusTypeDef result;

	result = HAL_I2C_Mem_Read(&hi2c1, DEVICE_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, pdata, size, I2C_TIMEOUT);

	return result;

}
