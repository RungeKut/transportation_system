#include "seg_display.h"
#include "main.h"

/*const uint8_t whiting[3][10] = {
{0xF8, 0xF0, 0xF0, 0xF1, 0xF3, 0xF7, 0xFF, 0xFF, 0xFE, 0xFC},
{0xFE, 0xFE, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xFE, 0xFE, 0xFE},
{0xFE, 0xFF, 0xFF, 0xF7, 0xE7, 0xC7, 0xC6, 0xC6, 0xCE, 0xDE}
};
const uint8_t number[] =
{
  0x3f, //0
  0x06, //1
  0x5b, //2
  0x4f, //3
  0x66, //4
  0x6d, //5
  0x7d, //6
  0x07, //7
  0x7f, //8
  0x6f, //9
	0xff  //???????? ??????
};*/

void SegmentON(uint8_t segment)
{
	if (digit == 1) {
		LL_GPIO_ResetOutputPin(Seg_3_Disp_IMO_GPIO_Port, Seg_3_Disp_IMO_Pin);			//off 4 digit
		LL_GPIO_SetOutputPin(Seg_1_Disp_reset_GPIO_Port, Seg_1_Disp_reset_Pin);		//on 1 digit
	} else if (digit == 2) {
		LL_GPIO_ResetOutputPin(Seg_1_Disp_reset_GPIO_Port, Seg_1_Disp_reset_Pin);	//off 1 digit
		LL_GPIO_SetOutputPin(Seg_2_Disp_CS_GPIO_Port, Seg_2_Disp_CS_Pin);					//on 2 digit
	} else if (digit == 3) {
		LL_GPIO_ResetOutputPin(Seg_2_Disp_CS_GPIO_Port, Seg_2_Disp_CS_Pin);				//off 2 digit
		LL_GPIO_SetOutputPin(Seg_3_Disp_IMO_GPIO_Port, Seg_3_Disp_IMO_Pin);				//on 3 digit
	}
	if (segment & 0x01) {
		LL_GPIO_ResetOutputPin(Seg_a_Disp_DC_GPIO_Port, Seg_a_Disp_DC_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_a_Disp_DC_GPIO_Port, Seg_a_Disp_DC_Pin);
	} if (segment & 0x02) {
		LL_GPIO_ResetOutputPin(Seg_b_GPIO_Port, Seg_b_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_b_GPIO_Port, Seg_b_Pin);
	} if (segment & 0x04) {
		LL_GPIO_ResetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	} if (segment & 0x08) {
		LL_GPIO_ResetOutputPin(Seg_d_GPIO_Port, Seg_d_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_d_GPIO_Port, Seg_d_Pin);
	} if (segment & 0x10) {
		LL_GPIO_ResetOutputPin(Seg_e_GPIO_Port, Seg_e_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_e_GPIO_Port, Seg_e_Pin);
	} if (segment & 0x20) {
		LL_GPIO_ResetOutputPin(Seg_f_GPIO_Port, Seg_f_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_f_GPIO_Port, Seg_f_Pin);
	} if (segment & 0x40) {
		LL_GPIO_ResetOutputPin(Seg_g_GPIO_Port, Seg_g_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_g_GPIO_Port, Seg_g_Pin);
	} if (segment & 0x80) {
		LL_GPIO_ResetOutputPin(Seg_dp_GPIO_Port, Seg_dp_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_dp_GPIO_Port, Seg_dp_Pin);
	}
	digit++;
	if (digit >= 4)
	{
		digit = 1;
//For test segment indicator		
//	weight++;
//	if (weight > 999) { weight = 0; }
		
//		if ((timerOff > 0) || GLOBAL_FLAG_TX != 0) 
//		{
//		}
	}
//	if (timerOff > 0)
//	{
//		timerOff--;
//	}
}



