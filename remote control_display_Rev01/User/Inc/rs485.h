#ifndef RS485_H
#define RS485_H

#include "main.h"

/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA1_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA1_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA1_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA1_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))
#define DMA1_Channel6_IT_Mask    ((uint32_t)(DMA_ISR_GIF6 | DMA_ISR_TCIF6 | DMA_ISR_HTIF6 | DMA_ISR_TEIF6))
#define DMA1_Channel7_IT_Mask    ((uint32_t)(DMA_ISR_GIF7 | DMA_ISR_TCIF7 | DMA_ISR_HTIF7 | DMA_ISR_TEIF7))

/* DMA2 Channelx interrupt pending bit masks */
#define DMA2_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA2_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA2_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA2_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA2_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))

#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF8001)

#define DMAEnable ((uint16_t)(1<<0))
#define DMADisable ((uint16_t)0)

#define TransCompl_Int_Enable ((uint16_t)(1<<1))
#define TransCompl_Int_Disable ((uint16_t)0)

#define HalfCompl_Int_Enable ((uint16_t)1<<2)
#define HalfCompl_Int_Disable ((uint16_t)0)

#define TransError_Int_Enable ((uint16_t)(1<<3))
#define TransError_Int_Disable ((uint16_t)0)

#define ReadMemory ((uint16_t)(1<<4))
#define ReadPerif ((uint16_t)0)

#define CircularMode_Enable ((uint16_t)1<<5)
#define CircularMode_Disable ((uint16_t)0)


#define PeripheralInc_Enable ((uint16_t)(1<<6))
#define PeripheralInc_Disable ((uint16_t)0)

#define MemoryInc_Enable ((uint16_t)(1<<7))
#define MemoryInc_Disable ((uint16_t)0)

#define PDataSize_B ((uint16_t)0)
#define PDataSize_W ((uint16_t)0x0100)
#define PDataSize_DW ((uint16_t)0x0200)

#define MDataSize_B ((uint16_t)0)
#define MDataSize_W ((uint16_t)0x0400)
#define MDataSize_DW ((uint16_t)0x0800)

#define DMA_Priority_Low ((uint16_t)0)
#define DMA_Priority_Med ((uint16_t)(1<<12))
#define DMA_Priority_Hi  ((uint16_t)(2<<12))
#define DMA_Priority_VHi ((uint16_t)(3<<12))

#define M2M_Enable (1<<14)
#define M2M_Disable 0

/**********Begin**********RS485_Status********************/
/*typedef enum
{
  MENU         = 0x01, //На экране меню
  PREPERING    = 0x03, //На экране подготовка
  STOP         = 0x05, //На экране Авария
  LOW_BAT      = 0x07, //На экране Низкий заряд сообщение
  CLEAR        = 0xFF  //Дисплей должен быть перерисован
} RS485_StatusTypeDef;
extern volatile RS485_StatusTypeDef RS485_Status;*/
/**********End************RS485_Status********************/

/* BEGIN Prototypes */
void DMA_Init(DMA_Channel_TypeDef* Channel, uint32_t Perif, uint32_t Mem, uint16_t Size, uint16_t Conf);
void DMA_Enable(DMA_Channel_TypeDef* Channel);
void DMA_Disable(DMA_Channel_TypeDef* Channel);

uint32_t DMA_GetCurrentDataCounter(DMA_Channel_TypeDef* Channel);
void DMA_DeInit(DMA_Channel_TypeDef* Channel);

void rs485_StartUp(void);
void ClearButtonGlobalFlagRX(void);
void ClearButtonGlobalFlagTX(void);
void DMA1_RX_ClearFlag(void);
void DMA1_TX_ClearFlag(void);
void DMA1_RX_Start(void);
void DMA1_RX_InitStart(void);
void DMA1_TX_Start(void);
void DMA1_TX_InitStart(void);
void DMA1_RX_Stop(void);
void DMA1_TX_Stop(void);
void USART1_IRQEnable(void);
void CorrectRX(void);
void CorrectTX(void);
void DMA1_RecieveComplete(void);
void DMA1_TransmitComplete(void);
void DMA1_Errors(void);
void DMA_IRQHandler_Callback(DMA_TypeDef*);
/* END Prototypes */

#endif /* RS485_H */
