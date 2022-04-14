#include "ina239_power.h"
#include "main.h"
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
#define SPI_READ 0x1							//																				 ┃
#define SPI_WRITE 0x0							//																				 ┃
// INA239 Registers	Address																									 ┃
#define INA_CONFIG					0x00	//Configuration 16bit											 ┃
#define INA_ADC_CONFIG			0x01	//ADC_CONFIG ADC Configuration 16bit			 ┃
#define INA_SHUNT_CAL				0x02	//Shunt Calibration 16bit									 ┃
#define INA_VSHUNT					0x04	//Shunt Voltage Measurement 16bit					 ┃
#define INA_VBUS						0x05	//Bus Voltage Measurement 16bit						 ┃
#define INA_DIETEMP		      0x06  //Temperature Measurement 16bit            ┃
#define INA_CURRENT         0x07  //Current Result 16bit                     ┃
#define INA_POWER           0x08  //Power Result 24bit                       ┃
#define INA_DIAG_ALRT       0x0B  //Diagnostic Flags and Alert 16bit         ┃
#define INA_SOVL            0x0C  //Shunt Overvoltage Threshold 16bit        ┃
#define INA_SUVL            0x0D  //Shunt Undervoltage Threshold 16bit       ┃
#define INA_BOVL            0x0E  //Bus Overvoltage Threshold 16bit          ┃
#define INA_BUVL            0x0F  //Bus Undervoltage Threshold 16bit         ┃
#define INA_TEMP_LIMIT      0x10  //Temperature Over-Limit Threshold 16bit   ┃
#define INA_PWR_LIMIT       0x11  //Power Over-Limit Threshold 16bit         ┃
#define INA_MANUFACTURER_ID 0x3E  //Manufacturer ID 16bit                    ┃
#define INA_DEVICE_ID       0x3F  //Device ID 16bit                          ┃
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#define BatCapacity_mWh 200000
#define INA_CURRENT_LSB_nA 305175 // Imax,мА / 2^15 x 10^6
const uint32_t SHUNT_CAL = 4096;   //819.2 x 10^6 x INA_CURRENT_LSB_nA x Rшунта_мкОм / 10^15 (4167)
static const uint8_t INA_REG[17][2] = {
{ INA_CONFIG,           16 },
{ INA_ADC_CONFIG,       16 },
{ INA_SHUNT_CAL,        16 },
{ INA_VSHUNT,           16 },
{ INA_VBUS,             16 },
{ INA_DIETEMP,          16 },
{ INA_CURRENT,          16 },
{ INA_POWER,            24 },
{ INA_DIAG_ALRT,        16 },
{ INA_SOVL,             16 },
{ INA_SUVL,             16 },
{ INA_BOVL,             16 },
{ INA_BUVL,             16 },
{ INA_TEMP_LIMIT,       16 },
{ INA_PWR_LIMIT,        16 },
{ INA_MANUFACTURER_ID,  16 },
{ INA_DEVICE_ID,        16 }};
uint32_t INA_REG_Readed[4];
float INA_Voltage_mV;
float INA_Current_mA;
float INA_Current_mA_temp;
float INA_Power_mW;
//uint16_t INA_Temperature_0C;
volatile uint8_t spi_rx_buf[3];
volatile float RemainBatCapacity_mWh = 0;
//volatile float CargeBatCapacity_mWh = 0;
uint32_t Percent_temp = 0;
uint8_t Charge_flag = 0;
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Старт                                                                      ┃
void ina239_StartUp(void)//                                                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{  
//  for (uint8_t i = 0; i < 17; i++)
  for (uint8_t i = 4; i < 8; i++)
  {
    INA_REG_Readed[i - 4] = ReadReg_INA(INA_REG[i][0], INA_REG[i][1]);
  }
  INA_Voltage_mV = ((int16_t)(INA_REG_Readed[0])) * 3.125;
  INA_Current_mA = ((int16_t)(INA_REG_Readed[2])) * 0.3;
  
  INA_Power_mW   = INA_REG_Readed[3] * 0.06;
//  INA_Temperature_0C = INA_REG_Readed[1];
//==============================================================================
//Расчитываем емкость исходя из времени конверсии АЦП и коэф. усреднения 4120x1024x2/10^6/3600
  if (INA_Current_mA > 0)
  {
    if (Battery_Status < BAT_ERR_MISSING)
      Battery_Status = BAT_OK;
    if (RemainBatCapacity_mWh < 10.0*BatCapacity_mWh)
    {
      RemainBatCapacity_mWh -= INA_Power_mW * 0.002343822; //При разряде оставшаяся емкость будет уменьшаться
    }
    if (GLOBAL_FLASH_FLAG & EnableWrite_FLASH)
    {
      flashPage_write32(FloatToUint(RemainBatCapacity_mWh), 1024);
    }
  }
  else if (INA_Current_mA < (float)(-100)) //Если ток заряда меньше 100мА то считаем зарядку оконченой
  {
    if (Battery_Status < BAT_ERR_MISSING)
      Battery_Status = CHARGING;
    if (RemainBatCapacity_mWh < 10.0*BatCapacity_mWh)
    {
      RemainBatCapacity_mWh += INA_Power_mW * 0.002343822; //При заряде оставшаяся емкость будет увеличиваться
    }
    if (GLOBAL_FLASH_FLAG & EnableWrite_FLASH)
    {
      flashPage_write32(FloatToUint(RemainBatCapacity_mWh), 1024);
    }
  }
  else // Если ток аккумулятора стал равным нулю, то зарядка завершена, и устройство питается от зарядника
  {
    if ((GLOBAL_FLASH_FLAG & EnableWrite_FLASH) &&
        (Battery_Status != END_CHARGE) &&
        (INA_Current_mA_temp < 0))
    {
    
      RemainBatCapacity_mWh = (float)BatCapacity_mWh;
      flashPage_write32(FloatToUint(RemainBatCapacity_mWh), 1024);
      Battery_Status = END_CHARGE;
    }
  }
  BatPercentCalc();
  INA_Current_mA_temp = INA_Current_mA;
//==============================================================================
/*  uint32_t K = 100000/(BATTERY_MAX_V-BATTERY_MIN_V);
  uint32_t B = 100000*BATTERY_MIN_V/(BATTERY_MAX_V-BATTERY_MIN_V);
  uint32_t P = (K * INA_Voltage_mV/100 - B) / 1000;
  if (P <= 100)
  {
    battery = P;
  }
  else if (INA_Voltage_mV / 100 > BATTERY_MAX_V)
  {
    battery = 100;
  }
  else if (INA_Voltage_mV / 100 < BATTERY_MIN_V)
  {
    battery = 1;
  }
  */
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Посчтитаем проценты                                                        ┃
void BatPercentCalc(void)//                                                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if ((RemainBatCapacity_mWh < 1.0*BatCapacity_mWh) && (RemainBatCapacity_mWh > 0))
  {
    Percent_temp = 1 + RemainBatCapacity_mWh * (100.0 / BatCapacity_mWh);
    if ((Percent_temp >= 1) && (Percent_temp <= 100))
    {
      battery = Percent_temp;
    }
  }
  else if (RemainBatCapacity_mWh >= 1.0*BatCapacity_mWh)
  {
    battery = 100;
  }
  else
  {
    battery = 1;
  }
}
//==============================================================================

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Установка низкого уровня ChipSelect                                        ┃
void INA_CS_0(void)//                                                        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  LL_GPIO_ResetOutputPin(INA_CS_GPIO_Port, INA_CS_Pin);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Установка высокого уровня ChipSelect                                       ┃
void INA_CS_1(void)//                                                        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  LL_GPIO_SetOutputPin(INA_CS_GPIO_Port, INA_CS_Pin);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// Функция, реализующая чтение указанного регистра                           ┃
// (используется программное управление передачей).                          ┃
uint32_t ReadReg_INA(uint8_t tx_cmd, uint8_t l_cmd)//                        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if ((l_cmd == 0) || (l_cmd > 32))
  {
    return 0;
  }
  irq_disable();
  uint8_t cmd = (tx_cmd << 2) | SPI_READ;
  uint8_t tx_data[4];
  uint32_t data = 0;
  INA_CS_0();
//  SPI1->CR1 &= ~SPI_CR1_CPHA;
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
//  SPI1->CR1 |= SPI_CR1_CPHA;
  HAL_SPI_Receive(&hspi1, (uint8_t*)&tx_data, l_cmd / 8, 10);
  INA_CS_1();
  for (uint8_t i = 0; i < l_cmd / 8; i++)
  {
    data += tx_data[(l_cmd / 8) - i - 1] << i * 8 ;
  }
  irq_enable();
  return data;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
// Функция, реализующая запись указанного регистра                           ┃
// (используется программное управление передачей).                          ┃
void WriteReg_INA(uint8_t tx_cmd, uint8_t l_cmd, uint32_t data)//            ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  if ((l_cmd == 0) || (l_cmd > 32))
  {
    return;
  }
  irq_disable();
  uint8_t cmd = (tx_cmd << 2) | SPI_WRITE;
  uint8_t rx_data[4];
  for (uint8_t i = 0; i < l_cmd / 8; i++)
  {
    rx_data[i] = (data << (l_cmd + (i * 8))) >> 24 ;
  }
  INA_CS_0();

  HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);

  HAL_SPI_Transmit(&hspi1, (uint8_t*)&rx_data, l_cmd / 8, 10);

  INA_CS_1();
  
  irq_enable();
  return;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Resets all registers to default values.                                    ┃
void INA_SysRESET(void)//                                                    ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  WriteReg_INA(INA_CONFIG, 16, 0x1000);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Задержка перед начальным преобразованием АЦП, милиСек  кратно двойке       ┃
void INA_CONVDLY(uint16_t time)//        0 - (default)                       ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint16_t data = (time / 2) << 6;
  WriteReg_INA(INA_CONFIG, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Выбор диапазона токового АЦП IN+IN–,  0 = 163.84 mV (default)              ┃
uint8_t INA_ADCRANGE_VALUE = 0;//                                            ┃
void INA_ADCRANGE(uint8_t bit)//        1 =  40.96 mV                        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  INA_ADCRANGE_VALUE = bit;
  uint8_t data = bit << 4;
  WriteReg_INA(INA_CONFIG, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Установка режима работы, непрерывного или при изменении к-либо параметра   ┃
//0h = Shutdown                                                              ┃
//1h = Triggered bus voltage, single shot                                    ┃
//2h = Triggered shunt voltage triggered, single shot                        ┃
//3h = Triggered shunt voltage and bus voltage, single shot                  ┃
//4h = Triggered temperature, single shot                                    ┃
//5h = Triggered temperature and bus voltage, single shot                    ┃
//6h = Triggered temperature and shunt voltage, single shot                  ┃
//7h = Triggered bus voltage, shunt voltage and temperature, single shot     ┃
//8h = Shutdown                                                              ┃
//9h = Continuous bus voltage only                                           ┃
//Ah = Continuous shunt voltage only                                         ┃
//Bh = Continuous shunt and bus voltage                                      ┃
//Ch = Continuous temperature only                                           ┃
//Dh = Continuous bus voltage and temperature                                ┃
//Eh = Continuous temperature and shunt voltage                              ┃
//Fh = Continuous bus, shunt voltage and temperature  (default)              ┃
void INA_MODE(uint8_t bit)//                                                 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
//  uint8_t data = bit << 12;
  WriteReg_INA(INA_ADC_CONFIG, 16, 0xBFFF);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Время преобразования измерения напряжения VBUS                             ┃
//0h = 50 us                                                                 ┃
//1h = 84 us                                                                 ┃
//2h = 150 us                                                                ┃
//3h = 280 us                                                                ┃
//4h = 540 us                                                                ┃
//5h = 1052 us  (default)                                                    ┃
//6h = 2074 us                                                               ┃
//7h = 4120 us                                                               ┃
void INA_VBUSCT(uint8_t bit)//                                               ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint8_t data = bit << 9;
  WriteReg_INA(INA_ADC_CONFIG, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Время преобразования измерения напряжения ШУНТА                            ┃
//0h = 50 us                                                                 ┃
//1h = 84 us                                                                 ┃
//2h = 150 us                                                                ┃
//3h = 280 us                                                                ┃
//4h = 540 us                                                                ┃
//5h = 1052 us  (default)                                                    ┃
//6h = 2074 us                                                               ┃
//7h = 4120 us                                                               ┃
void INA_VSHCT(uint8_t bit)//                                                ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint8_t data = bit << 6;
  WriteReg_INA(INA_ADC_CONFIG, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Время преобразования измерения ТЕМПЕРАТУРЫ                                 ┃
//0h = 50 us                                                                 ┃
//1h = 84 us                                                                 ┃
//2h = 150 us                                                                ┃
//3h = 280 us                                                                ┃
//4h = 540 us                                                                ┃
//5h = 1052 us  (default)                                                    ┃
//6h = 2074 us                                                               ┃
//7h = 4120 us                                                               ┃
void INA_VTCT(uint8_t bit)//                                                 ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint8_t data = bit << 3;
  WriteReg_INA(INA_ADC_CONFIG, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Счетчик усреднения выборки АЦП. Применяется ко всем активным входам.       ┃
//0h = 1  (default)                                                          ┃
//1h = 4                                                                     ┃
//2h = 16                                                                    ┃
//3h = 64                                                                    ┃
//4h = 128                                                                   ┃
//5h = 256                                                                   ┃
//6h = 512                                                                   ┃
//7h = 1024                                                                  ┃
void INA_AVG(uint8_t bit)//                                                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint8_t data = bit << 0;
  WriteReg_INA(INA_ADC_CONFIG, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Задание сопротивления токового ШУНТА.                                      ┃
//1000h  (default)                                                           ┃
void INA_CURRLSB(uint16_t max_current_mA, uint16_t resistance_uOhm)//        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint32_t data = SHUNT_CAL;
  if (INA_ADCRANGE_VALUE > 0)
  {
    data *= 4;
  }
  WriteReg_INA(INA_SHUNT_CAL, 16, data);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Настройка перед включением                                                 ┃
void INA_INIT(void)//                                                        ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  INA_CS_1();
  INA_SysRESET();
  INA_CONVDLY(50);
  INA_ADCRANGE(0x00);
  INA_MODE(0x0F);
//  INA_VBUSCT(0x07);
//  INA_VSHCT(0x07);
//  INA_VTCT(0x07);
//  INA_AVG(0x07);
  INA_CURRLSB(10000, 16666);
  ina239_StartUp();
  LL_TIM_EnableCounter(TIM6);
  LL_TIM_EnableIT_UPDATE(TIM6);
}
