//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file proto.h*/
/**\brief Реализация чтения modbus адреса через SPI.*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "spimbaddr.h"
#include "swtimer.h"
#include "atomic.h"
#include "proto.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	bool AddressIsValid;
	uint8_t Address;
} ModBus_AddressTypeDef;
/* Private define ------------------------------------------------------------*/
// spi
#define MBADDR_SPI		(SPI3)
#define MBADDR_SPI_HNDL	(&hspi3)
#define SPI_TIMEOUT_MS	(5)
#define MBADDR_SPI_GPIO_Port	(GPIOC)
#define MBADDR_SPI_CLK_Pin		(GPIO_PIN_10)
// spi

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//extern Driver_TelemetryTypeDef DriverTelemetry;
//extern SPI_HandleTypeDef hspi3;
static ModBus_AddressTypeDef MBAddress;

/* Private function prototypes -----------------------------------------------*/
static uint8_t ReadMBAddress(void);
static void SaveMBAddress(uint8_t addr);
//static void SwTimer_MBAddressCB(void);
uint8_t adr_input[4];
uint8_t adr_sym = 0;
/* Private functions ---------------------------------------------------------*/

__STATIC_INLINE uint8_t ReadMBAddress(void)
{
	uint8_t i,data;
	// spi
//	SPI3_LOAD_GPIO_Port->BSRR = (uint32_t)SPI3_LOAD_Pin << 16;
//	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
//	SPI3_LOAD_GPIO_Port->BSRR = SPI3_LOAD_Pin;
//	__HAL_SPI_ENABLE(MBADDR_SPI_HNDL);
//	do {} while (MBADDR_SPI_GPIO_Port->IDR & MBADDR_SPI_CLK_Pin);
//	do {} while (!(MBADDR_SPI_GPIO_Port->IDR & MBADDR_SPI_CLK_Pin));
//	do {} while (MBADDR_SPI_GPIO_Port->IDR & MBADDR_SPI_CLK_Pin);
//	__HAL_SPI_DISABLE(MBADDR_SPI_HNDL);
//	do {} while (! __HAL_SPI_GET_FLAG(MBADDR_SPI_HNDL, SPI_FLAG_RXNE));
   // end_spi

	// read_pin_adr
	adr_input[0]  =  HAL_GPIO_ReadPin(ADR1_GPIO_Port,  ADR1_Pin);
	adr_input[1]  =  HAL_GPIO_ReadPin(ADR2_GPIO_Port,  ADR2_Pin);
	adr_input[2]  =  HAL_GPIO_ReadPin(ADR3_GPIO_Port,  ADR3_Pin);
	adr_input[3]  =  HAL_GPIO_ReadPin(ADR4_GPIO_Port,  ADR4_Pin);

	 for (i=0; i<sizeof(adr_input); i++)
	  {
	    adr_sym += adr_input[i];
	  }
	 if (adr_sym > 1 || adr_sym == 0) {MBAddress.AddressIsValid = false; data = 0;}
	 else {
	       if      (adr_input[0] == 1) {data = 1;}
	       else if (adr_input[1] == 1) {data = 2;}
           else if (adr_input[2] == 1) {data = 3;}
           else                        {data = 4;}
	 }
	 adr_sym = 0;


//	uint8_t data = *(__IO uint8_t *)&MBADDR_SPI_HNDL->Instance->DR; //
//	data >>= 4; // сдвиг вправо на 4
	return data;
}

/*----------------------------------------------------------------------------*/
static void SaveMBAddress(uint8_t addr)
{
	if (addr != MBAddress.Address)
	{
		//if (MB_ENOERR == eMBSetAddress(addr))
		{
			MBAddress.Address = addr;
			MBAddress.AddressIsValid = true;
			/*
			ATOMIC_BLOCK_RESTORE
			{
				DriverTelemetry.HallCounter = MBAddress.Address;
			}
			*/
		}
	}
	else
	{
		MBAddress.AddressIsValid = true;
	}
}
/*----------------------------------------------------------------------------*/
/*
static void SwTimer_MBAddressCB()
{
	SaveMBAddress(ReadMBAddress());
}
*/
/* Exported functions --------------------------------------------------------*/
void MBAddress_Init(void)
{
	//SwTimer_Add(SWTIMER_MODE_CYCLE, MB_ADDRESS_TIMER_TIMEOUT, SwTimer_MBAddressCB);
	SaveMBAddress(ReadMBAddress());
}
/*----------------------------------------------------------------------------*/
bool GetMBAddress(uint8_t *pAddress)
{
	bool temp_flag = false;
	ATOMIC_BLOCK_RESTORE
	{
		temp_flag = MBAddress.AddressIsValid;
		//MBAddress.AddressIsValid = false;
		*pAddress = MBAddress.Address;
	}
	return temp_flag;
}
