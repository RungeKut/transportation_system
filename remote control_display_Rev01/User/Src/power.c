#include "power.h"
#include "main.h"

volatile uint8_t NumFlagSleepDeny = 0; //Количество флагов мешающих сну

/*----------------------------------------------------------------*/
void irq_enable(void)
/*----------------------------------------------------------------*/
{
  /* Peripheral interrupt init*/
  /* PVD_IRQn interrupt configuration */
  NVIC_SetPriority(PVD_IRQn, 0);
  NVIC_EnableIRQ(PVD_IRQn);
  /* RCC_CRS_IRQn interrupt configuration */
  NVIC_SetPriority(RCC_IRQn, 0);
  NVIC_EnableIRQ(RCC_IRQn);
}
/*----------------------------------------------------------------*/
void irq_disable(void)
/*----------------------------------------------------------------*/
{
  /* Peripheral interrupt init*/
  /* PVD_IRQn interrupt configuration */
//  NVIC_SetPriority(PVD_IRQn, 0);
  NVIC_DisableIRQ(PVD_IRQn);
  /* RCC_CRS_IRQn interrupt configuration */
//  NVIC_SetPriority(RCC_CRS_IRQn, 0);
  NVIC_DisableIRQ(RCC_IRQn);
}
/*----------------------------------------------------------------*/
void DebugOnStandByMode(void)
/*----------------------------------------------------------------*/
{
	DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
	DBGMCU->CR |= DBGMCU_CR_DBG_STOP;
	DBGMCU->CR |= DBGMCU_CR_DBG_STANDBY;
}
/*----------------------------------------------------------------*/
void GoToStandbyMode(void)
/*----------------------------------------------------------------*/
{
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  
/*  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);
  LL_IWDG_SetReloadCounter(IWDG, 0xFFFF);
  LL_IWDG_SetWindow(IWDG, 0xFFFF);
  LL_IWDG_DisableWriteAccess(IWDG);*/
  
  LL_GPIO_ResetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
  delayNs();						// пауза
  lcd_st7735s_RES_1();	// RST=1
	LL_mDelay(1);						// пауза
	lcd_st7735s_CS_0();		// CS=0
	delayNs();
  lcd_st7735s_send_cmd(ST7735S_DISPOFF);
  lcd_st7735s_CS_1();
	//DebugOnStandByMode();
//  irq_disable();
  LL_IWDG_ReloadCounter(IWDG);
/* с точки зрения ядра Cortex-M, что Stop, что Standby - это режим Deep Sleep */
/* поэтому надо в ядре включить Deep Sleep */
//	SCB->SCR |=  SCB_SCR_SLEEPDEEP_Msk;
  LL_LPM_EnableDeepSleep();
  /* флаг LPDS переводит внутренний регулятор напряжения в режим низкого энергопотребления */
  PWR->CR |= PWR_CR_LPDS;
/* флаг PDDS определяет выбор между Stop и Standby, его надо сбросить */
	PWR->CR |= PWR_CR_PDDS;
            
/* флаг Wakeup должн быть очищен, иначе есть шанс проснуться немедленно */    
	PWR->CR |= PWR_CR_CWUF;

/* стабилизатор питания в low-power режим, у нас в Stop потребления-то почти не будет */
//	PWR->CR |= PWR_CR_LPSDSR;

/* источник опорного напряжения Vref выключить автоматически */
//	PWR->CR |= PWR_CR_ULP;

/* выключили прерывания; пробуждению по ним это не помешает */
//unsigned state = irq_disable();

/* завершили незавершённые операция сохранения данных */
	__DSB();

/* заснули */
	__WFI();

/* переинициализация рабочих частот */
//init_clk();
//	SystemClock_Config();
	/* включаем источник 5V */
//	LL_GPIO_SetOutputPin(EN_5V_GPIO_Port, EN_5V_Pin);
/* после просыпания восстановили прерывания */
//irq_restore(state);
//	irq_enable();
}

volatile uint8_t Sleep_FLAG = 0;

/*----------------------------------------------------------------*/
void StandbyMode_IRQHandler(void)
/*----------------------------------------------------------------*/
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM_Sleep))
	{
		LL_TIM_ClearFlag_UPDATE(TIM_Sleep);
		if (Sleep_FLAG == 0)
		{
			Sleep_FLAG = 1;
			return;
		}
		Sleep_FLAG = 0;

		LL_mDelay(10);
		GoToStandbyMode();
		LL_TIM_SetCounter(TIM_Sleep,10);
	}
}
/*----------------------------------------------------------------*/
void ResetTIM_Sleep(void)
/*----------------------------------------------------------------*/
{
 NumFlagSleepDeny = (_Bool)( GLOBAL_FLAG_RX & ( BUT_UP_FLAG               |
                                                BUT_DOWN_FLAG             |
                                                BUT_FORWARD_FLAG          |
                                                BUT_BACKWARD_FLAG         |
                                                BUT_STRONG_FLAG           |
                                                BUT_WEAK_FLAG             |
                                                BUT_SPEED_UP_FLAG         |
                                                BUT_SPEED_DOWN_FLAG       |
                                             //   TEST_FLAG                 |
                                                INITIALIZATION_FLAG       |
                                                STOPING_FLAG              |
                                                CARRIAGE_CALIBRATION_FLAG |
                                                STOP_BUTTON_FLAG          ));
  if ( NumFlagSleepDeny != 0 )
  {
    LL_TIM_SetCounter(TIM_Sleep, 0x0);
  }
}
/*----------------------------------------------------------------*/
void SleepAfterWatchDog(void)
/*----------------------------------------------------------------*/
{
  //Если проснулись по пину wakeUp
  if ((PWR->CSR & PWR_CSR_WUF_Msk) == 1)
  {
    //Сбрасываем флаг пробуждения
    PWR->CR = PWR_CR_CWUF_Msk;
  }
  else /*if ((PWR->CSR & PWR_CSR_EWUP2_Msk) == 1)*/
  {
    GoToStandbyMode();
  }
}
