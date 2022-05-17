/**
 ******************************************************************************
 * @file    X-NUCLEO-IHM08M1.c
 * @author  System lab - Automation and Motion control team
 * @version V1.1.0
 * @date    23-Oct-2015
 * @brief   This file provides the set of functions to manage the X-Nucleo board
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/** @addtogroup DRIVERS     DRIVERS 
  * @brief  Driver Layer
  * @{ 
  */

/** @addtogroup BSP     BSP 
  * @brief  BSP Layer
  * @{ 
  */

/** @addtogroup X-NUCLEO-IHM08M1    X-NUCLEO-IHM08M1
  * @brief  X-Nucleo board
  * @{ 
  */


//#define COMPL_MODE


/* Includes ------------------------------------------------------------------*/
#include "X-NUCLEO-IHM08M1.h"
#include "stm32f4xx_hal.h"
#include "stm32F401_nucleo_ihm08m1.h"
       
#include "6Step_Lib_Hall.h"

/** @defgroup L6398_ECH1CH2_DCH3_IO_Write    L6398_ECH1CH2_DCH3_IO_Write
  *  @{
    * @brief Enable Input channel CH1 and CH2 for L6398   
  * @retval None
*/

/*
void L6398_ECH1CH2_DCH3_IO_Write(uint8_t step_position,uint8_t sync_rect, uint16_t CCR_value)
{
  uint32_t tmpccmrx = 0;  
  
  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
//  tmpccmrx |= TIM_OCMODE_PWM1;
  if(step_position == 1) {
	  tmpccmrx |= TIM_OCMODE_PWM1;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2;
  }
  HF_TIMx.Instance->CCMR1 = tmpccmrx;
  HF_TIMx.Instance->CCER |= TIM_CCER_CC1E;   //enable
  if(sync_rect != 1)
  {
    if(step_position != 1)
    {
      HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable
    }
  }
  else HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable

 // if(step_position == 1)
    L6398_HFTIM_DC_CH1(HF_TIMx.Init.Period / 2 - CCR_value);

  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
//  tmpccmrx |= TIM_OCMODE_PWM1<<8;
  if(step_position == 4) {
	  tmpccmrx |= TIM_OCMODE_PWM1<<8;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2<<8;
  }
  HF_TIMx.Instance->CCMR1 = tmpccmrx;
  HF_TIMx.Instance->CCER |= TIM_CCER_CC2E;   //enable
  if(sync_rect != 1)
  {
  if(step_position != 4)
    {
     HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable

    }
  }
  else   HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable
//  if(step_position == 4)
     L6398_HFTIM_DC_CH2(HF_TIMx.Init.Period / 2 - CCR_value);
//  else
//    L6398_HFTIM_DC_CH2(0);

  tmpccmrx = HF_TIMx.Instance->CCMR2;
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
  tmpccmrx |= TIM_OCMODE_FORCED_INACTIVE;
  HF_TIMx.Instance->CCMR2 = tmpccmrx;
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable
  L6398_HFTIM_DC_CH3(0);
}

*/

void L6398_ECH1CH2_DCH3_IO_Write(uint8_t step_position,uint8_t sync_rect, uint16_t CCR_value)
{
  uint32_t tmpccmrx = 0;

  tmpccmrx = HF_TIMx.Instance->CCMR2;
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;

  tmpccmrx |= TIM_OCMODE_FORCED_INACTIVE;
  HF_TIMx.Instance->CCMR2 = tmpccmrx;
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable
  L6398_HFTIM_DC_CH3(0);

//	HF_TIMx.Instance->EGR = TIM_EVENTSOURCE_COM;

  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;

#ifdef COMPL_MODE
  if(step_position == 1) {
	  tmpccmrx |= TIM_OCMODE_PWM1;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2;
  }
#else
  tmpccmrx |= TIM_OCMODE_PWM1;  
#endif
  HF_TIMx.Instance->CCMR1 = tmpccmrx;    
//  HF_TIMx.Instance->CCER |= TIM_CCER_CC1E;   //enable
  if(sync_rect != 1)
  {
    if(step_position != 1)
    { 
      HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable  
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable
    }
  }
//  else HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable
  
#ifdef COMPL_MODE
  L6398_HFTIM_DC_CH1(HF_TIMx.Init.Period / 2 - CCR_value);
#else
  if(step_position == 1)
    L6398_HFTIM_DC_CH1(CCR_value);
  else 
    L6398_HFTIM_DC_CH1(0); 
#endif
  
  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;

#ifdef COMPL_MODE
  if(step_position == 4) {
	  tmpccmrx |= TIM_OCMODE_PWM1<<8;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2<<8;
  }
#else
  tmpccmrx |= TIM_OCMODE_PWM1<<8;  
#endif
  HF_TIMx.Instance->CCMR1 = tmpccmrx;    
//  HF_TIMx.Instance->CCER |= TIM_CCER_CC2E;   //enable
  if(sync_rect != 1)
  {
  if(step_position != 4)
    {  
     HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable

    }
  }
//  else   HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable

#ifdef COMPL_MODE
  L6398_HFTIM_DC_CH2(HF_TIMx.Init.Period / 2 - CCR_value);
#else
  if(step_position == 4)
     L6398_HFTIM_DC_CH2(CCR_value);  
  else  
    L6398_HFTIM_DC_CH2(0);   
#endif

  
  HF_TIMx.Instance->CCER |= TIM_CCER_CC1E;   //enable
  HF_TIMx.Instance->CCER |= TIM_CCER_CC2E;   //enable
  if(sync_rect) {
	  HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable
	  HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable
	}

}



/**
  * @}  
  */

/** @defgroup L6398_ECH1CH3_DCH2_IO_Write    L6398_ECH1CH3_DCH2_IO_Write
  *  @{
    * @brief Enable Input channel CH1 and CH3 for L6398   
  * @retval None
*/



void L6398_ECH1CH3_DCH2_IO_Write(uint8_t step_position,uint8_t sync_rect, uint16_t CCR_value)
{
  uint32_t tmpccmrx = 0;  
  
  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
  tmpccmrx |= TIM_OCMODE_FORCED_INACTIVE<<8;
  HF_TIMx.Instance->CCMR1 = tmpccmrx;
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable
  L6398_HFTIM_DC_CH2(0);

//	HF_TIMx.Instance->EGR = TIM_EVENTSOURCE_COM;

  tmpccmrx = HF_TIMx.Instance->CCMR2;
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
#ifdef COMPL_MODE
  if(step_position == 5) {
	  tmpccmrx |= TIM_OCMODE_PWM1;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2;
  }
#else
  tmpccmrx |= TIM_OCMODE_PWM1;  
#endif
  HF_TIMx.Instance->CCMR2 = tmpccmrx; 
//  HF_TIMx.Instance->CCER |= TIM_CCER_CC3E;   //enable
  
  if(sync_rect != 1)
  {
   if(step_position != 5)
    {        
     HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable

    }
  }   
//  else  HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable
  
#ifdef COMPL_MODE
  L6398_HFTIM_DC_CH3(HF_TIMx.Init.Period / 2 - CCR_value);
#else
  if(step_position == 5)
    L6398_HFTIM_DC_CH3(CCR_value);  
  else  
    L6398_HFTIM_DC_CH3(0);    
#endif
  
  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
#ifdef COMPL_MODE
  if(step_position == 2) {
	  tmpccmrx |= TIM_OCMODE_PWM1;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2;
  }
#else
  tmpccmrx |= TIM_OCMODE_PWM1;  
#endif
  HF_TIMx.Instance->CCMR1 = tmpccmrx;     
//  HF_TIMx.Instance->CCER |= TIM_CCER_CC1E;   //enable

  if(sync_rect != 1)
  {
    if(step_position != 2)
    {        
      HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable  
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable

    }
  }
//  else HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable

#ifdef COMPL_MODE
  L6398_HFTIM_DC_CH1(HF_TIMx.Init.Period / 2 - CCR_value);
#else
  if(step_position == 2)
    L6398_HFTIM_DC_CH1(CCR_value);  
  else  
    L6398_HFTIM_DC_CH1(0);    
#endif


  HF_TIMx.Instance->CCER |= TIM_CCER_CC1E;   //enable
  HF_TIMx.Instance->CCER |= TIM_CCER_CC3E;   //enable
  if(sync_rect) {
 	  HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable
 	  HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable
 	}

}



/**
  * @}  
  */
/** @defgroup L6398_ECH2CH3_DCH1_IO_Write    L6398_ECH2CH3_DCH1_IO_Write
  *  @{
    * @brief Enable Input channel CH2 and CH3 for L6398   
  * @retval None
*/
void L6398_ECH2CH3_DCH1_IO_Write(uint8_t step_position,uint8_t sync_rect, uint16_t CCR_value)
{
  uint32_t tmpccmrx = 0;   

  
  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  tmpccmrx |= TIM_OCMODE_FORCED_INACTIVE;
  HF_TIMx.Instance->CCMR1 = tmpccmrx;
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable
  L6398_HFTIM_DC_CH1(0);

//	HF_TIMx.Instance->EGR = TIM_EVENTSOURCE_COM;

  tmpccmrx = HF_TIMx.Instance->CCMR1;
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
#ifdef COMPL_MODE
  if(step_position == 3) {
	  tmpccmrx |= TIM_OCMODE_PWM1<<8;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2<<8;
  }
#else
  tmpccmrx |= TIM_OCMODE_PWM1<<8;  
#endif
  HF_TIMx.Instance->CCMR1 = tmpccmrx;    
  HF_TIMx.Instance->CCER |= TIM_CCER_CC2E;   //enable 
  if(sync_rect != 1)
  {
    if(step_position!= 3)
    {   
      HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable   
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable

    }
  }
//  else HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable

#ifdef COMPL_MODE
  L6398_HFTIM_DC_CH2(HF_TIMx.Init.Period / 2 - CCR_value);
#else
  if(step_position == 3)
    L6398_HFTIM_DC_CH2(CCR_value);  
  else  
    L6398_HFTIM_DC_CH2(0);    
#endif

  
  tmpccmrx = HF_TIMx.Instance->CCMR2;
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;
#ifdef COMPL_MODE
  if(step_position == 6) {
	  tmpccmrx |= TIM_OCMODE_PWM1;
  } else {
	  tmpccmrx |= TIM_OCMODE_PWM2;
  }
#else
  tmpccmrx |= TIM_OCMODE_PWM1;  
#endif
  HF_TIMx.Instance->CCMR2 = tmpccmrx;     
  HF_TIMx.Instance->CCER |= TIM_CCER_CC3E;   //enable 
  if(sync_rect != 1)
  {
    if(step_position != 6)
    {    
      HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable    
    } else {
    	  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable

    }
  }
//   else HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable

#ifdef COMPL_MODE
  L6398_HFTIM_DC_CH3(HF_TIMx.Init.Period / 2 - CCR_value);
#else
  if(step_position == 6)
    L6398_HFTIM_DC_CH3(CCR_value);  
  else  
    L6398_HFTIM_DC_CH3(0);    
#endif
  if(sync_rect) {
 	  HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable
 	  HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable
 	}
 }
/**
  * @}  
  */
/** @defgroup L6398_DCH1CH2CH3_IO_Write    L6398_DCH1CH2CH3_IO_Write
  *  @{
    * @brief Disable all channels for L6398   
  * @retval None
*/
void L6398_DCH1CH2CH3_IO_Write()
{
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable 
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable 
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable   
}
/**
  * @}  
  */
/** @defgroup L6398_Start_PWM_generation    L6398_Start_PWM_generation
  *  @{
    * @brief Enable the PWM generation on Input channels for L6398 
  * @retval None
*/

void L6398_Start_PWM_generation()
{
  HF_TIMx.Instance->CCER |= TIM_CCER_CC1E;   //enable 
  HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable
  HF_TIMx.Instance->CCER |= TIM_CCER_CC2E;   //enable 
  HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;   //enable
  HF_TIMx.Instance->CCER |= TIM_CCER_CC3E;   //enable 
  HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;   //enable


//  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable
//  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable
//  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable
}
/**
  * @}  
  */
/** @defgroup L6398_Stop_PWM_generation    L6398_Stop_PWM_generation
  *  @{
    * @brief Disable the PWM generation on Input channels for L6398 
  * @retval None
*/
void L6398_Stop_PWM_generation()
{
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable 
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable 
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3E;  //disable
  HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable         
}
/**
  * @}  
  */
/** @defgroup L6398_HFTIM_DC_CH1    L6398_HFTIM_DC_CH1
  *  @{
    * @brief Set the Duty Cycle value for CH1  
  * @retval None
*/
void L6398_HFTIM_DC_CH1(uint16_t CCRx)
{

  HF_TIMx.Instance->HF_TIMx_CCR1 = CCRx;  
}
/**
  * @}  
  */
/** @defgroup L6398_HFTIM_DC_CH2    L6398_HFTIM_DC_CH2
  *  @{
    * @brief Set the Duty Cycle value for CH2
  * @retval None
*/
void  L6398_HFTIM_DC_CH2(uint16_t CCRx)
{
  HF_TIMx.Instance->HF_TIMx_CCR2 = CCRx;  
}
/**
  * @}  
  */
/** @defgroup L6398_HFTIM_DC_CH3    L6398_HFTIM_DC_CH3
  *  @{
    * @brief Set the Duty Cycle value for CH3  
  * @retval None
*/
void  L6398_HFTIM_DC_CH3(uint16_t CCRx)
{ 
  HF_TIMx.Instance->HF_TIMx_CCR3 = CCRx;  
}
/**
  * @}  
  */

/** @defgroup BSP_X_NUCLEO_FAULT_LED_ON    BSP_X_NUCLEO_FAULT_LED_ON
  *  @{
    * @brief Turns selected LED On.
  * @retval None
*/
void BSP_X_NUCLEO_FAULT_LED_ON()
{
  HAL_GPIO_WritePin(GPIOB,5,GPIO_PIN_SET);
}
/**
  * @}  
  */
/** @defgroup BSP_X_NUCLEO_FAULT_LED_OFF    BSP_X_NUCLEO_FAULT_LED_OFF
  *  @{
    * @brief Turns selected LED Off.
  * @retval None
*/
void BSP_X_NUCLEO_FAULT_LED_OFF()
{
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
}
/**
  * @}  
  */

/**
  * @}  end X-NUCLEO-IHM08M1 
  */

/**
  * @}  end BSP
  */

/**
  * @}  end DRIVERS
  */
