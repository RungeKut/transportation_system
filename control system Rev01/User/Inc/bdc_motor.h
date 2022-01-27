#ifndef BDC_MOTOR_H
#define BDC_MOTOR_H

#include "main.h"

extern volatile uint32_t quantityCounter;
extern uint16_t Encoder_Period;
extern uint16_t Encoder_DutyCicle;
extern uint32_t Encoder_Freqency;
extern uint16_t DutyToBDC;
extern uint32_t Encoder_Speed;

/* BEGIN Prototypes */
void bdc_StartUp(void);
void bdc_speed_up(void);
void bdc_speed_down(void);
void SetDutyCycleBDC(TIM_TypeDef*, uint16_t);
void Encoder_IRQHandler_Callback(TIM_TypeDef*);
void bdc_ON(void);
void bdc_OFF(void);
void bdc_TIM2_Init(void);
/* END Prototypes */

#endif /* BDC_MOTOR_H */
