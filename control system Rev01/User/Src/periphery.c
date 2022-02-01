#include "periphery.h"
#include "main.h"
volatile Button_StatusTypeDef Button_Status = BUT_OFF;
volatile uint8_t NumButPressed = 0; //Количество нажатых кнопок
volatile uint32_t delayCounter_flag = 0;
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Разрешить прерывания																											 ┃
void irq_enable(void)//																											 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  NVIC_EnableIRQ(RTC_IRQn);
  NVIC_EnableIRQ(TIM2_IRQn);
  NVIC_EnableIRQ(TIM6_IRQn);
  NVIC_EnableIRQ(TIM21_IRQn);
  NVIC_EnableIRQ(TIM22_IRQn);
//  NVIC_EnableIRQ(SPI1_IRQn);
  NVIC_EnableIRQ(USART1_IRQn);
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Запретить прерывания                                                       ┃
void irq_disable(void)//                                                     ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  NVIC_DisableIRQ(RTC_IRQn);
  NVIC_DisableIRQ(TIM2_IRQn);
  NVIC_DisableIRQ(TIM6_IRQn);
  NVIC_DisableIRQ(TIM21_IRQn);
  NVIC_DisableIRQ(TIM22_IRQn);
//  NVIC_DisableIRQ(SPI1_IRQn);
  NVIC_DisableIRQ(USART1_IRQn);
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Включить отладку по jtag находясь в standby                                ┃
void DebugOnStandByMode(void)//                                              ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
  DBGMCU->CR |= DBGMCU_CR_DBG_STOP;
  DBGMCU->CR |= DBGMCU_CR_DBG_STANDBY;
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Действия при работе микропереключателей                                    ┃
void SwitchControl(void)//                                                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_STP_MAX_GPIO_Port, LIMIT_STP_MAX_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_UP_FLAG;
  }
  else if ( NumButPressed == 0 )
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_UP_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_STP_MIN_GPIO_Port, LIMIT_STP_MIN_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_DOWN_FLAG;
  }
  else if ( NumButPressed == 0 )
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_DOWN_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_BDC_MAX_GPIO_Port, LIMIT_BDC_MAX_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_FORWARD_FLAG;
  }
  else if ( NumButPressed == 0 )
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_FORWARD_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_BDC_MIN_GPIO_Port, LIMIT_BDC_MIN_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_BACKWARD_FLAG;
  }
  else if ( NumButPressed == 0 )
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_BACKWARD_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(Panic_GPIO_Port, Panic_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= STOP_BUTTON_FLAG;
  }
  else
  {
    GLOBAL_FLAG_TX &= ~STOP_BUTTON_FLAG;
  }
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Выполнение действия по активному флагу                                     ┃
void ButtonFlagControl(void)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  NumButPressed = (_Bool)( GLOBAL_FLAG_RX & BUT_UP_FLAG               )+ 
                  (_Bool)( GLOBAL_FLAG_RX & BUT_DOWN_FLAG             )+
                  (_Bool)( GLOBAL_FLAG_RX & BUT_FORWARD_FLAG          )+
                  (_Bool)( GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG         )+
                  (_Bool)( GLOBAL_FLAG_RX & BUT_STRONG_FLAG           )+
                  (_Bool)( GLOBAL_FLAG_RX & BUT_WEAK_FLAG             )+
                  (_Bool)( GLOBAL_FLAG_RX & BUT_SPEED_UP_FLAG         )+
                  (_Bool)( GLOBAL_FLAG_RX & BUT_SPEED_DOWN_FLAG       )+
                  (_Bool)( GLOBAL_FLAG_TX & TEST_SCREW_FLAG           )+
                  (_Bool)( GLOBAL_FLAG_TX & INITIALIZATION_FLAG       )+
 //                 (_Bool)( GLOBAL_FLAG_TX & STOPING_FLAG              )+
                  (_Bool)( GLOBAL_FLAG_TX & CARRIAGE_CALIBRATION_FLAG )+
                  (_Bool)( GLOBAL_FLAG_TX & STOP_BUTTON_FLAG          );

  if ((NumButPressed == 1) && (Button_Status != BUT_DENY))
  {
    Button_Status = (Button_StatusTypeDef)(
    (_Bool)( GLOBAL_FLAG_RX & BUT_UP_FLAG         )*0x01+ 
    (_Bool)( GLOBAL_FLAG_RX & BUT_DOWN_FLAG       )*0x02+
    (_Bool)( GLOBAL_FLAG_RX & BUT_FORWARD_FLAG    )*0x03+
    (_Bool)( GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG   )*0x04+
    (_Bool)( GLOBAL_FLAG_RX & BUT_STRONG_FLAG     )*0x05+
    (_Bool)( GLOBAL_FLAG_RX & BUT_WEAK_FLAG       )*0x06+
    (_Bool)( GLOBAL_FLAG_RX & BUT_SPEED_UP_FLAG   )*0x07+
    (_Bool)( GLOBAL_FLAG_RX & BUT_SPEED_DOWN_FLAG )*0x08);
  }
  else
  {
    if (NumButPressed == 0)
    {
      Button_Status = BUT_OFF;
    }
    else if (NumButPressed > 1)
    {
      Button_Status = BUT_DENY;
    }
    //Сброс флага ускорения изменения скорости каретки
    quantityCounter = 0;
  }
  
         if (( Button_Status == BUT_UP ) && //Кнопка подъемник вверх
             ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_UP_FLAG;
      LL_GPIO_SetOutputPin(PP_UP_GPIO_Port,PP_UP_Pin);
      Sleep_Reset();
    }
    else if ( Button_Status == BUT_DOWN )/* && //Кнопка подъемник вниз
             ( Battery_Status != CHARGING ))*/
    {
      GLOBAL_FLAG_TX |= BUT_DOWN_FLAG;
      LL_GPIO_SetOutputPin(PP_DOWN_GPIO_Port,PP_DOWN_Pin);
      Sleep_Reset();
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_FORWARD_FLAG )) && //Кнопка подъемник вперед
               ( Button_Status == BUT_FORWARD                ) &&
               (delayCounter_flag == 0))
    {
      if (!(GLOBAL_FLAG_TX & BUT_FORWARD_FLAG))
      {
        GLOBAL_FLAG_TX |= BDC_ON_FLAG;
        DC_DC_SW_ENABLE;
        //Выход на плату подъемника
        //LL_GPIO_SetOutputPin(PP_LEFT_GPIO_Port,PP_LEFT_Pin);
        LL_GPIO_SetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);
        delayCounter_flag = 10000;
        GLOBAL_FLAG_TX |= BUT_FORWARD_FLAG;
        return;
      }
      delayCounter_flag = 1000;
      bdc_ON();
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_BACKWARD_FLAG )) && //Кнопка подъемник назад
               ( Button_Status == BUT_BACKWARD                ) &&
               (delayCounter_flag == 0))
    {
      if (!(GLOBAL_FLAG_TX & BUT_BACKWARD_FLAG))
      {
        GLOBAL_FLAG_TX |= BDC_ON_FLAG;
        DC_DC_SW_ENABLE;
        //Выход на плату подъемника
        //LL_GPIO_SetOutputPin(PP_RIGHT_GPIO_Port,PP_RIGHT_Pin);
        LL_GPIO_SetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);
        delayCounter_flag = 10000;
        GLOBAL_FLAG_TX |= BUT_BACKWARD_FLAG;
        return;
      }
      delayCounter_flag = 1000;
      bdc_ON();
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_UP_FLAG )) && //Кнопка разгрузка увеличить вес
               ( Button_Status == BUT_STRONG            ) &&
               ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_STRONG_FLAG;
      if (!(GLOBAL_FLAG_TX & DIRECTION_FLAG))
        GLOBAL_FLAG_TX &= ~STOPING_FLAG;
      MOTOR_START_DOWN(AC_TIM, ST_TIM);
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_DOWN_FLAG)) && //Кнопка разгрузка уменьшить вес
               ( Button_Status == BUT_WEAK          ) &&
               ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_WEAK_FLAG;
      if (GLOBAL_FLAG_TX & DIRECTION_FLAG)
        GLOBAL_FLAG_TX &= ~STOPING_FLAG;
      MOTOR_START_UP(AC_TIM, ST_TIM);
    }
    else if (( Button_Status == BUT_SPEED_UP ) && //Кнопка скорость +
             ( delayCounter_flag == 0))
    {
      bdc_speed_up();
      GLOBAL_FLAG_TX &= ~BDC_CURRENT_LIMIT_FLAG;
      delayCounter_flag = 10000;
    }
    else if (( Button_Status == BUT_SPEED_DOWN ) && //Кнопка скорость -
             ( delayCounter_flag == 0))
    {
      bdc_speed_down();
      GLOBAL_FLAG_TX &= ~BDC_CURRENT_LIMIT_FLAG;
      delayCounter_flag = 10000;
    }
    else if (( GLOBAL_FLAG_TX & INITIALIZATION_FLAG ) && //Инициализация привода пружины
             (!(GLOBAL_FLAG_TX & STOP_BUTTON_FLAG)))
    {
      Motor_Initialization(AC_TIM, ST_TIM, TIM7);
    }
    else if (delayCounter_flag == 0)
    {
      LL_GPIO_ResetOutputPin(PP_UP_GPIO_Port,PP_UP_Pin);
      LL_GPIO_ResetOutputPin(PP_DOWN_GPIO_Port,PP_DOWN_Pin);
      GLOBAL_FLAG_TX &= ~(  BUT_UP_FLAG          |
                            BUT_DOWN_FLAG        |
                            BUT_FORWARD_FLAG     |
                            BUT_BACKWARD_FLAG    |
                            BUT_STRONG_FLAG      |
                            BUT_WEAK_FLAG        |
                            BUT_SPEED_UP_FLAG    |
                            BUT_SPEED_DOWN_FLAG
                        );
      
      MOTOR_STOP(AC_TIM, ST_TIM, TIM2);
      bdc_OFF();
    }
  //Декремент флага для ограницения количества(частоты) вызова функций
  if (delayCounter_flag > 0)
    delayCounter_flag--;
//  return;
}
