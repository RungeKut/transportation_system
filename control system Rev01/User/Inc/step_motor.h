#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#include "main.h"

/* BEGIN Prototypes */
void step_StartUp(void);
void ON_STEP_TIMER(TIM_TypeDef*, TIM_TypeDef*);
void OFF_STEP_TIMER(TIM_TypeDef*, TIM_TypeDef*);
void MOTOR_STOP(TIM_TypeDef*, TIM_TypeDef*, TIM_TypeDef*);
void MOTOR_START_UP(TIM_TypeDef*, TIM_TypeDef*);
void MOTOR_START_DOWN(TIM_TypeDef*, TIM_TypeDef*);
void Motor_Initialization(TIM_TypeDef*, TIM_TypeDef*, TIM_TypeDef*);
void Count_Num_Step (void);
void Test_Screw(void);
void Motor_Setup(void);
void EMERGENCY_STOP(TIM_TypeDef*, TIM_TypeDef*);
void INITIALIZATION_END(void);
void StepGenerate_IRQHandler_Callback(TIM_TypeDef*);
void Acceleration_IRQHandler_Callback(TIM_TypeDef*, TIM_TypeDef*);
void step_TIM2_Init(void);
/* END Prototypes */

#endif /* STEP_MOTOR_H */
