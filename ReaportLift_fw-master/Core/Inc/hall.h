//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file hall.h*/
/**\brief Реализация сигналов от датчиков Холла и кнопки.*/


#ifndef HALL_H_
#define HALL_H_
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void InitHall(void);
uint8_t GetHallState(void);
void HallSensorCB(uint16_t GPIO_Pin);
uint8_t GetPrevHallSector(uint8_t sector);
uint8_t GetNextHallSector(uint8_t sector);
bool IsHallSensorStateChanged(void);
void SetHallStateChangeFlag(uint8_t state);
#ifdef __cplusplus
}
#endif


#endif /* HALL_H_ */
