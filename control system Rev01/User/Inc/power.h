#ifndef POWER_H
#define POWER_H

#include "main.h"

/* BEGIN Prototypes */
void GoToStandbyMode(void);
void Sleep_IRQHandler_Callback(TIM_TypeDef*);
void Sleep_Reset(void);
/* END Prototypes */

#endif /* POWER_H */
