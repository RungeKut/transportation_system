/**
 ******************************************************************************
 * @file    MC_SixStep_param.h
 * @author  System lab - Automation and Motion control team
 * @version V1.1.0
 * @date    23-Oct-2015
 * @brief   This header file provides all parameters to driver a motor with 6Step
            library
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

/** @addtogroup MIDDLEWARES     MIDDLEWARES 
  * @brief  Middlewares Layer
  * @{ 
*/

/** @addtogroup MC_6-STEP_LIB       MC_6-STEP_LIB 
  * @brief  Motor Control driver
  * @{ 
*/

/** @defgroup Main_Motor_parameters    Main_Motor_parameters
  *  @{
    * @brief All motor parameters for 6Step driving
*/
 
/* **************************************************************************** 
 ==============================================================================   
                       ###### BASIC PARAMETERS ######
 ============================================================================== 
**************************************************************************** */   
#define NUM_POLE_PAIRS                       12      /*!< Number of Motor Pole pairs */
#define DIRECTION                            0      /*!< Set motor direction CW = 0 and CCW = 1*/
#define TARGET_SPEED                      3000      /*!< Target speed in closed loop control when the potentiometer is disabled */ 
#define POTENTIOMETER                        1      /*!< Enable (1)/Disable (0) the potentiometer */ 
#define VOLTAGE_MODE                         1     /*!< (1) for VOLTAGE mode / (0) for CURRENT mode control */
#define CURRENT_LIMIT			0x0FFF
#define TEMPERATURE_LIMIT		     0
/* **************************************************************************** 
 ==============================================================================   
                       ###### ADVANCED PARAMETERS ######
 ============================================================================== 
**************************************************************************** */  
#define SYSCLOCK_FREQUENCY            84000000     /*!< Hz */


/*!< ********************* Hall sensors **************************************/
#define START_COUNTER_STEPS_DECREMENTATION   8
#define NUMBER_OF_STARTS                     3
#define HALL_KO_SUCCESSIVE_MAX              60
#define MAX_SPEED                         3000      /*!< Motor rated max speed */
#define MIN_SPEED							30
#define COMMUTATION_DELAY                   20
#define STEP_DURATION_MAXIMUM				((SYSCLOCK_FREQUENCY*(uint32_t)10/((LF_TIMx.Instance->PSC+1)*NUM_POLE_PAIRS*MIN_SPEED))<<1)
#define STEP_DURATION_MINIMUM 				((SYSCLOCK_FREQUENCY*(uint32_t)10/((LF_TIMx.Instance->PSC+1)*NUM_POLE_PAIRS*MAX_SPEED))>>1)

/*!< ********************* Open loop control *********************************/
#define STARTUP_CURRENT_REFERENCE        1000      /*!< StartUP Currente Reference */
#define ACC                              10 /*10000*/      /*!< Mechanical acceleration rate */
#define MINIMUM_ACC                       1 /*500*/      /*!< Mechanical acceleration rate for BIG load application */
#define NUMBER_OF_STEPS                  20000      /*!< Number of elements for motor start-UP (max value 65535)*/ 
#define TIME_FOR_ALIGN                     100      /*!< Time for alignment (msec)*/ 
#define BUTTON_DELAY                      1000      /*!< Delay time to enable push button for new command (1 = 1msec)*/ 
#define NUMBER_ZCR                          12      /*!< Number of zero crossing event during the startup for closed loop control begin */   

/*!< ********************* Closed Loop control - CURRENT MODE *********************************/
#define SPEED_LOOP_TIME                      1      /*!< Speed Loop time (1 = 1msec) */  
#define KP_GAIN                           6000      /*!< Kp parameter for PI regulator */
#define KI_GAIN                            150      /*!< Ki parameter for PI regulator */   
#define KP_DIV                            8192      /*!< Kp parameter divider for PI regulator */
#define KI_DIV                            8192      /*!< Ki parameter divider for PI regulator */   
#define LOWER_OUT_LIMIT                    100      /*!< Low Out value of PI regulator */      
#define UPPER_OUT_LIMIT                  16384      /*!< High Out value of PI regulator */
#define MAX_POT_SPEED                     3000      /*!< Maximum Speed regulated by potentiometer */
#define MIN_POT_SPEED                     10      /*!< Minimum Speed regulated by potentiometer */
#define VAL_POT_SPEED_DIV                    2      /*!< Validation potentiometer speed divider */
#define INITIAL_DEMAGN_DELAY                 2      /*!< Initial value for delay time during startup for Bemf detection */

/*!< ********************* Closed Loop control - VOLTAGE MODE *********************************/
#define KP_GAIN_VM                          100     /*!< Kp parameter for PI regulator */
#define KI_GAIN_VM                           30     /*!< Ki parameter for PI regulator */   
#define KP_DIV_VM                          8192     /*!< Kp parameter divider for PI regulator */
#define KI_DIV_VM                          8192     /*!< Ki parameter divider for PI regulator */   
#define LOWER_OUT_LIMIT_VM                  400     /*!< Low Out value of PI regulator */      
#define UPPER_OUT_LIMIT_VM                 1799     /*!< High Out value of PI regulator */   
#define DUTY_CYCLE_INIT_VALUE               10     /*!< Initial duty cycle value during startup */

/*!< *********** Syncronous rectification for high frequency leg ****************/
#define SYNCHRONOUS_RECTIFICATION           1       /*!< Enable (1) or Disable (0) the complementary output of high frequency leg for synchronous rectification */

/*!< Zero Crossissing parameters */
#define BEMF_THRSLD_DOWN                    80      /*!< Zero Crossing threshold */  
#define BEMF_THRSLD_UP                      80      /*!< Zero Crossing threshold */ 

/*!< Speed filtering parameters */
#define FILTER_DEEP                         20      /*!< Number of bits for digital filter */  
#define HFBUFFERSIZE                        10
#define ADC_SPEED_TH                        82      /*!<Fixed treshold to change the target speed (t.b.f) */ 

/*!< Motor stall detection parameters */
#define BEMF_CONSEC_DOWN_MAX                10      /*!< Maximum value of BEMF Consecutive Threshold Falling Crossings Counter in closed loop */       
#define BEMF_CNT_EVENT_MAX                 100      /*!< Maximum number of BEMF Counter in open loop*/       

/*!< Debug pin */
#define GPIO_ZERO_CROSS                      1      /*!< Enable (1) the GPIO toggling for zero crossing detection */  
#define GPIO_COMM                            1      /*!< Enable (1) the GPIO toggling for commutation */  


/*!< Demo mode parameters */
#define DEMO_START_TIME                   5000      /*!< Time (msec) to keep the motor in run mode */  
#define DEMO_STOP_TIME                    2000      /*!< Time (msec) to keep the motor in stop mode */  

#define TRUE                                 1      /*!< Define TRUE */  
#define FALSE                                0      /*!< Define FALSE */

/**
  * @}   Main_Motor_parameters
  */


/**
  * @}  end MC_6-STEP_LIB 
  */

/**
  * @}  end MIDDLEWARES
  */
