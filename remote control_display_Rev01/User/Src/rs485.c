#include "rs485.h"
#include "main.h"

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//                                                                           ┃
void DMA_Init(DMA_Channel_TypeDef* Channel, uint32_t Perif, uint32_t Mem, uint16_t Size, uint16_t Conf)
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
uint32_t tmp = 0;

    tmp = Channel->CCR;			// Копируем биты настройки 
    tmp &= CCR_CLEAR_Mask;	// и стираем все кроме битов EN. А он и так будет 0
    tmp |= Conf;						// Закатываем на результат наши биты настроек. 

    Channel->CNDTR = Size;	// Заполняем все нужные поля. Размер передчи
    Channel->CPAR = Perif;	// Адрес периферии
    Channel->CMAR = Mem;		// Адрес в памяти
    Channel->CCR = tmp;			// Записываем настройки в память.
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//                                                                           ┃
uint32_t DMA_GetCurrentDataCounter(DMA_Channel_TypeDef* Channel)//           ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  return Channel->CNDTR;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//                                                                           ┃
void DMA_Enable(DMA_Channel_TypeDef* Channel)//                              ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
	Channel->CCR |= DMA_CCR_EN; //Включаем определенный канал
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//                                                                           ┃
void DMA_Disable(DMA_Channel_TypeDef* Channel)//                             ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  Channel->CCR &= (uint16_t)(~DMA_CCR_EN); //Выключаем определенный канал
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// Процедурка деинициализации DMA нужна, чтобы вернуть все настройки в       ┃
// изначальное состояние, как после сброса:                                  ┃
void DMA_DeInit(DMA_Channel_TypeDef* Channel)//                              ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  Channel->CCR &= (uint16_t)(~DMA_CCR_EN);
  Channel->CCR = 0;
  Channel->CNDTR = 0;
  Channel->CPAR = 0;
  Channel->CMAR = 0;
  if (Channel == DMA1_Channel1)
  {
/* Reset interrupt pending bits for DMA1 Channel1 */
    DMA1->IFCR |= DMA1_Channel1_IT_Mask;
  }
  else if (Channel == DMA1_Channel2)
  {
/* Reset interrupt pending bits for DMA1 Channel2 */
    DMA1->IFCR |= DMA1_Channel2_IT_Mask;
  }
  else if (Channel == DMA1_Channel3)
  {
/* Reset interrupt pending bits for DMA1 Channel3 */
    DMA1->IFCR |= DMA1_Channel3_IT_Mask;
  }
  else if (Channel == DMA1_Channel4)
  {
/* Reset interrupt pending bits for DMA1 Channel4 */
    DMA1->IFCR |= DMA1_Channel4_IT_Mask;
  }
  else if (Channel == DMA1_Channel5)
  {
/* Reset interrupt pending bits for DMA1 Channel5 */
    DMA1->IFCR |= DMA1_Channel5_IT_Mask;
  }
  else if (Channel == DMA1_Channel6)
  {
/* Reset interrupt pending bits for DMA1 Channel6 */
    DMA1->IFCR |= DMA1_Channel6_IT_Mask;
  }
  else if (Channel == DMA1_Channel7)
  {
/* Reset interrupt pending bits for DMA1 Channel7 */
    DMA1->IFCR |= DMA1_Channel7_IT_Mask;
  }
//			else if (Channel == DMA2_Channel1)
//      {
//        /* Reset interrupt pending bits for DMA2 Channel1 */
//        DMA2->IFCR |= DMA2_Channel1_IT_Mask;
//      }
//      else if (Channel == DMA2_Channel2)
//      {
//        /* Reset interrupt pending bits for DMA2 Channel2 */
//        DMA2->IFCR |= DMA2_Channel2_IT_Mask;
//      }
//      else if (Channel == DMA2_Channel3)
//      {
//        /* Reset interrupt pending bits for DMA2 Channel3 */
//        DMA2->IFCR |= DMA2_Channel3_IT_Mask;
//      }
//      else if (Channel == DMA2_Channel4)
//      {
//        /* Reset interrupt pending bits for DMA2 Channel4 */
//        DMA2->IFCR |= DMA2_Channel4_IT_Mask;
//      }
//      else
//      {
//        if (Channel == DMA2_Channel5)
//        {
//          /* Reset interrupt pending bits for DMA2 Channel5 */
//          DMA2->IFCR |= DMA2_Channel5_IT_Mask;
//        }
//      }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// Старт                                                                     ┃
void rs485_StartUp(void)//                                                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  USART1_IRQEnable();
  
  DMA1_RX_Stop();
  DMA1_TX_Stop();
  
  DMA1_RX_ClearFlag();
  DMA1_TX_ClearFlag();
  
//  DMA1_RX_InitStart();
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// Сброс всех флагов приемника                                               ┃
void ClearButtonGlobalFlagTX(void)//                                         ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  GLOBAL_FLAG_TX &= ~(  BUT_UP_FLAG          |
                        BUT_DOWN_FLAG        |
                        BUT_FORWARD_FLAG     |
                        BUT_BACKWARD_FLAG    |
                        BUT_STRONG_FLAG      |
                        BUT_WEAK_FLAG        |
                        BUT_SPEED_UP_FLAG    |
                        BUT_SPEED_DOWN_FLAG
                    );
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// Сброс всех флагов приемника                                               ┃
void ClearButtonGlobalFlagRX(void)//                                         ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  GLOBAL_FLAG_RX &= ~(  BUT_UP_FLAG          |
                        BUT_DOWN_FLAG        |
                        BUT_FORWARD_FLAG     |
                        BUT_BACKWARD_FLAG    |
                        BUT_STRONG_FLAG      |
                        BUT_WEAK_FLAG        |
                        BUT_SPEED_UP_FLAG    |
                        BUT_SPEED_DOWN_FLAG
                    );
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_RX_ClearFlag(void)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  LL_DMA_ClearFlag_TE5(DMA1); //флаг прерывания в случае возникновения ошибки передачи данных в канале
  LL_DMA_ClearFlag_HT5(DMA1); //флаг прерывания по окончанию передачи половины передаваемых данных
  LL_DMA_ClearFlag_TC5(DMA1); //флаг прерывания по окончанию передачи всех данных
  LL_DMA_ClearFlag_GI5(DMA1); //флаг глобального прерывания от событий TE, HT или TC
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_TX_ClearFlag(void)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  LL_DMA_ClearFlag_TE4(DMA1); //флаг прерывания в случае возникновения ошибки передачи данных в канале
  LL_DMA_ClearFlag_HT4(DMA1); //флаг прерывания по окончанию передачи половины передаваемых данных
  LL_DMA_ClearFlag_TC4(DMA1); //флаг прерывания по окончанию передачи всех данных
  LL_DMA_ClearFlag_GI4(DMA1); //флаг глобального прерывания от событий TE, HT или TC
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_RX_InitStart(void)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  RCC->AHBENR  |= RCC_AHBENR_DMA1EN;       // Подали тактирование на DMA
  while (!(LL_USART_IsActiveFlag_TC(USART1))) {}
 
  LL_USART_Disable(USART1);
  
  DMA1_TX_Stop();
 
// Настраиваем канал
  DMA_Init( DMA1_Channel5,                   // 5-й канал 1 контроллера. 
              (uint32_t)&(USART1->RDR),      // Адрес откуда брать -- адрес регистра DR  в USART1
              (uint32_t)rx_str,              // Адрес куда класть результат
              bufferSize,                    // Указываем размер буфера.
              TransCompl_Int_Enable       |  // Прерывание по окончанию
              HalfCompl_Int_Disable       |  // Прерывание по половине
              TransError_Int_Disable      |  // Прерывание по ошибке
              ReadPerif                   |  // Читаем из периферии
              CircularMode_Disable        |  // Цикличный режим включен
              PeripheralInc_Disable       |  // Адрес периферии не увеличиваем
              MemoryInc_Enable            |  // А вот адрес приемного буфера увеличиваем, перебирая байт за байтом его
              PDataSize_B                 |  // Размер данных из периферии - байт
              MDataSize_B                 |  // Размер данных в памяти - байт
              DMA_Priority_Low            |  // Низкий приоритет
              M2M_Disable                 ); // Режим копирования память-память выключен. 
 
//  USART1->ISR &=~(USART_ISR_TC);           // Сбросили флаг передачи
  USART1->CR3 |=USART_CR3_DMAR;              // Включили сигнал от приема на DMA. В данном случае за UART1 на RX закреплен DMA1_Channel3)
  USART1->CR3 &=~USART_CR3_DMAT;
//  USART1->ICR |=USART_ICR_EOBCF;
//  USART1->ICR |=USART_ICR_ORECF;
//  LL_USART_ClearFlag_IDLE(USART1);
 // LL_USART_ClearFlag_
//  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_RX);
//  LL_USART_EnableDMAReq_RX(USART1);
// Включаем DMA
  DMA_Enable(DMA1_Channel5);
  LL_USART_Enable(USART1);
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_TX_InitStart(void)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  RCC->AHBENR  |= RCC_AHBENR_DMA1EN;          // Подали тактирование на DMA
//  while (!(LL_USART_IsActiveFlag_RXNE(USART1))) {}
  LL_USART_Disable(USART1);
  
  DMA1_RX_Stop();
  
  // Настраиваем канал
  DMA_Init( DMA1_Channel4,                   // Канал будет 4 контроллера 1. Nменно этот канал пинает USART1 при передаче
              (uint32_t)&(USART1->TDR),      // Указываем адрес регистра данных USART
              (uint32_t)tx_str,              // Указываем адрес буфера для передачи. В него предварительно записаны данные.
              bufferSize,                    // Указываем размер буфера. 
              TransCompl_Int_Enable       |  // Прерывание по окончанию
              HalfCompl_Int_Disable       |  // Прерывание по половине посылке
              TransError_Int_Disable      |  // Прерывание по ошибке
              ReadMemory                  |  // Читаем из памяти (а пишем в периферию)
              CircularMode_Disable        |  // Циклический режим
              PeripheralInc_Disable       |  // Адрес периферии не меняем
              MemoryInc_Enable            |  // А вот адрес памяти наоборот увеличиваем
              PDataSize_B                 |  // Данные размером в байт в периферии
              MDataSize_B                 |  // Данные размером в байт в памяти
              DMA_Priority_Low            |  // Низкий приоритет
              M2M_Disable                 ); // Не из памяти в память.
 
//  DMA1->IFCR = DMA_IFCR_CTCIF4;            // Сбрасываем флажок бита пустого регистра. Чтобы не отправить сразу нулевой байт
  USART1->CR3 |= USART_CR3_DMAT;             // Подключаем канал DMA к UART. Установив бит отвечающий за пинание уартом дма канала.
  USART1->CR3 &= ~USART_CR3_DMAR;
//  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX);
//  LL_USART_EnableDMAReq_TX(USART1);
// Включаем DMA
  DMA_Enable(DMA1_Channel4);
  LL_USART_Enable(USART1);
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_RX_Stop(void)//                                                    ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  DMA_Disable(DMA1_Channel5);                // Выключили канал.
  DMA_DeInit(DMA1_Channel5);                 // Обнулили DMA канал
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_TX_Stop(void)//                                                    ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  DMA_Disable(DMA1_Channel4);                // Выключили канал.
  DMA_DeInit(DMA1_Channel4);                 // Обнулили DMA канал
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void CorrectRX(void)//                                                       ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
	uint8_t i;
	for (i = 0; i<=6; i++) {
		if (rx_str[i] == rxStrKeyUp[i]) {}
		else {
			return;
		}
	}
	for (i = 0; i<=15; i++) {
		if (rx_str[i + 16] == rxStrKeyDown[i]) {}
		else {
			return;
		}
	}
	battery = rx_str[11];
	weight  = rx_str[12];
	speed   = rx_str[13];

	GLOBAL_FLAG_RX = (rx_str[7]  << 24) +
                   (rx_str[8]  << 16) +
                   (rx_str[9]  << 8)  +
                   (rx_str[10] << 0);
  
  GLOBAL_CHARGE_FLAG = rx_str[14];
	return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void CorrectTX(void)//                                                       ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
	tx_str[15] = GLOBAL_FLAG_TX >> 24;
	tx_str[16] = (GLOBAL_FLAG_TX << 8) >> 24;
	tx_str[17] = (GLOBAL_FLAG_TX << 16) >> 24;
	tx_str[18] = GLOBAL_FLAG_TX;
  tx_str[23] = Button_Status;
	return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_RecieveComplete(void)//                                            ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{ 
//  irq_disable();
//  flagRxEnable = 0;
//  DMA1_RX_ClearFlag();
//  if (flagTxEnable == 0) {
//    LL_mDelay(5);
//    DMA1_TX_InitStart();
//    flagTxEnable = 1;
//  }
//  crc_after_recive();
  CorrectRX();
//  irq_enable();
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_TransmitComplete(void)//                                           ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  irq_disable();
//  flagTxEnable = 0;
//  DMA1_TX_ClearFlag();
//  if (flagRxEnable == 0) {
//    LL_mDelay(1);
    DMA1_RX_InitStart();
//    flagRxEnable = 1;
//  }
  CorrectTX();
//  irq_enable();
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void DMA1_Errors(void)//                                                     ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  DMA1_RX_Stop();
  DMA1_TX_Stop();
  LL_mDelay(1000);
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Обработка прерывания DMA                                                   ┃
void DMA_IRQHandler_Callback(DMA_TypeDef *DMAx)//                            ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if(LL_DMA_IsActiveFlag_TC4(DMAx))
  {
    LL_DMA_ClearFlag_TC4(DMAx);
    DMA1_TransmitComplete();
  }
  else if(LL_DMA_IsActiveFlag_TC5(DMAx))
  {
    LL_DMA_ClearFlag_TC5(DMAx);
    DMA1_RecieveComplete();
  }
  else if(LL_DMA_IsActiveFlag_TE4(DMAx) || LL_DMA_IsActiveFlag_TE5(DMAx))
  {
    DMA1_Errors();
  }
}
