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
#include <string.h>
#include "main.h"
#include "bitbanding.h"
#include "stm32f4xx_hal.h"
#include "mb.h"
#include "mbport.h"
#include "proto.h"
#include "6Step_Lib_Hall.h"
#include "swtimer.h"
#include "adctenso.h"
#include "ramp.h"
#include "eprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TELEMETRY_UPDATE_Msk	(0)

//Driver State Masks
#define DRIVER_STATE_MODE_Msk		(0xE000)
#define DRIVER_STATE_MODE_Pos		(13)
#define DRIVER_STATE_DIR_Msk		(0x1000)
#define DRIVER_STATE_DIR_Pos		(12)
#define DRIVER_STATE_PULSE_Msk		(0x0FFF)
#define DRIVER_STATE_PULSE_Pos		(0)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Driver_ConfigTypeDef DriverConfig;
Driver_GroupStateTypeDef DriverGroupState;
Driver_TelemetryTypeDef DriverTelemetry;
static uint8_t proto_expire_timer;
static uint8_t proto_wait_short_coil_timer;
static uint32_t proto_flags = 0;
extern uint8_t ModBusAddr;
extern EPROM_Driver_ConfigTypeDef EPROM_Data;
static bool connected = false;

enum Commands {NotInitialized, StartInit, CompleteInit, ResetAlarm};
static int init_flag = NotInitialized;
/* Private function prototypes -----------------------------------------------*/
static void UpdateDriverState(void);
static void UpdateDriverConfig(void);
static uint32_t GetDriverTelemetry(Driver_TelemetryTypeDef *telemetry);

/* Private functions ---------------------------------------------------------*/

static void UpdateDriverState(void)
{
	uint8_t indx;
	uint16_t drv_state;
	uint16_t drv_mode;
	uint16_t drv_dir;
	uint16_t drv_pulse;

	if (ModBusAddr >= DriverGroupState.GroupBaseAddress &&
		DriverGroupState.GroupBaseAddress + DriverGroupState.DriveNumber > ModBusAddr)
	{
		if (!connected) {
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			connected = true;
		}
		if (SwTimer_GetCompare(proto_expire_timer) != 0)
		{
			SwTimer_SetCounter(proto_expire_timer, 0);
			SwTimer_SetMode(proto_expire_timer, SWTIMER_MODE_SINGLE);
		}
		GetDriverTelemetry(&DriverTelemetry);
		BIT_BAND_SRAM(proto_flags, TELEMETRY_UPDATE_Msk) = 1;
		if (MC_IsPotentiometerActive())
		{
			MC_EnablePotentiometer(0);
		}
//		indx = DriverGroupState.GroupBaseAddress + DriverGroupState.DriveNumber - ModBusAddr -1;
		indx = ModBusAddr - 1;
		drv_state = DriverGroupState.State[indx];
		drv_mode = (drv_state & DRIVER_STATE_MODE_Msk) >> DRIVER_STATE_MODE_Pos;
		drv_dir = (drv_state & DRIVER_STATE_DIR_Msk) >> DRIVER_STATE_DIR_Pos;
		drv_pulse = (drv_state & DRIVER_STATE_PULSE_Msk) >> DRIVER_STATE_PULSE_Pos;
		switch  (drv_mode)
		{
		case 0:
			if (!MC_IsMotorRun())
			{
				MC_StartMotor();
				//RampMotorSetFlag(RAMP_START_STOP_Pos);
			}
			if (drv_dir != MC_GetDirection() && MC_GetDirection() == MC_GetTargetDirection())
			{
				MC_SetTargetDirection(drv_dir);
			}
			MC_SetSpeedTarget((uint16_t)(((uint32_t)drv_pulse * HF_TIMx.Instance->ARR)>>12));
			break;
		case 1:
			MC_EnableBrake();
			break;
		case 2:
			if (!MC_IsMotorRun())
			{
				RampMotorSetFlag(RAMP_START_STOP_Pos);
				SwTimer_SetCounter(proto_wait_short_coil_timer, 0);
				SwTimer_SetMode(proto_wait_short_coil_timer, SWTIMER_MODE_SINGLE);
			}
			else
			{
				MC_ShortCoils();
			}
			break;
		case 4:
			MC_StopMotor();
			break;
		}
	}
	return;
}
/*----------------------------------------------------------------------------*/
static void UpdateDriverConfig(void)
{
	if (DriverConfig.ResetCounter)
	{
		MC_ResetCommCounter();
	}
	if (DriverConfig.Acceleration != MC_GetAcceleration())
	{
		MC_SetAcceleration(DriverConfig.Acceleration);
	}
	if (DriverConfig.PoleNum != MC_GetPolePairs())
	{
		MC_SetPolePairs(DriverConfig.PoleNum);
	}
	if (DriverConfig.GroupCmdTimeOut != SwTimer_GetCompare(proto_expire_timer))
	{
		SwTimer_SetCompare(proto_expire_timer, DriverConfig.GroupCmdTimeOut);
		EPROM_Write(offsetof(EPROM_Driver_ConfigTypeDef, GroupCmdTimeOut), (uint8_t *)(&(DriverConfig.GroupCmdTimeOut)), sizeof(EPROM_Data.GroupCmdTimeOut));
	}
	if ((DriverConfig.FeedBack & 0x01) != (EPROM_Data.FeedBackMode & 0x01))
	{
		EPROM_Write(offsetof(EPROM_Driver_ConfigTypeDef, FeedBackMode), (uint8_t *)(&DriverConfig.FeedBack), sizeof(EPROM_Data.FeedBackMode));
	}
	if (DriverConfig.CurrentLimit  != (EPROM_Data.CurrentLimit & 0x0FFF))
	{
		MC_SetCurrentLimit(DriverConfig.CurrentLimit);
		EPROM_Write(offsetof(EPROM_Driver_ConfigTypeDef, CurrentLimit), (uint8_t *)(&DriverConfig.CurrentLimit), sizeof(EPROM_Data.CurrentLimit));
	}
	if (DriverConfig.TempearatureLimit  != (EPROM_Data.TempearatureLimit & 0x0FFF))
	{
		MC_SetTemperatureLimit(DriverConfig.TempearatureLimit);
		EPROM_Write(offsetof(EPROM_Driver_ConfigTypeDef, TempearatureLimit), (uint8_t *)(&DriverConfig.TempearatureLimit), sizeof(EPROM_Data.TempearatureLimit));
	}

	if (DriverConfig.Command == ResetAlarm) {
		DriverConfig.Command = 0;
		MC_ResetAllErrors();
	} else if (DriverConfig.Command == StartInit) {
		init_flag = StartInit;
	} else if (DriverConfig.Command == CompleteInit && init_flag == StartInit) {
		MC_ClearError(MC_NOT_INITIALIZED_Pos);
	}

	return;
}

static uint32_t GetDriverTelemetry(Driver_TelemetryTypeDef *telemetry)
{
	uint16_t *buff = NULL;
	uint32_t buff_sz = 0;

	GetTensoADCBuff(&buff, &buff_sz);
	if (buff_sz <= sizeof(telemetry->ADCCode))
	{
		memcpy(telemetry->ADCCode, buff, buff_sz);
	}

//	for (int i =0; i < 4; ++i) {
//		telemetry->ADCCode[i] = MC_GetTemperatureLimit();//DriverConfig.TempearatureLimit;
//	}

	telemetry->HallCounter = (uint16_t)(MC_GetCommCounter());
	telemetry->RevCounter = (uint16_t)(MC_GetMechSpeedRPMValue());
	telemetry->ErrorStatus = MC_GetError();
	return 0;
}
/*----------------------------------------------------------------------------*/
static void SwTimer_GrpCmdExpairedCB(void)
{
	connected = false;
	MC_SetError(MC_NOT_INITIALIZED_Pos);
	MC_StopMotor();
}
/*----------------------------------------------------------------------------*/
static void SwTimer_WaitShortCoilCB(void)
{
	if (IsRampMotorStopped())
	{
//		MC_ShortCoils();
	}
	else
	{
		SwTimer_SetCounter(proto_wait_short_coil_timer, 0);
		SwTimer_SetMode(proto_wait_short_coil_timer, SWTIMER_MODE_SINGLE);
	}
}
/* Exported functions --------------------------------------------------------*/

void ProtoInit(void)
{
	if (! EPROM_IsInitialized())
	{
		EPROM_Init();
	}
	if (EPROM_IsInitialized())
	{
		DriverConfig.Acceleration = EPROM_Data.Acceleration;
		DriverConfig.PoleNum = EPROM_Data.PoleNum;
		DriverConfig.GroupCmdTimeOut = EPROM_Data.GroupCmdTimeOut;
		DriverConfig.FeedBack = EPROM_Data.FeedBackMode & 0x01;
		DriverConfig.CurrentLimit = EPROM_Data.CurrentLimit & 0x0FFF;
		DriverConfig.TempearatureLimit = EPROM_Data.TempearatureLimit & 0x0FFF;
	}
	proto_expire_timer = SwTimer_Add(SWTIMER_MODE_STOP, GRP_CMD_EXPIRED_TIMER_TIMEOUT, SwTimer_GrpCmdExpairedCB);
	proto_wait_short_coil_timer = SwTimer_Add(SWTIMER_MODE_STOP, WAIT_SHORT_COIL_TIMER_TIMEOUT, SwTimer_WaitShortCoilCB);

	MC_SetError(MC_NOT_INITIALIZED_Pos);
}
/*----------------------------------------------------------------------------*/
/* MODBUS CallBacks */
eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs)
{
	eMBErrorCode eStatus = MB_ENOERR;
	uint16_t *pDriverInfo = NULL;
	int iRegIndex;

	usAddress -= 1;
	if ((usAddress >= DRIVER_TELEMETRY_MB_REG_ADDR) &&
			(usAddress + usNRegs <= DRIVER_TELEMETRY_MB_REG_ADDR + DRIVER_NREGS(Driver_TelemetryTypeDef)))
	{
		iRegIndex = (int) (usAddress - DRIVER_TELEMETRY_MB_REG_ADDR);
		pDriverInfo = (uint16_t *)&DriverTelemetry;
		if (BIT_BAND_SRAM(proto_flags, TELEMETRY_UPDATE_Msk) == 0)
		{
			GetDriverTelemetry(&DriverTelemetry);
		}
		else
		{
			BIT_BAND_SRAM(proto_flags, TELEMETRY_UPDATE_Msk) = 0;
		}
	}
	else if ((usAddress >= DRIVER_STATE_MB_REG_ADDR) &&
			(usAddress + usNRegs <= DRIVER_STATE_MB_REG_ADDR + DRIVER_NREGS(Driver_GroupStateTypeDef)))
	{
		iRegIndex = (int) (usAddress - DRIVER_STATE_MB_REG_ADDR);
		pDriverInfo = (uint16_t *)&DriverGroupState;
	}
	else if ((usAddress >= DRIVER_CONFIG_MB_REG_ADDR) &&
			(usAddress + usNRegs <= DRIVER_CONFIG_MB_REG_ADDR + DRIVER_NREGS(Driver_ConfigTypeDef)))
	{
		iRegIndex = (int) (usAddress - DRIVER_CONFIG_MB_REG_ADDR);
		pDriverInfo = (uint16_t *)&DriverConfig;
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	if ((eStatus == MB_ENOERR) && (pDriverInfo != NULL))
	{
		while (usNRegs > 0)
		{
			*pucRegBuffer++ = (unsigned char) (pDriverInfo[iRegIndex] >> 8);
			*pucRegBuffer++ = (unsigned char) (pDriverInfo[iRegIndex] & 0xFF);
			iRegIndex++;
			usNRegs--;
		}
	}
	return eStatus;
}
/*----------------------------------------------------------------------------*/
eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode)
{
	eMBErrorCode eStatus = MB_ENOERR;
	int iRegIndex;
	uint16_t *pDriverInfo = NULL;

	usAddress -= 1;
	if ((usAddress >= DRIVER_STATE_MB_REG_ADDR) &&
			(usAddress + usNRegs <= DRIVER_STATE_MB_REG_ADDR + DRIVER_NREGS(Driver_GroupStateTypeDef)))
	{
		iRegIndex = (int) (usAddress - DRIVER_STATE_MB_REG_ADDR);
		pDriverInfo = (uint16_t *)&DriverGroupState;
	}
	else if ((usAddress >= DRIVER_CONFIG_MB_REG_ADDR) &&
			(usAddress + usNRegs <= DRIVER_CONFIG_MB_REG_ADDR + DRIVER_NREGS(Driver_ConfigTypeDef)))
	{
		iRegIndex = (int) (usAddress - DRIVER_CONFIG_MB_REG_ADDR);
		pDriverInfo = (uint16_t *)&DriverConfig;
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	if ((eStatus == MB_ENOERR) && (pDriverInfo != NULL))
	{
		switch (eMode)
		{
		case MB_REG_READ:
			while (usNRegs > 0)
			{
				*pucRegBuffer++ = (unsigned char) (pDriverInfo[iRegIndex] >> 8);
				*pucRegBuffer++ = (unsigned char) (pDriverInfo[iRegIndex] & 0xFF);
				iRegIndex++;
				usNRegs--;
			}
			break;

		case MB_REG_WRITE:
			while (usNRegs > 0)
			{
				pDriverInfo[iRegIndex] = *pucRegBuffer++ << 8;
				pDriverInfo[iRegIndex] |= *pucRegBuffer++;
				iRegIndex++;
				usNRegs--;
			}
			if (pDriverInfo == (uint16_t *)&DriverGroupState)
			{
				UpdateDriverState();
			}
			else if (pDriverInfo == (uint16_t *)&DriverConfig)
			{
				UpdateDriverConfig();
			}
		}
	}

	return eStatus;
}
/*----------------------------------------------------------------------------*/

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
	return MB_ENOREG;
}

/*----------------------------------------------------------------------------*/
eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
	return MB_ENOREG;
}

/*----------------------------------------------------------------------------*/

bool isMbConnected() {
	return connected;
}

/*----------------------------------------------------------------------------*/
