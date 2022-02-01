#include "bdc_motor.h"
#include "main.h"

volatile uint16_t Encoder_Period = 0;    //*0.1ms
volatile uint16_t Encoder_DutyCicle = 0;  //*0.1ms
volatile uint32_t Encoder_Freqency = 0;  //mHz
volatile uint16_t DutyToBDC = 0;
volatile uint32_t Encoder_Speed = 0; // см/сек
volatile uint32_t quantityCounter = 0;

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Старт                                                                      ┃
void bdc_StartUp(void)//                                                     ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  LL_TIM_CC_EnableChannel(BDC_TIM, BDC_TIM_CH);
  LL_TIM_EnableCounter(BDC_TIM);
//  LL_TIM_OC_SetCompareCH3(BDC_TIM, 0x03E8);
  SetDutyCycleBDC(BDC_TIM, 0);
//  NVIC_DisableIRQ(ENCODER_IRQ);
  LL_TIM_EnableIT_CC1(ENCODER_TIM);
  LL_TIM_CC_EnableChannel(ENCODER_TIM, ENCODER_DirectCH);
  LL_TIM_EnableIT_CC2(ENCODER_TIM);
  LL_TIM_CC_EnableChannel(ENCODER_TIM, ENCODER_IndirectCH);
  LL_TIM_EnableCounter(ENCODER_TIM);
//  LL_TIM_EnableIT_UPDATE(ENCODER_TIM);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Увеличить скорость карретки                                                ┃
void bdc_speed_up(void)//                                                    ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if ((quantityCounter == 0) && (speed < MAX_SPEED_BDC))
  {
    speed++;
    quantityCounter++;
  }
  else if  ((quantityCounter > 0) && (quantityCounter <= 3))
  {
    quantityCounter++;
  }
  else if  ((quantityCounter > 3) && (quantityCounter <= 15) && (speed < MAX_SPEED_BDC))
  {
    quantityCounter++;
    speed++;
  }
  else if  ((quantityCounter > 15) && (speed < MAX_SPEED_BDC - 10))
  {
    quantityCounter++;
    speed = speed + 10;
  }
  else if  ((quantityCounter > 15) && (speed < MAX_SPEED_BDC))
  {
    quantityCounter++;
    speed++;
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Уменьшить скорость карретки                                                ┃
void bdc_speed_down(void)//                                                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if ((quantityCounter == 0) && (speed > MIN_SPEED_BDC))
  {
    speed--;
    quantityCounter++;
  }
  else if  ((quantityCounter > 0) && (quantityCounter <= 5))
  {
    quantityCounter++;
  }
  else if  ((quantityCounter > 5) && (quantityCounter <= 15) && (speed > MIN_SPEED_BDC))
  {
    quantityCounter++;
    speed--;
  }
  else if  ((quantityCounter > 15) && (speed > MIN_SPEED_BDC + 10))
  {
    quantityCounter++;
    speed = speed - 10;
  }
  else if  ((quantityCounter > 15) && (speed > MIN_SPEED_BDC))
  {
    quantityCounter++;
    speed--;
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Задать скважность управления BDC, 100% = 1000duty                          ┃
void SetDutyCycleBDC(TIM_TypeDef *TIMx, uint16_t duty)//                     ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  duty = 1000 - duty;
  if ((duty > BDCMinDutyCicle) && (duty <= BDCMaxDutyCicle))
  {
    LL_TIM_OC_SetCompareCH3(TIMx, duty * 2 - 1);
  }
  else if (duty == 0)
  {
    LL_TIM_OC_SetCompareCH3(TIMx, 0x0000);
  }
}

//  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
//  LL_TIM_EnableCounter(TIM3);
//  LL_TIM_OC_SetCompareCH3(TIM3, 0x1F40);

//const  uint16_t AngleToCCR [] = {
//LL_TIM_OC_SetCompareCH3(TIM3, AngleToCCR[Angle]);

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Прерывание таймера энкодера                                                ┃
void Encoder_IRQHandler_Callback(TIM_TypeDef *TIMx)//                        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  LL_GPIO_TogglePin(XS11_GPIO_Port, XS11_Pin);
  if (LL_TIM_IsActiveFlag_UPDATE(TIMx))
  {
    LL_TIM_ClearFlag_UPDATE(TIMx);
  }
  else if (LL_TIM_IsActiveFlag_CC1(TIMx))
  {
    Encoder_Period = LL_TIM_IC_GetCaptureCH1(TIMx); //*0.1ms
    LL_TIM_ClearFlag_CC1(TIMx);
  }
  else if (LL_TIM_IsActiveFlag_CC2(TIMx))
  {
    Encoder_DutyCicle = LL_TIM_IC_GetCaptureCH2(TIMx); //*0.1ms
    LL_TIM_ClearFlag_CC2(TIMx);
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Посчитать скорость с энкодера                                              ┃
void Encoder_Speed_GET(void)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  Encoder_Period = LL_TIM_IC_GetCaptureCH1(ENCODER_TIM);    //*0.1ms
//  Encoder_DutyCicle = LL_TIM_IC_GetCaptureCH2(ENCODER_TIM); //*0.1ms
  uint32_t temp_speed;
  Encoder_Freqency = 10000000 / Encoder_Period; //mHz
  temp_speed = Encoder_Freqency * 3142 * ENCODER_DIAM / ENCODER_HOLES / 1000000;
  if (temp_speed <= MAX_SPEED_BDC * 10)
  {
    Encoder_Speed = temp_speed;
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Запуск периферии                                                           ┃
void bdc_ON(void)//                                                          ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (!(GLOBAL_FLAG_TX & BDC_ON_FLAG))
  {
//    bdc_TIM2_Init();
    LL_TIM_SetPrescaler(BDC_TIM, BDC_PWM_Freqency);
    BOOST_ENABLE;
    DC_DC_SW_ENABLE;
    LL_TIM_EnableIT_CC1(ENCODER_TIM);
//    LL_TIM_CC_EnableChannel(ENCODER_TIM, ENCODER_DirectCH);
    LL_TIM_EnableIT_CC2(ENCODER_TIM);
//    LL_TIM_CC_EnableChannel(ENCODER_TIM, ENCODER_IndirectCH);
//    LL_TIM_EnableCounter(ENCODER_TIM);
    LL_TIM_EnableIT_UPDATE(ENCODER_TIM);
//    __NVIC_EnableIRQ(ENCODER_IRQ);
    SetDutyCycleBDC(BDC_TIM, 0);
    LL_TIM_EnableCounter(BDC_TIM);
    LL_TIM_CC_EnableChannel(BDC_TIM, BDC_TIM_CH);
    GLOBAL_FLAG_TX |= BDC_ON_FLAG;
    Encoder_Speed = 0;
  }

    CurrentBDC_GET();
    Encoder_Speed_GET();
    Sleep_Reset();
    SetDutyCycleBDC(BDC_TIM, DutyToBDC);
  
    if ((Encoder_Speed < speed) & (DutyToBDC + 3 < BDCMaxDutyCicle))
    {
      DutyToBDC = DutyToBDC + 3;
    }
    else if ((Encoder_Speed == speed) & (DutyToBDC < BDCMaxDutyCicle))
    {
      DutyToBDC++;
    }
    else if ((Encoder_Speed > speed) & (DutyToBDC > BDCMinDutyCicle + 2))
    {
      DutyToBDC = DutyToBDC - 2;
    }
    else if ((DutyToBDC == 0) | (DutyToBDC > BDCMaxDutyCicle))
    {
      Encoder_Speed = 0;
      DutyToBDC = 0;
    }
    else
    {
      DutyToBDC = DutyToBDC + 5;
    }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Выключение периферии                                                       ┃
void bdc_OFF(void)//                                                         ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if (GLOBAL_FLAG_TX & BDC_ON_FLAG)
  {
    BOOST_DISABLE;
    DC_DC_SW_DISABLE;
    Encoder_Speed = 0;
    Encoder_Period = 0;
    Encoder_DutyCicle = 0;
    DutyToBDC = 0;
    LL_TIM_DisableIT_CC1(ENCODER_TIM);
//    LL_TIM_CC_DisableChannel(ENCODER_TIM, ENCODER_DirectCH);
    LL_TIM_DisableIT_CC2(ENCODER_TIM);
//    LL_TIM_CC_DisableChannel(ENCODER_TIM, ENCODER_IndirectCH);
//    LL_TIM_DisableCounter(ENCODER_TIM);
    LL_TIM_DisableIT_UPDATE(ENCODER_TIM);
//    __NVIC_DisableIRQ(ENCODER_IRQ);
    SetDutyCycleBDC(BDC_TIM, 0);
    LL_TIM_CC_DisableChannel(BDC_TIM, BDC_TIM_CH);
    LL_TIM_DisableCounter(BDC_TIM);
    LL_mDelay(50);
    LL_GPIO_ResetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);
    LL_GPIO_ResetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);
    GLOBAL_FLAG_TX &= ~BDC_ON_FLAG;
    
//    LL_TIM_DeInit(TIM2);
  }
}
void bdc_TIM2_Init(void)
{  
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**TIM2 GPIO Configuration
  PA5   ------> TIM2_CH1
  */
  GPIO_InitStruct.Pin = Encoder_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(Encoder_GPIO_Port, &GPIO_InitStruct);

  NVIC_SetPriority(TIM2_IRQn, 2);
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM_InitStruct.Prescaler = 3199;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerInput(TIM2, LL_TIM_TS_TI1FP1);
  LL_TIM_SetSlaveMode(TIM2, LL_TIM_SLAVEMODE_RESET);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_DisableIT_TRIG(TIM2);
  LL_TIM_DisableDMAReq_TRIG(TIM2);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_INDIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_FALLING);
}
