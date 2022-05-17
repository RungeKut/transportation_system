/*
 * Flash.cpp
 *
 *  Created on: 24 мар. 2021 г.
 *      Author: arkhipov.a
 */

#include "Flash.h"
#include "stm32l0xx_hal_flash.h"
#include "stm32l0xx_hal_flash_ex.h"





void Flash::flashErase (uint32_t pageAddress , int npages)
{

			FLASH_EraseInitTypeDef EraseInit;
			EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
			EraseInit.NbPages = npages;
			EraseInit.PageAddress = pageAddress;
			uint32_t page_error = 0;
			HAL_FLASHEx_Erase( &EraseInit, &page_error);

}

void Flash::flashWrite (uint32_t Address , uint32_t Data)
{

			HAL_FLASH_Unlock( );	//Unlock flash;
			flashErase(Address,1);
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data);
			HAL_FLASH_Lock( );	// Lock flash;*/

}

void Flash::flashRead (unsigned int &DatatoRead , uint32_t Address)
{
			volatile unsigned int temp;
			temp = ( *(__IO uint32_t*) Address);
			if (temp != 0xFFFF) DatatoRead = temp;

}

void Flash::flashRead (Turn_t &DatatoRead , uint32_t Address)
{
			volatile unsigned int temp;
			temp = ( *(__IO uint32_t*) Address);
			if (temp != 0xFFFF) DatatoRead =(Turn_t) temp;

}

void Flash::flashRead (bool &DatatoRead , uint32_t Address)
{
			volatile unsigned int temp;
			temp = ( *(__IO uint32_t*) Address);
			if (temp != 0xFFFF) DatatoRead =(bool) temp;

}
