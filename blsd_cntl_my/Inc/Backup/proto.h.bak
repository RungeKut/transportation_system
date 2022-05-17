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


#ifndef PROTO_H_
#define PROTO_H_
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t ResetCounter;
	uint16_t GroupCmdTimeOut;
	uint16_t Acceleration;
	uint16_t PoleNum;
	uint16_t FeedBack;
	uint16_t CurrentLimit;
	uint16_t TempearatureLimit;
	uint16_t Command;
} Driver_ConfigTypeDef;

typedef struct
{
	uint16_t GroupBaseAddress;
	uint16_t DriveNumber;
	uint16_t State[32];
} Driver_GroupStateTypeDef;

typedef struct
{
	uint16_t ADCCode[4];
	uint16_t RevCounter;
	uint16_t HallCounter;
	uint16_t ErrorStatus;
} Driver_TelemetryTypeDef;

/* Exported constants --------------------------------------------------------*/
#define DRIVER_CONFIG_MB_REG_ADDR		(1000)
#define DRIVER_STATE_MB_REG_ADDR		(1200)
#define DRIVER_TELEMETRY_MB_REG_ADDR	(1100)
/* Exported macro ------------------------------------------------------------*/
#define DRIVER_NREGS(TypeDef)			(sizeof(TypeDef)/sizeof(uint16_t))
/* Exported functions --------------------------------------------------------*/
void ProtoInit(void);
#ifdef __cplusplus
}
#endif

#endif /* PROTO_H_ */
