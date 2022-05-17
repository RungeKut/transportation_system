#include "bdc_motor.h"
#include "main.h"

volatile uint16_t Encoder_Period = 0;    //*0.1ms
volatile uint16_t Encoder_DutyCicle = 0;  //*0.1ms
volatile uint32_t Encoder_Freqency = 0;  //mHz
volatile uint16_t DutyToBDC = 0;
volatile uint32_t Encoder_Speed = 0; // см/сек
volatile uint32_t quantityCounter = 0;
uint8_t SpeedEnable = 0;

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
  if (!(GLOBAL_FLAG_TX & BDC_PWM_ON_FLAG))
  {
//    bdc_TIM2_Init();
    LL_TIM_SetPrescaler(BDC_TIM, BDC_PWM_Freqency);
    BOOST_ENABLE;
//    DC_DC_SW_ENABLE;
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
    GLOBAL_FLAG_TX |= BDC_PWM_ON_FLAG;
    Encoder_Speed = 0;
  }

    CurrentBDC_GET();
    if (BDC_mAmp > 300)
      SpeedEnable = 1;
    if (SpeedEnable == 1)
    Encoder_Speed_GET();
    Sleep_Reset();
    SetDutyCycleBDC(BDC_TIM, DutyToBDC);
  
    if ((Encoder_Speed < speed) & (DutyToBDC + 3 < BDCMaxDutyCicle))
    {
      DutyToBDC = DutyToBDC + 3;
    }
    else if ((Encoder_Speed <= speed) & (DutyToBDC < BDCMaxDutyCicle))
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
//      DutyToBDC = DutyToBDC + 5;
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
    SpeedEnable = 0;
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
    LL_GPIO_ResetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);
    LL_GPIO_ResetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);
    GLOBAL_FLAG_TX &= ~BDC_PWM_ON_FLAG;
    GLOBAL_FLAG_TX &= ~BDC_ON_FLAG;
    
//    LL_TIM_DeInit(TIM2);
  }
}
///*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//// (вход, установка, п, и, д, период в секундах, мин.выход, макс. выход)     ┃
//uint16_t computePID(float input, float setpoint, float kp, float ki, float kd, float dt, uint16_t minOut, uint16_t maxOut)//                                                         ┃
///*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
//{
//  float err = setpoint - input;
//  static float integral = 0, prevErr = 0;
//  integral = constrain(integral + (float)err * dt * ki, minOut, maxOut);
//  float D = (err - prevErr) / dt;
//  prevErr = err;
//  return constrain(err * kp + integral + D * kd, minOut, maxOut);
//}
