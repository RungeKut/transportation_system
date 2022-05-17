/*
 * button.h
 *
 *  Created on: 18 мая 2020 г.
 *      Author: arkhipov.a
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    BUTTON_RELEASE,
    BUTTON_PRESS
} ButtonState_TypeDef;

typedef enum
{
    NONE_BUTTON,
    UP_BUTTON,
    DOWN_BUTTON,
    LEFT_BUTTON,
    RIGHT_BUTTON
} ButtonID_TypeDef;

/* Private function -----------------------------------------------------------*/
void PressButtonCB(uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* INC_BUTTON_H_ */
