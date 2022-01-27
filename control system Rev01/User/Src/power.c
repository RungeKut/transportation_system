#include "power.h"
#include "main.h"

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//																																					 ┃
void GoToStandbyMode(void)//																								 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
	//DebugOnStandByMode();
//  irq_desable();

/* с точки зрения ядра Cortex-M, что Stop, что Standby - это режим Deep Sleep */
/* поэтому надо в ядре включить Deep Sleep */
	SCB->SCR |=  SCB_SCR_SLEEPDEEP_Msk;
/* флаг PDDS определяет выбор между Stop и Standby, его надо сбросить */
  PWR->CR |= PWR_CR_PDDS;
            
/* флаг Wakeup должн быть очищен, иначе есть шанс проснуться немедленно */    
  PWR->CR |= PWR_CR_CWUF;

/* стабилизатор питания в low-power режим, у нас в Stop потребления-то почти не будет */
//  PWR->CR |= PWR_CR_LPSDSR;

/* источник опорного напряжения Vref выключить автоматически */
//  PWR->CR |= PWR_CR_ULP;

/* выключили прерывания; пробуждению по ним это не помешает */
//unsigned state = irq_disable();

/* завершили незавершённые операция сохранения данных */
  __DSB();

/* заснули */
  __WFI();

/* переинициализация рабочих частот */
//init_clk();
//  SystemClock_Config();
  /* включаем источник 5V */
//  LL_GPIO_SetOutputPin(EN_5V_GPIO_Port, EN_5V_Pin);
/* после просыпания восстановили прерывания */
//irq_restore(state);
//  irq_enable();
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Таймер перехода в спящий режим                                             ┃
void Sleep_IRQHandler_Callback(TIM_TypeDef *TIMx)//                          ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIMx))
  {
    LL_TIM_ClearFlag_UPDATE(TIMx);
    if (TIM6_FLAG == 0)
    {
      TIM6_FLAG = 1;
      return;
    }
    TIM6_FLAG = 0;
    MOTOR_STOP(AC_TIM, ST_TIM, TIM2);
    /* отключаем источник 5V */
    DC_DC_SW_DISABLE;
//    DC5V_DISABLE;
    LL_mDelay(10);
//    GoToStandbyMode();
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Сброс таймера засыпания                                                    ┃
void Sleep_Reset(void)//                                                     ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  //LL_TIM_SetCounter(TIM6, 0x0000);
  
}
