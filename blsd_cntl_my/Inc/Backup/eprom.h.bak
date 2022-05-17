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
/**\brief Реализация протокола обмена через modbus.*/


#ifndef EPROM_H_
#define EPROM_H_
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
 typedef struct
 {
 	uint16_t GroupCmdTimeOut;
 	uint16_t Acceleration;
 	uint8_t PoleNum;
 	uint8_t FeedBackMode;
 	uint16_t CurrentLimit;
 	uint16_t TempearatureLimit;
 } EPROM_Driver_ConfigTypeDef;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
 HAL_StatusTypeDef EPROM_Init(void);
 HAL_StatusTypeDef EPROM_Write(uint16_t MemAddress, uint8_t *pdata, uint16_t size);
 HAL_StatusTypeDef EPROM_Read(uint16_t MemAddress, uint8_t *pdata, uint16_t size);
 bool EPROM_IsInitialized(void);

#ifdef __cplusplus
}
#endif


#endif /* EPROM_H_ */
