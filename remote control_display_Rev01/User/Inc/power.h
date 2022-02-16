#ifndef POWER_H
#define POWER_H

#include "main.h"

extern volatile uint8_t NumFlagSleepDeny;

void irq_enable(void);
void irq_disable(void);
void DebugOnStandByMode(void);
void GoToStandbyMode(void);
void ResetTIM_Sleep(void);
void SleepAfterWatchDog(void);
void StandbyMode_IRQHandler(void);

#endif /* POWER_H */
