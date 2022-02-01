#include "step_motor.h"
#include "main.h"

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Стат																																			 ┃
void step_StartUp(void)//																										 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//	if (ACCELERATION_EN != 0)
//	{
//		LL_TIM_EnableIT_UPDATE(AC_TIM);
//	}
//	else
//	{
//		ST_TIM->ARR  =  MAX_FREQ;
//	}
  NVIC_EnableIRQ(TIM21_IRQn);
  NVIC_EnableIRQ(TIM22_IRQn);
  
  LL_TIM_EnableIT_UPDATE(AC_TIM);
  LL_TIM_EnableIT_UPDATE(ST_TIM);
  GLOBAL_FLAG_TX |= STOP_FLAG;
  MOTOR_STOP(AC_TIM, ST_TIM, TIM7);

  GLOBAL_FLAG_TX &= ~STOPING_FLAG;
  GLOBAL_FLAG_TX |= INITIALIZATION_FLAG;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Последовательность выключения таймеров задатчика step сигнала              ┃
//(таймер ускорения, таймер генерации step)                                  ┃
void ON_STEP_TIMER(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS)//                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (!(LL_TIM_IsEnabledCounter(TIMA)) || (ACCELERATION_EN != 0))
  {
//    NVIC_EnableIRQ(TIM21_IRQn);
    LL_TIM_ClearFlag_UPDATE(TIMA);
    LL_TIM_EnableCounter(TIMA);
//    LL_TIM_EnableIT_UPDATE(TIMA);
  }
  if (!(LL_TIM_IsEnabledCounter(TIMS)))
  {
//    NVIC_EnableIRQ(TIM22_IRQn);
    LL_TIM_ClearFlag_UPDATE(TIMS);
    LL_TIM_EnableCounter(TIMS);
//    LL_TIM_EnableIT_UPDATE(TIMS);
//    LL_TIM_SetAutoReload(TIMS, CALC_CYCLE(Freq_step));
//    LL_TIM_OC_SetCompareCH3(TIMS, CALC_CYCLE(Freq_step)/2);
//    LL_TIM_CC_EnableChannel(TIMS, LL_TIM_CHANNEL_CH3);
    keystrokeCounter++;
  }
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Последовательность выключения таймеров задатчика step сигнала              ┃
//(таймер ускорения, таймер генерации step)                                  ┃
void OFF_STEP_TIMER(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS)//                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (LL_TIM_IsEnabledCounter(TIMA))
  {
//    NVIC_DisableIRQ(TIM21_IRQn);
    LL_TIM_DisableCounter(TIMA);
//    LL_TIM_DisableIT_UPDATE(TIMA);
  }
  if (LL_TIM_IsEnabledCounter(TIMS))
  {
//    NVIC_DisableIRQ(TIM22_IRQn);
    LL_TIM_DisableCounter(TIMS);
//    LL_TIM_DisableIT_UPDATE(TIMS);
//    LL_TIM_CC_DisableChannel(TIMS, LL_TIM_CHANNEL_CH3);
    if (ACCELERATION_EN != 0)
    {
      TIMS->ARR = CALC_CYCLE(MIN_FREQ);
    }
  }
  if (keystrokeCounter >= MaxNumPush)
  {
    GLOBAL_FLAG_TX |= INITIALIZATION_FLAG;
  }
//  LL_TIM_DeInit(TIM2);
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// последовательность остановки шагового двигателя                           ┃
//(таймер ускорения, таймер генерации step, таймер сна мк)                   ┃
void MOTOR_STOP(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS, TIM_TypeDef *TIMSleep)
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (!( GLOBAL_FLAG_TX & STOP_FLAG ))
  {
    GLOBAL_FLAG_TX |= STOPING_FLAG;
    if ( Freq_step <= MIN_FREQ )
    {
      GLOBAL_FLAG_TX |= STOP_FLAG;
      GLOBAL_FLAG_TX &= ~MOVE_UP_FLAG;
      GLOBAL_FLAG_TX &= ~MOVE_DOWN_FLAG;
      GLOBAL_FLAG_TX &= ~ACCELERATION_FLAG;
      GLOBAL_FLAG_TX &= ~START_UP_FLAG;
      GLOBAL_FLAG_TX &= ~START_DOWN_FLAG;
      STEPDRIVER_DISABLE;
      OFF_STEP_TIMER(TIMA, TIMS);
      BOOST_DISABLE;
      DC_DC_SW_DISABLE;
      GLOBAL_FLAG_TX &= ~STOPING_FLAG;
    }
  }
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Последовательность запуска шагового двигателя вперед                       ┃
//(таймер ускорения, таймер генерации step)                                  ┃
void MOTOR_START_UP(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS)//                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  GLOBAL_FLAG_TX |= START_UP_FLAG;
  Sleep_Reset();

  if ((!(GLOBAL_FLAG_TX & STOPING_FLAG))          &&
      (!(GLOBAL_FLAG_TX & LIMIT_SWITCH_DOWN_FLAG))  &&
        (LL_TIM_IsEnabledCounter(TIMS) == 0))
      {  
        BOOST_ENABLE;
        DC_DC_SW_ENABLE;
        STEPDRIVER_ENABLE;
        GLOBAL_FLAG_TX &= ~STOP_FLAG;
        GLOBAL_FLAG_TX |= ACCELERATION_FLAG;
        STEPDRIVER_DIRECTION_UP;
        GLOBAL_FLAG_TX |= DIRECTION_FLAG;
        if (ACCELERATION_EN != 0)
        {
          Freq_step = MIN_FREQ;
          Max_freq_step = MAX_FREQ;
          TIMA->ARR  =  CALC_CYCLE(ACCELERATION);
        }
        ON_STEP_TIMER(TIMA, TIMS);
        GLOBAL_FLAG_TX |= MOVE_UP_FLAG;
      }
  else if (( Freq_step == MIN_FREQ ) &&
           ( GLOBAL_FLAG_TX & STOPING_FLAG))
  {
    STEPDRIVER_DIRECTION_UP;
    GLOBAL_FLAG_TX |= DIRECTION_FLAG;
    GLOBAL_FLAG_TX &= ~STOPING_FLAG;
  }
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Последовательность запуска шагового двигателя назад                        ┃
//(таймер ускорения, таймер генерации step)                                  ┃
void MOTOR_START_DOWN(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS)//                ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  GLOBAL_FLAG_TX |= START_DOWN_FLAG;
  Sleep_Reset();
  
  if ((!(GLOBAL_FLAG_TX & STOPING_FLAG))            &&
      (!(GLOBAL_FLAG_TX & LIMIT_SWITCH_UP_FLAG))    &&
        (LL_TIM_IsEnabledCounter(TIMS) == 0))
      {
        BOOST_ENABLE;
        DC_DC_SW_ENABLE;
        STEPDRIVER_ENABLE;
        GLOBAL_FLAG_TX &= ~STOP_FLAG;
        GLOBAL_FLAG_TX |= ACCELERATION_FLAG;
        STEPDRIVER_DIRECTION_DOWN;
        GLOBAL_FLAG_TX &= ~DIRECTION_FLAG;
        if (ACCELERATION_EN != 0)
        {
          Freq_step = MIN_FREQ;
          Max_freq_step = MAX_FREQ;
          TIMA->ARR  =  CALC_CYCLE(ACCELERATION);
        }
        ON_STEP_TIMER(TIMA, TIMS);
        GLOBAL_FLAG_TX |= MOVE_DOWN_FLAG;
      }
  else if (( Freq_step == MIN_FREQ ) &&
           ( GLOBAL_FLAG_TX & STOPING_FLAG))
  {
    STEPDRIVER_DIRECTION_DOWN;
    GLOBAL_FLAG_TX &= ~DIRECTION_FLAG;
    GLOBAL_FLAG_TX &= ~STOPING_FLAG;
  }
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Инициализация перед каждым включением. Мотор вращается с пониженной        ┃
//частотой в обратном направлении до концевика обратного направления.        ┃
//(таймер ускорения, таймер генерации step, таймер сна мк)                   ┃
void Motor_Initialization (TIM_TypeDef *TIMA, TIM_TypeDef *TIMS, TIM_TypeDef *TIMsleep)
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{ 
  if (GLOBAL_FLAG_TX & LIMIT_SWITCH_UP_FLAG)
  {
    MOTOR_STOP(TIMA, TIMS, TIMsleep);
        if (GLOBAL_FLAG_TX & INITIALIZATION_FLAG)
        {
          OFF_STEP_TIMER(TIMA, TIMS);
          GLOBAL_FLAG_TX &= ~INITIALIZATION_FLAG;
          GLOBAL_FLAG_TX &= ~ STOPING_FLAG;
          step = MAX_STEP;
          Freq_step = MIN_FREQ;
          keystrokeCounter = 0;
        }
  }
  else if (LL_TIM_IsEnabledCounter(TIMS) == 0) 
  {
    LL_TIM_DisableCounter(TIMsleep);
    MOTOR_START_DOWN(TIMA, TIMS);
    GLOBAL_FLAG_TX |= START_DOWN_FLAG;
    GLOBAL_FLAG_TX &= ~ STOPING_FLAG;
  }
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Полная инициализация с подсчетом количества шагов мотора. Мотор вращается с┃
//пониженной частотой в прямом направлении до концевика прямого направления, ┃
//затем едет обратно и считает шаги до концевика обратного направления.      ┃
void Count_Num_Step (void)//                                                 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
/*  if (CarriageStep == 1)
  {
    if (GLOBAL_FLAG_TX & LIMIT_SWITCH_DOWN_FLAG)
    {
      CarriageStep++;
      return;
    }
    else if (LL_TIM_IsEnabledCounter(TIM3) == 0)
    {
      Motor_Initialization();
    }
  }*/
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Долговременное тестирование с ПВ 2on/18off мин каждый мотор                ┃
void Test_Screw(void)//                                                      ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
/*  if (TestScrewStep == 1)
  {
    LL_TIM_SetCounter(TIM6, 0x0000);
    if (!(GLOBAL_FLAG_TX & DC_DC_SW_FLAG)) DC_DC_SW_ENABLE;
    
    if (!(GLOBAL_FLAG_TX & BUT_WEAK_FLAG))
    {
      MOTOR_STOP();
      GLOBAL_FLAG_TX &= ~0x1F;
      GLOBAL_FLAG_TX |= BUT_WEAK_FLAG;  //Уменьшить вес
    }
    else if (GLOBAL_FLAG_TX & LIMIT_SWITCH_UP_FLAG)
    {
      TestScrewStep++;
    }
  }
  else if (TestScrewStep == 2)
  {
    if (!(GLOBAL_FLAG_TX & BUT_STRONG_FLAG))
    {
      MOTOR_STOP();
      GLOBAL_FLAG_TX &= ~0x2F;
      GLOBAL_FLAG_TX |= BUT_STRONG_FLAG;  //Увеличить вес
    }
    else if (GLOBAL_FLAG_TX & LIMIT_SWITCH_DOWN_FLAG)
    {
      TestScrewStep--;
    }
  }*/
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Экстренная остановка шагового двигателя                                    ┃
//(таймер ускорения, таймер генерации step)                                  ┃
void EMERGENCY_STOP(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS)//                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  GLOBAL_FLAG_TX &= ~START_UP_FLAG;
  GLOBAL_FLAG_TX &= ~START_DOWN_FLAG;
  GLOBAL_FLAG_TX |= STOP_FLAG;
  LL_TIM_DisableCounter(TIMA);
  LL_TIM_DisableCounter(TIMS);
  Freq_step = MIN_FREQ;
  GLOBAL_FLAG_TX &= ~STOPING_FLAG;
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void INITIALIZATION_END(void)//                                              ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
/*  if (GLOBAL_FLAG_TX & INITIALIZATION_FLAG)
  {
    MOTOR_STOP();
    GLOBAL_FLAG_TX &= ~INITIALIZATION_FLAG;
    step = MAX_STEP;
    Freq_step = MIN_FREQ;
  }*/
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Генерация сигнала step для драйвера                                        ┃
void StepGenerate_IRQHandler_Callback(TIM_TypeDef *TIMS)//                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIMS))
  {
    LL_TIM_ClearFlag_UPDATE(TIMS);
    if (GLOBAL_FLAG_TX & DIRECTION_FLAG)
    {
      if ((!(GLOBAL_FLAG_TX & LIMIT_SWITCH_DOWN_FLAG)) &
          (!(GLOBAL_FLAG_TX & STOP_BUTTON_FLAG)))
      {
        LL_GPIO_TogglePin(DRV_STEP_GPIO_Port, DRV_STEP_Pin);
        if (step > 0)
        {
          step--;
        }
        return;
      }
      else
      {
        step = 0; //Дополнительная инициализация во время работы при наезде на концевик
        keystrokeCounter = 0;
      }
    }
    else
    {
      if ((!(GLOBAL_FLAG_TX & LIMIT_SWITCH_UP_FLAG)) &
          (!(GLOBAL_FLAG_TX & STOP_BUTTON_FLAG)))
      {
        LL_GPIO_TogglePin(DRV_STEP_GPIO_Port, DRV_STEP_Pin);
        if (step < MAX_STEP)
        {
          step++;
        }
        return;
      }
      else
      {
        step = MAX_STEP; //Дополнительная инициализация во время работы при наезде на концевик
        keystrokeCounter = 0;
      }
    }
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Таймер ускорения/торможения                                                ┃
void Acceleration_IRQHandler_Callback(TIM_TypeDef *TIMA, TIM_TypeDef *TIMS)//┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIMA))
  {
    LL_TIM_ClearFlag_UPDATE(TIMA);
    TIMS->ARR  =  CALC_CYCLE(Freq_step);
//    uint32_t temp = CALC_CYCLE(Freq_step);
//    LL_TIM_SetAutoReload(TIMS, temp);
//    LL_TIM_OC_SetCompareCH3(TIMS, temp/2);
    
    if ((Freq_step > MIN_FREQ) &&
        (GLOBAL_FLAG_TX & STOPING_FLAG))
    {
      Freq_step--;
    }
    else if ((Freq_step < Max_freq_step) &&
            ((GLOBAL_FLAG_TX & START_UP_FLAG)   ||
             (GLOBAL_FLAG_TX & START_DOWN_FLAG)))
    {
      Freq_step++;
    }
  }
}
