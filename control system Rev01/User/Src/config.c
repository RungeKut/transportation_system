#include "config.h"
#include "main.h"

volatile char tx_str[SendSize] = {
//                                |  GLOBAL_FLAG_TX   |bat |wght|sped|chrg|    |
//1    2    3    4    5    6    7 |  8    9    10   11|  12|  13|  14|  15|  16|  17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
0x53,0x79,0x73,0x74,0x65,0x6d,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x54,0x72,0x61,0x6e,0x73,0x70,0x6f,0x72,0x74,0x61,0x74,0x69,0x6f,0x6e,0x0d
};

volatile uint8_t   weight = 0;
volatile uint8_t   speed = 40;
volatile uint8_t   battery = 0;
volatile uint32_t  GLOBAL_FLAG_RX = 0;        //Принятый набор с ПУ (изменяемый ПУ)
volatile uint32_t  GLOBAL_FLAG_TX = 0;        //Набор для отправки в ПУ (изменяемый прошивкой)
volatile uint32_t  step = 0;                  //
volatile uint32_t  Freq_step = MAX_FREQ / 4;  //
volatile uint32_t  Max_freq_step = MIN_FREQ;  //
volatile uint32_t  TIM6_FLAG = 0;             //Время до окончания активности TX флагов
volatile uint8_t   CarriageStep = 1;          //
volatile uint8_t   TestScrewStep = 1;         //
volatile uint8_t   keystrokeCounter = 0;      //

volatile Battery_StatusTypeDef Battery_Status     = BAT_OK;
volatile BDC_PWM_FreqencyTypeDef BDC_PWM_Freqency = PWM_8kHz;

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

void _delay_us(uint16_t us)
{
  //загружаем число тактов ожидания (не более 2^24)// 30 - вычитание тактор накладных   расходов
  SysTick->LOAD = ((F_CPU/1000000)*us)-30;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //включаем таймер SysTick
   
  while(0x00 == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {}   //0x10000 - бит номер 16 //SysTick_CTRL_COUNTFLAG_Msk //(uint32_t)0x10000U
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //выключение таймер SysTick

}

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Щелкнуть релюшками несколько раз                                           ┃
void ClickRelay(uint8_t q)//                                                 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
   for (int i = 1; i<=q; i++)
  {
    LL_GPIO_SetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);
    LL_GPIO_SetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);
    LL_mDelay(20);
    LL_GPIO_ResetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);
    LL_GPIO_ResetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);
    LL_mDelay(100);
  }
}

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Конфигурация при загрузке процессора                                       ┃
void StartUpConfig(void)//                                                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  LL_GPIO_SetOutputPin(FLASH_CS_GPIO_Port, FLASH_CS_Pin);
  
  INA_INIT();
  
  BOOST_DISABLE;
  
//  DC_DC_SW_ENABLE;
  
//  ClickRelay(2);
  
//  RCC->APB1ENR |= RCC_APB1ENR_PWREN;//вкл тактирование PWR
//  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  
  rs485_StartUp();
  
//  Config_SysTick();
  
//  LL_TIM_DeInit(TIM2);
  
  bdc_StartUp();
  
  step_StartUp();
  
  CurrentBDC_StartUp();
  
//  ina239_StartUp();
  
  flash_StartUp();
  
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Ограничение возвращаемого значения диапазоном                              ┃
uint32_t uint_constrain(uint32_t x, uint32_t a, uint32_t b)//                ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  if ( a >= b ) return -1;
       if ( x <= a ) return a;
  else if ( x >= b ) return b;
  else return x;
}
