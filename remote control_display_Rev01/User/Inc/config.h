#ifndef CONFIG_H
#define CONFIG_H

/*
Вставить в заголовочный файл main.h в раздел USER CODE BEGIN Includes
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "config.h"
#include "periphery.h"
#include "bdc_motor.h"
#include "rs485.h"
#include "step_motor.h"
#include "power.h"
#include "dma.h"

*/

#include "main.h"

#define BUT_UP_FLAG                0x00000001 //Кнопка подъемник вверх
#define BUT_DOWN_FLAG              0x00000002 //Кнопка подъемник вниз
#define BUT_FORWARD_FLAG           0x00000004 //Кнопка подъемник вперед
#define BUT_BACKWARD_FLAG          0x00000008 //Кнопка подъемник назад

#define BUT_STRONG_FLAG            0x00000010 //Кнопка разгрузка увеличить вес
#define BUT_WEAK_FLAG              0x00000020 //Кнопка разгрузка уменьшить вес
#define INITIALIZATION_FLAG        0x00000040 //Nнициализация привода
#define STOP_FLAG                  0x00000080 //Мотор остановлен

#define START_UP_FLAG              0x00000100 //Мотор должен вращаться в прямом направлении
#define START_DOWN_FLAG            0x00000200 //Мотор должен вращаться в обратном направлении
#define STOP_BUTTON_FLAG           0x00000400 //Нажата кнопка аварийной остановки
#define LIMIT_SWITCH_UP_FLAG       0x00000800 //Концевой выключатель при максимальном положении

#define LIMIT_SWITCH_DOWN_FLAG     0x00001000 //Концевой выключатель при минимальном положении
#define DIRECTION_FLAG             0x00002000 //Направление вращения
#define STOPING_FLAG               0x00004000 //Мотор в процессе торможения
#define MOVE_UP_FLAG               0x00008000 //Мотор вращается в прямом направлении

#define MOVE_DOWN_FLAG             0x00010000 //Мотор вращается в обратном направлении
#define ACCELERATION_FLAG          0x00020000 //Мотор в процессе ускорения
#define COUNT_NUM_STEP_FLAG        0x00040000 //Ведется подсчет кол-ва шагов м/у концевыми выключателями
#define STANDBY_MODE_FLAG          0x00080000 //Ждущий режим

#define BDC_PWM_ON_FLAG            0x00100000 //Двигатель каретки работает
#define BDC_ON_FLAG                0x00200000 //Реле направления включены
#define BOOST_ENABLE_FLAG          0x00400000 //DC/DC BOOST Включен
#define DC_DC_SW_FLAG              0x00800000 //Силовой ключ замкнут

#define CARRIAGE_CALIBRATION_FLAG  0x01000000 //Подсчет количества шагов м/у концевиками
#define TEST_SCREW_FLAG            0x02000000 //Карретка катается от концевика к концевику.
#define DRV_ENABLE_FLAG            0x04000000 //Step driver enable in
#define BUT_SPEED_UP_FLAG          0x08000000 //Кнопка скорость каретки увеличить

#define BUT_SPEED_DOWN_FLAG        0x10000000 //Кнопка скорость каретки уменьшить
#define LIMIT_SWITCH_FORWARD_FLAG  0x20000000 //Концевой выключатель каретка впереди
#define LIMIT_SWITCH_BACKWARD_FLAG 0x40000000 //Концевой выключатель каретка сзади
#define BDC_CURRENT_LIMIT_FLAG     0x80000000 //Ток мотора карретки за пределами уставки
/**********Begin**********GLOBAL_CHARGE_FLAG********************/
#define END_CHARGE_FLAG            0x00000001 //Зарядка завершена, устройство питается от зарядника
#define CHARGING_FLAG              0x00000002 //Идет процесс зарядки
//#define CHARGE_FLAG           0x00000004 //
//#define CHARGE_FLAG          0x00000008 //

//#define CHARGE_FLAG            0x00000010 //
//#define CHARGE_FLAG              0x00000020 //
//#define CHARGE_FLAG        0x00000040 //
//#define CHARGE_FLAG                  0x00000080 //
/**********End**********GLOBAL_CHARGE_FLAG********************/


#define SysTime 0x00 == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)

#define SleepTime_sec 30 //Время через которое засыпает пульт, макс 60секунд

/* Port BEGIN */
#define Type_of_timer      TIM_TypeDef        //Имя типа таймера стандартной библиотеки
#define TIM_Sleep          TIM15              //Таймер, используемый для задержки засыпания мк
#define Type_of_DMA        DMA_TypeDef        //Имя типа DMA стандартной библиотеки
#define N_DMA              DMA1               //Имя DMA стандартной библиотеки
#define CRC_SIZE           4                  //Сколько байт занимает CRC в пакете 4*8=32bit
#define PACK_SIZE          28                 //Размер посылки, байт
#define bufferSize         PACK_SIZE + CRC_SIZE
#define TIM_Sound          TIM3               //Таймер для генерации PWM звука
#define TIM_Sound_CH       LL_TIM_CHANNEL_CH1
#define TIM_Sound_PSC      359
#define TIM_Sound_ARR      2
#define Sound_bpm          10
/* Port END */

extern volatile char tx_str[bufferSize];
extern volatile char rx_str[bufferSize];
extern uint8_t flagRxEnable;
extern uint8_t flagTxEnable;
extern volatile uint8_t weight;
extern volatile uint8_t speed;
extern volatile uint8_t battery;
static const char rxStrKeyUp[7] = {0x53,0x79,0x73,0x74,0x65,0x6d,0x20};
static const char rxStrKeyDown[16] = {0x20,0x54,0x72,0x61,0x6e,0x73,0x70,0x6f,0x72,0x74,0x61,0x74,0x69,0x6f,0x6e,0x0d};
extern volatile uint32_t GLOBAL_FLAG_RX;
extern volatile uint32_t GLOBAL_FLAG_TX;
extern volatile uint8_t  GLOBAL_CHARGE_FLAG;

/* BEGIN Prototypes */
void StartUpConfig(void);

/* END Prototypes */

#endif /* CONFIG_H */
