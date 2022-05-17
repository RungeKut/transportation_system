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
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "ramp.h"
#include "bitbanding.h"
#include "6Step_Lib_Hall.h"
#include "swtimer.h"
#include "hall.h"

extern void MC_SetSpeedTargetLocked(uint16_t value);
extern void MC_UnlockSpeedTarget(uint16_t value);

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint16_t mem_amplitude;
	uint16_t mem_target_amplitude;
	bool 	mem_potentiometer;
	bool	lock;
} ramp_ctx_t;

#define EXPAND_AS_RAMP_SIZE_STRUCT(a,b,c) uint8_t ramp_##a;
typedef struct {
  RAMP_FLAG_TABLE(EXPAND_AS_RAMP_SIZE_STRUCT)
} ramp_table_size_t;

#define EXPAND_AS_RAMP_FLAG_DEFINITION(a,b,c) uint32_t ramp_flag_##a : 1;
typedef union
{
  uint32_t all;
  struct {
    RAMP_FLAG_TABLE(EXPAND_AS_RAMP_FLAG_DEFINITION)
  };
} ramp_flags_t;

typedef void (*ramp_flag_func_t)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define NUM_OF_RAMP_FLAGS (sizeof(ramp_table_size_t))

/* Private variables ---------------------------------------------------------*/
static uint32_t ramp_flags;
static ramp_ctx_t ramp_ctx_saved;
static ramp_ctx_t ramp_ctx_tmp;
static uint8_t 	swtim_stop_motor;
static uint8_t	swtim_ramp_motor;

/* Private function prototypes -----------------------------------------------*/
#define EXPAND_AS_RAMP_PROTOTYPE(a,b,c) static void process_##b##_flag(void);
RAMP_FLAG_TABLE(EXPAND_AS_RAMP_PROTOTYPE)

#define EXPAND_AS_RAMP_JUMP_TABLE(a,b,c) process_##b##_flag,
static ramp_flag_func_t ramp_jump_table[NUM_OF_RAMP_FLAGS] =
{
  RAMP_FLAG_TABLE(EXPAND_AS_RAMP_JUMP_TABLE)
};

static bool motor_stopped(void);

/* Private functions ---------------------------------------------------------*/

static bool motor_stopped(void)
{
  bool res = false;
  static uint8_t time_mult = 1;

  if ( SwTimer_GetMode(swtim_stop_motor) == SWTIMER_MODE_STOP)
  {//If the timer is stopped and the slow_down flag is set
    SwTimer_SetCompare(swtim_stop_motor, (time_mult * STOP_MOTOR_TIMER_TIMEOUT)); //Initialize the timer with preset value
    SwTimer_SetCounter(swtim_stop_motor, 0);
    SwTimer_SetMode(swtim_stop_motor, SWTIMER_MODE_SINGLE); // and run it.
  }
  if ( SwTimer_GetStatus(swtim_stop_motor) != 0 )
  {
    if (IsHallSensorStateChanged())
    {
    	SetHallStateChangeFlag(0); // Reset Hall state changed flag
    	time_mult++;
    }
    else
    {
      res = true;
      time_mult = 1;
    }
  }
  return res;
}


//! @brief Process reserve flag.
//!     - empty function, we would not have to get here
//!
//!
//! @param none
//! @return none
static void process_reserve_flag(void)
{

}
/*----------------------------------------------------------------------------*/
static void process_start_stop_flag(void)
{
	BIT_BAND_SRAM(ramp_flags, RAMP_START_STOP_Pos) = 0;
	if (MC_IsMotorRun())
	{
		ramp_ctx_tmp.mem_target_amplitude = 0;
		BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) = 1;// Установка флага замедления для последующей остановки мотора.
		BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) = 1;// Установка флага реальной остановки вращения.
	}
	else
	{
		ramp_ctx_tmp.mem_target_amplitude = ramp_ctx_saved.mem_target_amplitude;
		BIT_BAND_SRAM(ramp_flags, RAMP_SPEED_UP_Pos) = 1;//Установка флага ускорения.
		BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) = 1; // Установка флага включения двигателя.
	}
}



/**Обработать события требования запуска или остановки.
Произвести реальную остановку или запуск двигателя.*/
static void process_need_start_stop_flag(void)
{
	if (MC_IsMotorRun())
	{ // В случае вращения мотора и установленного флага сброса скорости происходит выключение мотора.
		if (BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) == 0)
		{
			BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) = 0;
			MC_StopMotor();
		}
	}
	else
	{ //Отсановка мотора, в случае, если установлен флаг ускорения, то он сбрасывается и производится включения мотора.
		if (BIT_BAND_SRAM(ramp_flags, RAMP_START_STOP_SPEED_UP_Pos) == 1)
		{
			BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) = 0;
			BIT_BAND_SRAM(ramp_flags, RAMP_START_STOP_SPEED_UP_Pos) = 0;
			MC_StartMotor();
		}
	}
}


//! @brief Process rising reverse flag
//!     - Reverse flag will be risen when reverse push button is pressed.
//!     - Process long hold event in particular way setting hold flag,
//!     - on button release rotation direction are changed, after
//!     - zeroing speed
//!
//! @param none
//! @return none
/***/
static void process_reverse_flag(void)
{
	BIT_BAND_SRAM(ramp_flags, RAMP_REVERSE_Pos) = 0;
	if ( MC_GetRawPulseValue() > 0 && MC_IsMotorRun() )
	{// if amplitude is not zero
		ramp_ctx_tmp.mem_target_amplitude = 0;
		BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) = 1; // set slow down flag to stop motor
		BIT_BAND_SRAM(ramp_flags, RAMP_NEED_REVERSE_Pos) = 1; // set flag to make motor reverse later after slow down
	} else
	{
		MC_ChangeDirection(); 		// change direction of rotation
	}
}

//! @brief Process need reverse flag to switch the direction of the motor rotation
//!     -
//! @param none
//!
static void process_need_reverse_flag(void)
{
	if ( (BIT_BAND_SRAM(ramp_flags, RAMP_NEED_REVERSE_Pos) == 1) && (BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) == 0) )
	{// if motor is stopped
		MC_ChangeDirection(); // change direction of rotation
		ramp_ctx_tmp.mem_target_amplitude = ramp_ctx_saved.mem_target_amplitude;
		BIT_BAND_SRAM(ramp_flags, RAMP_NEED_REVERSE_Pos) = 0; //reset flag
		BIT_BAND_SRAM(ramp_flags, RAMP_SPEED_UP_Pos) = 1; // set speed up flag to restore speed up to previously saved value
	}
}

//! @brief Process slow down flag to end up at zero motor speed
//!     -
//! @param none
//!
static void process_slow_down_flag(void)
{
  uint16_t 	current_amplitude = MC_GetRawPulseValue();
  uint16_t 	target_amplitude  = ramp_ctx_tmp.mem_target_amplitude;

  if (SwTimer_GetMode(swtim_ramp_motor) == SWTIMER_MODE_STOP && BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) != 0)
  {//If the timer is stopped and the slow_down flag is set
	  SwTimer_SetCounter(swtim_ramp_motor, 0);					// reset swtimer counter
	  SwTimer_SetMode(swtim_ramp_motor, SWTIMER_MODE_SINGLE); 	// and run it
  }
  if ( (current_amplitude > target_amplitude) && (SwTimer_GetStatus(swtim_ramp_motor) != 0) )
  {// If the amplitude is not zero and the soft timer flag is set
	  //current_amplitude >>= 1; // Decrease the amplitude dividing it by 2
	  if (current_amplitude >= MC_GetAcceleration())
	  {
		  current_amplitude -= MC_GetAcceleration();
	  } else
	  {
		  current_amplitude = target_amplitude;
	  }
	  MC_SetRawPulseValue(current_amplitude); // Save the new amplitude value
  }
  if (BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) || BIT_BAND_SRAM(ramp_flags, RAMP_NEED_REVERSE_Pos))
  {
	  if ((current_amplitude == target_amplitude) && IsRampMotorStopped())
	  {
		  BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) = 0;
	  }
  }
  else if (current_amplitude == target_amplitude)
  {
	  BIT_BAND_SRAM(ramp_flags, RAMP_SLOW_DOWN_Pos) = 0;
  }
}

//! @brief Process slow down flag to end up at previosly saved in mem_amplitude motor speed value
//!     -
//! @param none
//!
static void process_speed_up_flag(void)
{
	uint16_t 	current_amplitude = MC_GetRawPulseValue();
	uint16_t 	target_amplitude  = ramp_ctx_tmp.mem_target_amplitude;

	if ( (BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) == 1) &&
			(BIT_BAND_SRAM(ramp_flags, RAMP_START_STOP_SPEED_UP_Pos) == 0) )
	{
		current_amplitude = 0;
		MC_SetRawPulseValue(current_amplitude); //set current amplitude to 0
		BIT_BAND_SRAM(ramp_flags, RAMP_START_STOP_SPEED_UP_Pos) = 1;
	}
	if ( SwTimer_GetMode(swtim_ramp_motor) == SWTIMER_MODE_STOP	&&
			BIT_BAND_SRAM(ramp_flags, RAMP_SPEED_UP_Pos) != 0 )
	{//If the timer is stopped and speed_up flag is set
		SwTimer_SetCounter(swtim_ramp_motor, 0);
		SwTimer_SetMode(swtim_ramp_motor, SWTIMER_MODE_SINGLE);
	}
	if ( current_amplitude < target_amplitude && SwTimer_GetStatus(swtim_ramp_motor) != 0 )
	{// If the curren_amplitude is less than previously saved mem_amplitude and the timer set the flag
		current_amplitude +=(uint16_t)MC_GetAcceleration();
		if (current_amplitude <= target_amplitude)
		{
			MC_SetRawPulseValue(current_amplitude);
		}
		else
		{
			MC_SetRawPulseValue(target_amplitude);
		}
		//MC_SetRawPulseValue( (current_amplitude < target_amplitude) ? current_amplitude : target_amplitude ); // save the amplitude
	}
	if ( current_amplitude >= target_amplitude )
	{// If the amplitude is caught the previously memorized value up
		//MC_SetRawPulseValue(target_amplitude); // store the memorized value
		if ( BIT_BAND_SRAM(ramp_flags, RAMP_NEED_START_STOP_Pos) == 0 )
		{//if motor runs clear flag
			BIT_BAND_SRAM(ramp_flags, RAMP_SPEED_UP_Pos) = 0; // clear the flag
		}
	}
}

inline static void save_ramp_ctx(void)
{
	ramp_ctx_saved.mem_amplitude = MC_GetRawPulseValue();
	ramp_ctx_saved.mem_target_amplitude = MC_GetSpeedTarget();
	ramp_ctx_saved.mem_potentiometer = MC_IsPotentiometerActive();
	MC_EnablePotentiometer(0);
	MC_SetSpeedTargetLocked(ramp_ctx_saved.mem_target_amplitude);
}

inline static void restore_ramp_ctx(void)
{
	MC_UnlockSpeedTarget(ramp_ctx_saved.mem_target_amplitude);
	MC_EnablePotentiometer(ramp_ctx_saved.mem_potentiometer);
}


/* Exported functions --------------------------------------------------------*/

void RampMotorTask(void)
{
  //U8 offset = 0;

  if (ramp_flags & RAMP_FLAG_MASK) // маскирование зарезервированных битов.
  {//В случае, когда некоторые флаги установлены, происходит проверка каждого из них.
	  if (ramp_ctx_saved.lock != true)
	  {
		  save_ramp_ctx();
		  ramp_ctx_saved.lock = true;
		  memcpy(&ramp_ctx_tmp, &ramp_ctx_saved, sizeof(ramp_ctx_t));
	  }
	  for (uint8_t i=1; i < NUM_OF_RAMP_FLAGS; i++)
	  {
		  //uint32_t flag = (1<<i);
		  if ( (BIT_BAND_SRAM(ramp_flags, i) != 0) /*&& ((offset = pb_offset_table[flag]) != 0)*/ )
		  {
			  ramp_jump_table[i](); // вызов соответствующей функции.
		  }
	  }
  }
  else
  {
	  if (ramp_ctx_saved.lock == true)
	  {
		  restore_ramp_ctx();
		  ramp_ctx_saved.lock = false;
	  }
  }
}

void RampMotorInit(void)
{
	swtim_ramp_motor = SwTimer_Add(SWTIMER_MODE_STOP, RAMP_MOTOR_TIMER_TIMEOUT, NULL);
	swtim_stop_motor = SwTimer_Add(SWTIMER_MODE_STOP, STOP_MOTOR_TIMER_TIMEOUT, NULL);
}

void RampMotorSetFlag(uint8_t flag_pos)
{
	BIT_BAND_SRAM(ramp_flags, flag_pos) = 1;
}

bool IsRampMotorStopped(void)
{
	uint8_t real_dir;
	bool result = false;

	result = motor_stopped() || (MC_GetRealDirection(&real_dir) && real_dir != MC_GetDirection());
	return result;
}
