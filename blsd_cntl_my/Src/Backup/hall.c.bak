//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file hall.c*/
/**\brief Реализация сигналов от датчиков Холла и кнопки.*/

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "bitbanding.h"
#include "hall.h"
#include "button.h"
#include "atomic.h"
#include "6Step_Lib_Hall.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define POLL_CNT	(7)
#define HALL_STATE_CHANGED_Pos	(0)
#define HALL_STATE_CHANGED_Msk	(1<<HALL_STATE_CHANGED_Pos)
/* Private macro -------------------------------------------------------------*/
#define POLL_INPUT(GPIO_REG, GPIO_PIN)										\
	do {																	\
		uint8_t hall_tmp = 0;												\
		uint8_t poll_cnt = POLL_CNT;										\
																			\
		hall_tmp = BIT_BAND_PER(GPIO_REG->IDR, GPIO_PIN);					\
		do 																	\
		{																	\
			if (BIT_BAND_PER(GPIO_REG->IDR, GPIO_PIN) == hall_tmp)			\
			{																\
				--poll_cnt;													\
			}																\
			else															\
			{																\
				hall_tmp = BIT_BAND_PER(GPIO_REG->IDR, GPIO_PIN);			\
				poll_cnt = POLL_CNT;										\
			}																\
		} while (poll_cnt > 0);												\
	} while(0)


/**Таблица кодов датчиков Холла*/
/*          NAME     CODE    */
#define HALL_SENSOR_STATE_TABLE(ENTRY)\
    ENTRY(HALL_RESERVED_0, 0x00)\
    ENTRY(HALL_HAS1, 0x03)\
    ENTRY(HALL_HAS2, 0x01)\
    ENTRY(HALL_HAS3, 0x05)\
    ENTRY(HALL_HAS4, 0x04)\
    ENTRY(HALL_HAS5, 0x06)\
    ENTRY(HALL_HAS6, 0x02)\
    ENTRY(HALL_RESERVED_7, 0x07)

#define EXPAND_AS_HALL_SENSOR_ENUM_POSITION(a,b) a##_POSITION,
enum {
  HALL_SENSOR_STATE_TABLE(EXPAND_AS_HALL_SENSOR_ENUM_POSITION)
};

#define EXPAND_AS_HALL_SENSOR_ENUM(a,b) a##_CODE=b,
enum {
  HALL_SENSOR_STATE_TABLE(EXPAND_AS_HALL_SENSOR_ENUM)
};

#define EXPAND_AS_HALL_SENSOR_STRUCT(a,b) uint8_t hssz_##a;
typedef struct {
  HALL_SENSOR_STATE_TABLE(EXPAND_AS_HALL_SENSOR_STRUCT)
} HallSensorSize_t;

#define NUM_HALL_SENSOR_STATES (sizeof(HallSensorSize_t))

#define EXPAND_AS_HALL_SENSOR_VECTOR(a,b) b,
static const uint8_t hall_sensor_state_vector[NUM_HALL_SENSOR_STATES] = {
  HALL_SENSOR_STATE_TABLE(EXPAND_AS_HALL_SENSOR_VECTOR)
};

#define EXPAND_AS_HALL_SENSOR_OFFSET_VECTOR(a,b) [b] = (uint8_t)offsetof(HallSensorSize_t, hssz_##a),
static const uint8_t hall_sensor_offset_vector[NUM_HALL_SENSOR_STATES] = {
  HALL_SENSOR_STATE_TABLE(EXPAND_AS_HALL_SENSOR_OFFSET_VECTOR)
};

/* Private variables ---------------------------------------------------------*/
__IO uint8_t HallState;
__IO uint8_t HallStateChanged;
/* Private function prototypes -----------------------------------------------*/
void ChangeHallSensorState(void);

/* Private functions ---------------------------------------------------------*/


/*
inline void poll_input(GPIO_TypeDef* gpio_reg, uint16_t gpio_pin)
{
	uint8_t hall_tmp = 0;
	uint8_t poll_cnt = POLL_CNT;

	hall_tmp = BIT_BAND_PER(gpio_reg->IDR, gpio_pin);
	do
	{
		if (BIT_BAND_PER(gpio_reg->IDR, gpio_pin) == hall_tmp)
		{
			--poll_cnt;
		}
		else
		{
			hall_tmp = BIT_BAND_PER(gpio_reg->IDR, gpio_pin);
			poll_cnt = POLL_CNT;
		}
	} while (poll_cnt > 0);
}
*/

inline void ChangeHallSensorState(void)
{
  BIT_BAND_SRAM(HallStateChanged, HALL_STATE_CHANGED_Pos) = 1;
  MC_FindRealDirection();
}

/* Exported functions --------------------------------------------------------*/

void HallSensorCB(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Hall_1_Pin)
	{
		//POLL_INPUT(Hall_1_GPIO_Port, Hall_1_Pin);
		BIT_BAND_SRAM(HallState, 0) = BIT_BAND_PER(Hall_1_GPIO_Port->IDR, Hall_1_Pin);
	}
	else if (GPIO_Pin == Hall_2_Pin)
	{
		//POLL_INPUT(Hall_2_GPIO_Port, Hall_2_Pin);
		BIT_BAND_SRAM(HallState, 1) = BIT_BAND_PER(Hall_2_GPIO_Port->IDR, Hall_2_Pin);
	}
	else if (GPIO_Pin == Hall_3_Pin)
	{
		//POLL_INPUT(Hall_3_GPIO_Port, Hall_3_Pin);
		BIT_BAND_SRAM(HallState, 2) = BIT_BAND_PER(Hall_3_GPIO_Port->IDR, Hall_3_Pin);
	}
	MC_IncCommCount();
	ChangeHallSensorState();
	//MC_Commutate();
}

/*----------------------------------------------------------------------------*/
void InitHall(void)
{
	uint8_t temp = 0;

	//POLL_INPUT(Hall_1_GPIO_Port, Hall_1_Pin);
	//POLL_INPUT(Hall_2_GPIO_Port, Hall_2_Pin);
	//POLL_INPUT(Hall_3_GPIO_Port, Hall_3_Pin);
	temp =  BIT_BAND_PER(Hall_1_GPIO_Port->IDR, Hall_1_Pin) << 0 |
			BIT_BAND_PER(Hall_2_GPIO_Port->IDR, Hall_2_Pin) << 1 |
			BIT_BAND_PER(Hall_3_GPIO_Port->IDR, Hall_3_Pin) << 2;
	ATOMIC_BLOCK_RESTORE
	{
		HallState = temp;
	}
}

/*----------------------------------------------------------------------------*/
uint8_t GetHallState(void)
{
	uint8_t temp;
	ATOMIC_BLOCK_RESTORE
	{
		temp = HallState;
	}
	return temp;
}

/*----------------------------------------------------------------------------*/
uint8_t GetPrevHallSector(uint8_t sector)
{
  uint8_t sector_offset = 0;

  if ( (sector > HALL_RESERVED_0_CODE) && (sector < (HALL_RESERVED_7_CODE)) )
  {
    sector_offset = hall_sensor_offset_vector[sector];
    if ( --sector_offset < 1)
    {// if sector offset is zero then this is reserverved state and it should be assigned state before last reserved state
      sector_offset = hall_sensor_offset_vector[HALL_RESERVED_7_CODE] - 1;
    }
  }
  return hall_sensor_state_vector[sector_offset];
}

/*----------------------------------------------------------------------------*/
uint8_t GetNextHallSector(uint8_t sector)
{
  uint8_t sector_offset = 0;

  if ( (sector > HALL_RESERVED_0_CODE) && (sector < (HALL_RESERVED_7_CODE)) )
  {
    sector_offset = hall_sensor_offset_vector[sector];
    if ( ++sector_offset >= NUM_HALL_SENSOR_STATES-1 )
    {// if sector offset is equal last element hen this is reserverved state and it should be assigned state next after reserved state
      sector_offset = hall_sensor_offset_vector[HALL_RESERVED_0_CODE] + 1;
    }
  }
  return hall_sensor_state_vector[sector_offset];
}

/*----------------------------------------------------------------------------*/
void SetHallStateChangeFlag(uint8_t state)
{
  BIT_BAND_SRAM(HallStateChanged, HALL_STATE_CHANGED_Pos) = (state != 0) ? 1 : 0;
}
/*----------------------------------------------------------------------------*/
bool IsHallSensorStateChanged(void)
{
  bool result = false;
  result = BIT_BAND_SRAM(HallStateChanged, HALL_STATE_CHANGED_Pos) ? true : false;
  return result;
}
