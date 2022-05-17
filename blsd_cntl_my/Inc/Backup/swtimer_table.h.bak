//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author Новиков В. А.*/
/**\file swtimer_table.h*/
/**\brief Описываются програмные таймеры, используемые в приложении.*/

#ifndef SWTIMER_TABLE_H
#define SWTIMER_TABLE_H

#include <stdint.h>

/**Таблица програмных таймеров*/
/*                      NAME                   	TIME((n+1)*1ms)         */
#define SOFT_TIMER_TABLE(SOFT_TIMER_ENTRY) 							\
        SOFT_TIMER_ENTRY(CNT_RESET_TIMER,				499)			\
        SOFT_TIMER_ENTRY(GREEN_LED_TIMER,			499)			\
		SOFT_TIMER_ENTRY(PWR_TIMER,					299)			\
		SOFT_TIMER_ENTRY(MB_DELAY_TIMER,			1)				\
		SOFT_TIMER_ENTRY(GRP_CMD_EXPIRED_TIMER,		999)			\
		SOFT_TIMER_ENTRY(BUTTON_DEBOUNCE_TIMER,		49)				\
		SOFT_TIMER_ENTRY(BUTTON_DOUBLE_CLICK_TIMER,	499)			\
		SOFT_TIMER_ENTRY(RAMP_MOTOR_TIMER,			9)				\
		SOFT_TIMER_ENTRY(STOP_MOTOR_TIMER,			4)				\
		SOFT_TIMER_ENTRY(WAIT_SHORT_COIL_TIMER,		9)
        
/**Количество програмных таймеров (размер таблицы)*/
#define SWTIMER_NUM (sizeof(swtimer_size_t))

#define EXPAND_AS_SWTIMER_STRUCT(a,b) uint8_t a;
typedef struct 
{
  SOFT_TIMER_TABLE(EXPAND_AS_SWTIMER_STRUCT)
} swtimer_size_t;

/**Порядковые номера (имена) програмных таймеров*/
#define EXPAND_AS_SWTIMER_NAME_ENUM(a,b) a,
typedef enum tagSOFTTIMERINDEX
{
  SOFT_TIMER_TABLE(EXPAND_AS_SWTIMER_NAME_ENUM)
  SOFT_TIMER_LAST
} SWTIMERINDEX;

/**Величины периодов (задержек) програмных таймеров*/
#define EXPAND_AS_SWTIMER_TIMEOUT_ENUM(a,b) a##_TIMEOUT = b,
typedef enum tagSWTIMEOUT
{
  SOFT_TIMER_TABLE(EXPAND_AS_SWTIMER_TIMEOUT_ENUM)
} SWTIMEOUT;

#endif /*SWTIMER_TABLE_H*/
