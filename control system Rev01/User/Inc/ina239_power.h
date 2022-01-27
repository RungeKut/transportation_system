#ifndef INA239_POWER_H
#define INA239_POWER_H

#include "main.h"

extern volatile uint8_t spi_rx_buf[3];
extern volatile float RemainBatCapacity_mWh;
extern volatile float CargeBatCapacity_mWh;

/* BEGIN Prototypes */
void ina239_StartUp(void);
void INA_CS_0(void);
void INA_CS_1(void);
uint32_t ReadReg_INA(uint8_t, uint8_t);
void WriteReg_INA(uint8_t, uint8_t, uint32_t);
void INA_SysRESET(void);
void INA_CONVDLY(uint16_t);
void INA_ADCRANGE(uint8_t);
void INA_MODE(uint8_t);
void INA_VBUSCT(uint8_t);
void INA_VSHCT(uint8_t);
void INA_VTCT(uint8_t);
void INA_AVG(uint8_t);
void INA_CURRLSB(uint16_t, uint16_t);
void BatPercentCalc(void);
void INA_INIT(void);
/* END Prototypes */

#endif /* INA239_POWER */
