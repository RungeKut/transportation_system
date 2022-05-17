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

/**********Begin**********Battery_Status********************/
typedef enum
{
  BAT_OK            = 0x00,
  END_CHARGE        = 0x01, //Зарядка завершена, устройство питается от зарядника
  CHARGING          = 0x02, //Идет процесс зарядки
  BAT_ERR_MISSING   = 0x03, //Батарея не подключена
  BAT_ERR_IsOLD     = 0x04, //Батарея состарилась
  BAT_ERR_REPLACE   = 0x05, //Батарея требует замены
  BAT_ERR_CONNECT   = 0x06
} Battery_StatusTypeDef;
extern volatile Battery_StatusTypeDef Battery_Status;
/**********End************Battery_Status********************/

/**********Begin**********BDC_PWM_Freqency******************/
typedef enum
{
  PWM_16kHz = 0x00,
  PWM_8kHz  = 0x01,
  PWM_5k3Hz = 0x02,
  PWM_4kHz  = 0x03,
  PWM_3k2Hz = 0x04,
  PWM_2k6Hz = 0x05,
  PWM_2k2Hz = 0x06,
  PWM_2kHz  = 0x07,
  PWM_1kHz  = 0x15
} BDC_PWM_FreqencyTypeDef;
extern volatile BDC_PWM_FreqencyTypeDef BDC_PWM_Freqency;
/**********End************BDC_PWM_Freqency******************/

/**********Begin**********UserMacros************************/
#define BOOST_ENABLE              LL_GPIO_ResetOutputPin(Enable_DC_DC_GPIO_Port, Enable_DC_DC_Pin); GLOBAL_FLAG_TX |= BOOST_ENABLE_FLAG;
#define BOOST_DISABLE             LL_GPIO_SetOutputPin(Enable_DC_DC_GPIO_Port, Enable_DC_DC_Pin); GLOBAL_FLAG_TX &= ~BOOST_ENABLE_FLAG;
#define DC_DC_SW_ENABLE           LL_GPIO_SetOutputPin(DC_DC_SW_GPIO_Port, DC_DC_SW_Pin); GLOBAL_FLAG_TX |= DC_DC_SW_FLAG;
#define DC_DC_SW_DISABLE          LL_GPIO_ResetOutputPin(DC_DC_SW_GPIO_Port, DC_DC_SW_Pin); GLOBAL_FLAG_TX &= ~DC_DC_SW_FLAG;
#define STEPDRIVER_DISABLE        LL_GPIO_SetOutputPin(DRV_ENABLE_GPIO_Port, DRV_ENABLE_Pin); GLOBAL_FLAG_TX &= ~DRV_ENABLE_FLAG;
#define STEPDRIVER_ENABLE         LL_GPIO_ResetOutputPin(DRV_ENABLE_GPIO_Port, DRV_ENABLE_Pin); GLOBAL_FLAG_TX |= DRV_ENABLE_FLAG;
#define STEPDRIVER_DIRECTION_UP   LL_GPIO_SetOutputPin(DRV_DIRECTION_GPIO_Port, DRV_DIRECTION_Pin);
#define STEPDRIVER_DIRECTION_DOWN LL_GPIO_ResetOutputPin(DRV_DIRECTION_GPIO_Port, DRV_DIRECTION_Pin);
#define SendSize                  PACK_SIZE + CRC_SIZE
#define SysTime                   0x00 == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
#define CALC_CYCLE(freq)          (((F_CPU / (FREQUENCY_STEP + 1)) / 2) / freq)
#define MAX_FREQ                  54 * MAX_FREQ_RPM //максимальная частота, Гц сигнала step
/**********End************UserMacros************************/

/**********Begin*****Установка параметров разгрузки*********/
#define F_CPU           32e6   //частота процессора
#define MIN_FREQ        20     //минимальная частота, Гц сигнала step
#define MAX_FREQ_RPM    100    //максимальная частота, об/мин
#define ACCELERATION    5000   //максимальное ускорение, Гц/сек2 сигнала step
#define ACCELERATION_EN 0      //включить/отключить ускорение
#define MIN_WEIGHT      5      //минимальная сила разгрузки, кг
#define MAX_WEIGHT      70     //максимальная сила разгрузки, кг
#define MAX_STEP        573000 //максимальная число шагов, шт
#define MaxNumPush      50     //максимальное количество нажаний на пульт до переинициализации шагового двигателя, шт
#define MAX_SPEED_BDC   50     //максимальная скорость карретки, см/сек
#define MIN_SPEED_BDC   10     //минимальная скорость карретки, см/сек
#define ENCODER_HOLES   20      //количесвто щелей диска энкодера, шт
#define ENCODER_DIAM    12     //диаметр колеса в зацеплении с рельсой, см
#define BATTERY_MAX_V   252    //напряжение полностью заряженного аккумулятора
#define BATTERY_MIN_V   175    //напряжение полностью разряженного аккумулятора
#define BATTERY_LIMIT   5      //% заряда, ниже которого ограничивается подъем и регулировка рвзгрузки
#define CRC_SIZE        4      //Сколько байт занимает CRC в пакете 4*8=32bit
#define PACK_SIZE       28     //Размер посылки, байт
#define BDCMaxDutyCicle 1000   //Максимальная скважность ШИМ мотора карретки, %*10
#define BDCMinDutyCicle 0      //Минимальная скважность ШИМ мотора карретки, %*10
/**********End*******Установка параметров разгрузки*********/

/**********Begin*********_Port_*****************************/
#define Type_of_timer      TIM_TypeDef        //Имя типа таймера стандартной библиотеки
#define AC_TIM             TIM21              //Таймер, используемый для задания ускорения
#define ST_TIM             TIM22              //Таймер, используемый для генерации шагов
#define Type_of_RTC        RTC_TypeDef        //Имя типа таймера реального времени для сна мк
#define RTC_TIM            RTC                //Таймер, используемый для задержки засыпания мк
#define Type_of_DMA        DMA_TypeDef        //Имя типа DMA стандартной библиотеки
#define N_DMA              DMA1               //Имя DMA стандартной библиотеки
#define BDC_TIM            TIM3               //Таймер для генерации ШИМ мотора карретки. PSC=0, MODE=UP, ARR=0x07CF, CKD=0, Auto-reload=0, PWMmode=1, Output_compare=1, FastMode=0, CHPolarity=0
#define BDC_TIM_CH         LL_TIM_CHANNEL_CH3 //Канал таймера для генерации ШИМ мотора карретки.
#define ENCODER_TIM        TIM2               //Таймер для обработки энкодера
#define ENCODER_DirectCH   LL_TIM_CHANNEL_CH1 //Канал тригера фронта
#define ENCODER_IndirectCH LL_TIM_CHANNEL_CH2 //Канал тригера спада
#define ENCODER_IRQ        TIM2_IRQn          //Имя прерывания енкодера
/**********End***********_Port_*****************************/

extern volatile char      tx_str[SendSize];
extern volatile uint8_t   weight;
extern volatile uint8_t   speed;
extern volatile uint8_t   battery;
extern volatile uint32_t  GLOBAL_FLAG_RX;
extern volatile uint32_t  GLOBAL_FLAG_TX;
extern volatile uint32_t  step;
extern volatile uint32_t  Freq_step;
extern volatile uint32_t  Max_freq_step;
extern volatile uint32_t  TIM6_FLAG;
extern volatile uint8_t   CarriageStep;
extern volatile uint8_t   TestScrewStep;
extern volatile uint8_t   keystrokeCounter;

/* BEGIN Prototypes */
void ClickRelay(uint8_t);
void StartUpConfig(void);
uint32_t uint_constrain(uint32_t, uint32_t, uint32_t);
/* END Prototypes */

#endif /* CONFIG_H */
