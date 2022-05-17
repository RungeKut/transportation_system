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


#ifndef RAMP_H_
#define RAMP_H_
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "narg.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
 /*      NAME                	FUNC       				CODE                   */
 #define RAMP_FLAG_TABLE(ENTRY) 														\
   ENTRY(RESERVE,         		reserve,    			0) 								\
   ENTRY(START_STOP,      		start_stop, 			RAMP_START_STOP_Pos) 			\
   ENTRY(REVERSE,         		reverse,    			RAMP_REVERSE_Pos) 				\
   ENTRY(NEED_REVERSE,    		need_reverse,	  		RAMP_NEED_REVERSE_Pos) 			\
   ENTRY(SLOW_DOWN,       		slow_down,  			RAMP_SLOW_DOWN_Pos) 			\
   ENTRY(SPEED_UP,        		speed_up,   			RAMP_SPEED_UP_Pos) 				\
   ENTRY(NEED_START_STOP, 		need_start_stop, 		RAMP_NEED_START_STOP_Pos)  		\
   ENTRY(START_STOP_SPEED_UP, 	reserve, 				RAMP_START_STOP_SPEED_UP_Pos)



 #define EXPAND_AS_RAMP_POS_ENUM(a,b,c) RAMP_##a##_Pos,
 typedef enum {
   RAMP_FLAG_TABLE(EXPAND_AS_RAMP_POS_ENUM)
   RAMP_LAST_Pos
 } RAMP_pos_enum_t;

 #define RAMP_FLAG_ENTRY_NUM_ NARGS(RAMP_FLAG_TABLE(EXPAND_AS_RAMP_POS_ENUM) RAMP_LAST_Pos)
 #define RAMP_FLAG_ENTRY_NUM (RAMP_FLAG_ENTRY_NUM_-1)

#if ((RAMP_FLAG_ENTRY_NUM >= 32))
	#error "Type uint32_t does not fit for ramp_flags, probably RAMP_FLAG_TABLE too long!"
#endif

#define RAMP_FLAG_MASK (((1<<RAMP_LAST_Pos)-1) & ~(1<<RAMP_RESERVE_Pos))
/* Exported functions --------------------------------------------------------*/
 void RampMotorTask(void);
 void RampMotorInit(void);
 void RampMotorSetFlag(uint8_t flag_pos);
 bool IsRampMotorStopped(void);

#ifdef __cplusplus
}
#endif


#endif /* RAMP_H_ */
