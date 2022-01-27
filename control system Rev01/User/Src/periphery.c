#include "periphery.h"
#include "main.h"
volatile Button_StatusTypeDef Button_Status = BUT_OFF;
volatile uint8_t NumButPressed = 0; //Количество нажатых кнопок
volatile uint8_t bdcSpeed_flag = 0;
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
  else
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_UP_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_STP_MIN_GPIO_Port, LIMIT_STP_MIN_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_DOWN_FLAG;
  }
  else
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_DOWN_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_BDC_MAX_GPIO_Port, LIMIT_BDC_MAX_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_FORWARD_FLAG;
  }
  else
  {
    GLOBAL_FLAG_TX &= ~LIMIT_SWITCH_FORWARD_FLAG;
  }
/******************************************************************************/
  if ((LL_GPIO_IsInputPinSet(LIMIT_BDC_MIN_GPIO_Port, LIMIT_BDC_MIN_Pin)) == 1)
  {
    GLOBAL_FLAG_TX |= LIMIT_SWITCH_BACKWARD_FLAG;
  }
  else
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
    //Флаги для ускорения изменения скорости и для ограницения количества вызова функций
    //bdc_speed_up и bdc_speed_down. Синхронизировано с оборотами кольцевого буфера.
    quantityCounter = 0;
    bdcSpeed_flag = 0;
    bdc_OFF();
  }
  
         if (( Button_Status == BUT_UP ) && //Кнопка подъемник вверх
             ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_UP_FLAG;
      LL_GPIO_SetOutputPin(PP_UP_GPIO_Port,PP_UP_Pin);
      Sleep_Reset();
    }
    else if (( Button_Status == BUT_DOWN ) && //Кнопка подъемник вниз
             ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_DOWN_FLAG;
      LL_GPIO_SetOutputPin(PP_DOWN_GPIO_Port,PP_DOWN_Pin);
      Sleep_Reset();
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_FORWARD_FLAG )) && //Кнопка подъемник вперед
               ( Button_Status == BUT_FORWARD                ))/* &&
               (bdcSpeed_flag == 0))*/
    {
    //  bdcSpeed_flag = 1;
      GLOBAL_FLAG_TX |= BUT_FORWARD_FLAG;
      if (!(LL_TIM_CC_IsEnabledChannel(BDC_TIM, BDC_TIM_CH)))
      {
//      LL_GPIO_SetOutputPin(PP_LEFT_GPIO_Port,PP_LEFT_Pin); //Выход на плату подъемника
        LL_GPIO_SetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);
//        LL_mDelay(5);
      }
      bdc_ON();
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_BACKWARD_FLAG )) && //Кнопка подъемник назад
               ( Button_Status == BUT_BACKWARD                ))/* &&
               (bdcSpeed_flag == 0))*/
    {
    //  bdcSpeed_flag = 1;
      GLOBAL_FLAG_TX |= BUT_BACKWARD_FLAG;
      if (!(LL_TIM_CC_IsEnabledChannel(BDC_TIM, BDC_TIM_CH)))
      {
//      LL_GPIO_SetOutputPin(PP_RIGHT_GPIO_Port,PP_RIGHT_Pin); //Выход на плату подъемника
        LL_GPIO_SetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);
//        LL_mDelay(5);
      }
      bdc_ON();
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_UP_FLAG )) && //Кнопка разгрузка увеличить вес
               ( Button_Status == BUT_STRONG            ) &&
               ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_STRONG_FLAG;
      GLOBAL_FLAG_TX &= ~STOPING_FLAG;
      MOTOR_START_DOWN(AC_TIM, ST_TIM);
    }
    else if ((!( GLOBAL_FLAG_TX & LIMIT_SWITCH_DOWN_FLAG)) && //Кнопка разгрузка уменьшить вес
               ( Button_Status == BUT_WEAK          ) &&
               ( Battery_Status != CHARGING ))
    {
      GLOBAL_FLAG_TX |= BUT_WEAK_FLAG;
      GLOBAL_FLAG_TX &= ~STOPING_FLAG;
      MOTOR_START_UP(AC_TIM, ST_TIM);
    }
    else if (( Button_Status == BUT_SPEED_UP ) && //Кнопка скорость +
             ( bdcSpeed_flag == 0))
    {
      bdc_speed_up();
      GLOBAL_FLAG_TX &= ~BDC_CURRENT_LIMIT_FLAG;
      bdcSpeed_flag = 5;
    }
    else if (( Button_Status == BUT_SPEED_DOWN ) && //Кнопка скорость -
             ( bdcSpeed_flag == 0))
    {
      bdc_speed_down();
      GLOBAL_FLAG_TX &= ~BDC_CURRENT_LIMIT_FLAG;
      bdcSpeed_flag = 5;
    }
    else if (( GLOBAL_FLAG_TX & INITIALIZATION_FLAG ) && //Инициализация привода пружины
             ( NumButPressed == 1 ))
    {
      Motor_Initialization(AC_TIM, ST_TIM, TIM7);
    }
//    else if ( GLOBAL_FLAG_TX & CARRIAGE_CALIBRATION_FLAG ) //Калибровка привода пружины
//    {
//      Count_Num_Step();
//    }
//    else if ( GLOBAL_FLAG_TX & TEST_SCREW_FLAG ) //Тестирование ШВП пружины
//    {
//      Test_Screw();
//    }
    else
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
    }
    return;
}
