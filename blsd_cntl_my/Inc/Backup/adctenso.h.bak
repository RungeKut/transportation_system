//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file adctenso.h*/
/**\brief Реализация АЦП отцифровки внешних сигналов тензодатчиков и инклинометров.*/


#ifndef ADCTENSO_H_
#define ADCTENSO_H_
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define TENSO_CH		ADC_CHANNEL_5
#define TENSO_CHANNEL_NUM		(4)
#define TENSO_CHANNEL_Msk		(TENSO_CHANNEL_NUM - 1)

/* Exported functions --------------------------------------------------------*/
 void SwTimer_ADCTensoCB(void);
 void ADCTenso_ConvCpltCB(ADC_HandleTypeDef* hadc);
 void ADCTenso_Init(void);
 void ADCTenso_Task(void);
 uint32_t GetTensoADCBuff(uint16_t **buff_addr_ptr, uint32_t *buff_size_ptr);

#ifdef __cplusplus
}
#endif


#endif /* ADCTENSO_H_ */
