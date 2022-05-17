//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author Новиков В. А.*/
/**\file дзс1114_swtimer.c*/
/**\brief Драйвер програмного таймера*/

#include "stm32f4xx.h"
#include "swtimer.h"
//#include "<MCU_MODEL.h>"

/*Аппартный таймер, используемый програмными таймерами*/
#define SW_TIMER               TIM11
/*Хандлер аппаратного таймера*/
#define SW_TIMER_HNDL			(&htim11)
/*Имя обработчика прерываний програмного таймера*/
//#define SWTIMERIRQHANDLER          TIMER16_1_IRQHandler
/*Номер прерывания*/
//#define SW_TIMER_IRQ               TIMER_16_1_IRQn
/*Канал аппаратного таймера, используемый програмными таймерами*/
//#define SW_TIMER_CAP_CH            (1)
/*Частота проограмного таймера*/
#define SW_TICKRATE_HZ             (100000)

extern TIM_HandleTypeDef htim11;


/*"Медленная" половина обработчика прерываний аппаратного таймера.*/
/* Добавлена в обработчик PendSv в файл "stm32f4xx_it.c"
void PendSV_Handler(void)
{
	SwTimer_Update();
}
*/
/*Инциализировать аппаратный таймер, включить его прерывание переполнения(обновления) 
  или компаратора и запустить его*/
void HAL_SwTimer_Init(void)
{
	HAL_TIM_Base_Start_IT(SW_TIMER_HNDL);
	return;
}

/*Обнулить счетчик аппаратного таймера и сбросить его флаги(если есть)*/
void HAL_SwTimer_Sync(void)
{
	__HAL_TIM_DISABLE_IT(SW_TIMER_HNDL, TIM_IT_UPDATE);
	WRITE_REG(SW_TIMER_HNDL->Instance->CNT, 0);
	__HAL_TIM_ENABLE_IT(SW_TIMER_HNDL, TIM_IT_UPDATE);
	return;
}

/*Отключить прерывание аппаратного таймера*/
void HAL_SwTimer_Lock(void)
{
	__HAL_TIM_DISABLE_IT(SW_TIMER_HNDL, TIM_IT_UPDATE);
  return;
}

/*Возобновить прерывания аппаратного таймера*/
void HAL_SwTimer_Unlock(void)
{
  	__HAL_TIM_ENABLE_IT(SW_TIMER_HNDL, TIM_IT_UPDATE);
  return;
}
