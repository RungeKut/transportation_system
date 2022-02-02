#include "periphery.h"
#include "main.h"

volatile Button_StatusTypeDef Button_Status = BUT_OK;

void Vibro_touch(void) 
{
	LL_GPIO_TogglePin(VIBRO_GPIO_Port, VIBRO_Pin);
	return;
}

//Сканирование кнопок пульта и установка флагов в регистре приема
void ScanButton(void) {
  if (LL_GPIO_IsInputPinSet(SA1_GPIO_Port, SA1_Pin) == 1)
  {
    GLOBAL_FLAG_TX |= BUT_DOWN_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_DOWN_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA2_GPIO_Port, SA2_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_UP_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_UP_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA3_GPIO_Port, SA3_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_BACKWARD_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_BACKWARD_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA4_GPIO_Port, SA4_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_FORWARD_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_FORWARD_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA7_GPIO_Port, SA7_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_WEAK_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_WEAK_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA8_GPIO_Port, SA8_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_STRONG_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_STRONG_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA5_GPIO_Port, SA5_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_SPEED_DOWN_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_SPEED_DOWN_FLAG;
	}
  if	(LL_GPIO_IsInputPinSet(SA6_GPIO_Port, SA6_Pin) == 1) {
    GLOBAL_FLAG_TX |= BUT_SPEED_UP_FLAG;
	}
  else
  {
    GLOBAL_FLAG_TX &= ~BUT_SPEED_UP_FLAG;
	}
/*		if	(LL_GPIO_IsInputPinSet(SA9_GPIO_Port, SA9_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_-_FLAG;
		keystrokeCounter++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_-_FLAG;
	}
		if	(LL_GPIO_IsInputPinSet(SA10_GPIO_Port, SA10_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_-_FLAG;
		keystrokeCounter++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_-_FLAG;
	}*/
  
	return;
}

