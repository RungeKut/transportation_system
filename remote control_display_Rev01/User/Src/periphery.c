#include "periphery.h"
#include "main.h"

volatile Button_StatusTypeDef Button_Status = BUT_OK;

/*----------------------------------------------------------------*/
void Vibro_touch(void) 
/*----------------------------------------------------------------*/
{
	LL_GPIO_TogglePin(VIBRO_GPIO_Port, VIBRO_Pin);
	return;
}

/*----------------------------------------------------------------*/
//Сканирование кнопок пульта и установка флагов в регистре приема
void ScanButton(void)
/*----------------------------------------------------------------*/
{
  if	(LL_GPIO_IsInputPinSet(SA1_GPIO_Port, SA1_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_UP_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_UP;
      Sound_PWM(2000, 1);
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_UP_FLAG;
    if (Button_Status == BUT_UP)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if (LL_GPIO_IsInputPinSet(SA2_GPIO_Port, SA2_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_DOWN_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_DOWN;
      Sound_PWM(2050, 1);
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_DOWN_FLAG;
    if (Button_Status == BUT_DOWN)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if	(LL_GPIO_IsInputPinSet(SA3_GPIO_Port, SA3_Pin) == 1) 
  {
    GLOBAL_FLAG_TX |= BUT_BACKWARD_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_BACKWARD;
      if (GLOBAL_FLAG_RX & BDC_CURRENT_LIMIT_FLAG)
      {
        Sound_PWM(2500, 50);
      }
      else if (GLOBAL_FLAG_RX & LIMIT_SWITCH_BACKWARD_FLAG)
      {
        Sound_Play(track_limit);
      }
      else
      {
        Sound_PWM(2100, 1);
      }
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_BACKWARD_FLAG;
    if (Button_Status == BUT_BACKWARD)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if	(LL_GPIO_IsInputPinSet(SA4_GPIO_Port, SA4_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_FORWARD_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_FORWARD;
      if (GLOBAL_FLAG_RX & BDC_CURRENT_LIMIT_FLAG)
      {
        Sound_PWM(2500, 50);
      }
      else if (GLOBAL_FLAG_RX & LIMIT_SWITCH_FORWARD_FLAG)
      {
        Sound_Play(track_limit);
      }
      else
      {
        Sound_PWM(2150, 1);
      }
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_FORWARD_FLAG;
    if (Button_Status == BUT_FORWARD)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if	(LL_GPIO_IsInputPinSet(SA7_GPIO_Port, SA7_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_WEAK_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_WEAK;
      if (GLOBAL_FLAG_RX & LIMIT_SWITCH_DOWN_FLAG)
      {
        Sound_Play(track_limit);
      }
      else
      {
        Sound_PWM(2200, 1);
      }
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_WEAK_FLAG;
    if (Button_Status == BUT_WEAK)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if	(LL_GPIO_IsInputPinSet(SA8_GPIO_Port, SA8_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_STRONG_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_STRONG;
      if (GLOBAL_FLAG_RX & LIMIT_SWITCH_UP_FLAG)
      {
        Sound_Play(track_limit);
      }
      else
      {
        Sound_PWM(2250, 1);
      }
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_STRONG_FLAG;
    if (Button_Status == BUT_STRONG)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if	(LL_GPIO_IsInputPinSet(SA5_GPIO_Port, SA5_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_SPEED_DOWN_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_SPEED_DOWN;
      Sound_PWM(2300, 1);
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_SPEED_DOWN_FLAG;
    if (Button_Status == BUT_SPEED_DOWN)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
  if	(LL_GPIO_IsInputPinSet(SA6_GPIO_Port, SA6_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_SPEED_UP_FLAG;
    if (Button_Status == BUT_OK)
    {
      Button_Status = BUT_SPEED_UP;
      Sound_PWM(2350, 1);
    }
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_SPEED_UP_FLAG;
    if (Button_Status == BUT_SPEED_UP)
    {
      Button_Status = BUT_OK;
    }
	}
/*----------------------------------------------------------------*/
/*		if	(LL_GPIO_IsInputPinSet(SA9_GPIO_Port, SA9_Pin) == 1)
  {
		GLOBAL_FLAG_RX |= BUT_-_FLAG;
		keystrokeCounter++;
	}
  else
  {
		GLOBAL_FLAG_RX &= ~BUT_-_FLAG;
	}

		if	(LL_GPIO_IsInputPinSet(SA10_GPIO_Port, SA10_Pin) == 1)
  {
		GLOBAL_FLAG_RX |= BUT_-_FLAG;
		keystrokeCounter++;
	}
  else
  {
		GLOBAL_FLAG_RX &= ~BUT_-_FLAG;
	}*/
  
	return;
}

