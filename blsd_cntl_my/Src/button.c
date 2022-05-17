//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file proto.h*/
/**\brief Реализация протокола обмена через modbus.*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "atomic.h"
#include "swtimer.h"
#include "button.h"
#include "6Step_Lib_Hall.h"
#include "ramp.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
	BUTTON_RELEASE,
	BUTTON_PRESS,
	BUTTON_DBLCLICK
} ButtonState_TypeDef;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t button_debounce_timer_id;
static uint8_t button_dblclick_timer_id;
__IO static ButtonState_TypeDef button_state;
__IO static ButtonState_TypeDef button_init_state;
static uint8_t button_press_counter = 0;

extern uint8_t Enable_start_button;
/* Private function prototypes -----------------------------------------------*/
static void SwTimerDebounceCB(void);
static void SwTimerDblClickCB(void);
/* Private functions ---------------------------------------------------------*/
static void SwTimerDebounceCB(void)
{
/*	ButtonState_TypeDef prev_state = button_state;

	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET && button_init_state == BUTTON_PRESS)
	{
		button_state = BUTTON_PRESS;
		barrier();
	}
	else if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET &&
								button_init_state == BUTTON_RELEASE &&
								prev_state == BUTTON_PRESS)
	{
		button_state = BUTTON_RELEASE;
		barrier();
		if (prev_state == BUTTON_PRESS)
		{
			button_press_counter++;
			barrier();
		}
	}
 */
}

/*----------------------------------------------------------------------------*/
static void SwTimerDblClickCB(void)
{
	/*
	if (button_press_counter >= 2)
	{
		button_state = BUTTON_DBLCLICK;
		barrier();
		//MC_ChangeDirection();
		RampMotorSetFlag(RAMP_REVERSE_Pos);
	}
	else
	{
		//MC_EXT_button_SixStep();
		RampMotorSetFlag(RAMP_START_STOP_Pos);
	}
	button_press_counter = 0;
	Enable_start_button = TRUE;
	*/
}

/* Exported functions --------------------------------------------------------*/
void InitButton(void)
{
	/*
	button_debounce_timer_id = SwTimer_Add(SWTIMER_MODE_STOP, BUTTON_DEBOUNCE_TIMER_TIMEOUT, SwTimerDebounceCB);
	button_dblclick_timer_id = SwTimer_Add(SWTIMER_MODE_STOP, BUTTON_DOUBLE_CLICK_TIMER_TIMEOUT, SwTimerDblClickCB);
	*/
}
/*----------------------------------------------------------------------------*/
void PressButtonCB(uint16_t GPIO_Pin)
{
	/*
	//UNUSED(GPIO_Pin);
	if (HAL_GPIO_ReadPin(B1_GPIO_Port, GPIO_Pin) == GPIO_PIN_RESET)
	{
		button_init_state = BUTTON_PRESS;
		barrier();
	}
	else
	{
		button_init_state = BUTTON_RELEASE;
		barrier();
	}
	SwTimer_SetMode(button_debounce_timer_id, SWTIMER_MODE_SINGLE);
	SwTimer_SetMode(button_dblclick_timer_id, SWTIMER_MODE_SINGLE);
	return ;
	*/
}
/*----------------------------------------------------------------------------*/
