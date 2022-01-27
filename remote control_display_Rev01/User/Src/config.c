#include "config.h"
#include "main.h"

volatile char tx_str[bufferSize] = {//                                    |   GLOBAL_FLAG_TX  |bat |wght|sped|chrg|buts|
//1    2    3    4    5    6    7    8    9    10   11   12   13   14   15|  16   17   18   19|  20|  21|  22|  23|  24|  25   26   27   28   29   30   31   32
0x54,0x72,0x61,0x6e,0x73,0x70,0x6f,0x72,0x74,0x61,0x74,0x69,0x6f,0x6e,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x53,0x79,0x73,0x74,0x65,0x6d,0x0d
};

volatile char rx_str[bufferSize];
uint8_t flagRxEnable = 0;
uint8_t flagTxEnable = 0;
volatile uint8_t  weight = 0;
volatile uint8_t  speed = 40;
volatile uint8_t  battery = 0;
volatile uint32_t GLOBAL_FLAG_RX = 0;        //Принятый набор с ПУ (изменяемый ПУ)
volatile uint32_t GLOBAL_FLAG_TX = 0;        //Набор для отправки в ПУ (изменяемый прошивкой)

volatile uint8_t  GLOBAL_CHARGE_FLAG = 0;

void Config_SysTick(void)
{
  //SysTick_CTRL_TICKINT_Msk - выключаем прерывания от SysTick
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
  //SysTick_CTRL_CLKSOURCE_Msk - Указываем процессор как источник прерывания
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  //Выставляем регистр перезагрузки примерно в полсекунды
  SysTick->LOAD = SystemCoreClock/2;  
  //SysTick_CTRL_ENABLE_Msk - включаем таймер SysTick
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}  

/*void _delay_us(uint16_t us)
{
  //загружаем число тактов ожидания (не более 2^24)// 30 - вычитание тактор накладных   расходов
  SysTick->LOAD = ((F_CPU/1000000)*us)-30;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //включаем таймер SysTick
   
  while(0x00 == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}   //0x10000 - бит номер 16 //SysTick_CTRL_COUNTFLAG_Msk //(uint32_t)0x10000U
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //выключение таймер SysTick

}*/

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Конфигурация при загрузке процессора                                       ┃
//                                                                           ┃
void StartUpConfig(void)//                                                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  DebugOnStandByMode();
  
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;//вкл тактирование PWR
//  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  
  LL_TIM_EnableIT_UPDATE(TIM2);
	LL_TIM_EnableCounter(TIM2);
  
  LL_TIM_SetPrescaler(TIM_Sleep, 1092 * SleepTime_sec);
  LL_TIM_SetAutoReload(TIM_Sleep, 0xFFFF);
  LL_TIM_SetCounter(TIM_Sleep, 0xFFFF);
	LL_TIM_EnableIT_UPDATE(TIM_Sleep);
	LL_TIM_EnableCounter(TIM_Sleep); //Таймер сна
  
	LL_TIM_EnableIT_UPDATE(TIM6);
//	LL_TIM_EnableCounter(TIM6);
						
  st7735_StartUp();
  
  rs485_StartUp();
}
