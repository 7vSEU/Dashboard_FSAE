/*
 * GUI.h
 *
 *  Created on: Sep 30, 2025
 *      Author: 20948
 */

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

#include "stm32h7xx_hal.h"

#include "st7796.h"


extern uint8_t Wheel_Speed_v01;
extern uint16_t Pedal_ADC_1_v11;
extern uint16_t Pedal_1_up;
extern uint16_t Pedal_1_down;
extern uint16_t Pedal_ADC_2_v12;
extern uint16_t Pedal_2_up;
extern uint16_t Pedal_2_down;
extern uint16_t SC_ADC;;
extern uint8_t Unreliable_Status1;
extern uint8_t Unreliable_Status2;
extern uint16_t MinCellVolt_v05;
extern uint16_t BatVoltage_v02;
extern uint16_t InverterErr[];
extern uint16_t LV_volt;
extern uint16_t Oil_ADC1;
extern uint16_t Oil_ADC2;

extern uint8_t _CAN_unit_to_show[8];
extern uint8_t _CAN_unit_to_show2[8];
extern uint8_t _CAN_unit_to_show3[8];
extern uint8_t _CAN_unit_to_show4[8];
extern uint8_t _CAN_unit_to_show5[8];

void GUI_Refresh(uint8_t);
void User_input();
void Show_SC(int8_t,uint16_t,uint16_t);
void Show_Speed(uint8_t,uint16_t,uint16_t);
void Show_CAN_unit(uint32_t, uint8_t*, uint16_t , uint16_t);
void Show_Pedal(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
int8_t SC_adc_to_int(uint16_t);
void Show_Unreliable(uint8_t, uint8_t, uint16_t, uint16_t);
void Show_Hex_Figure(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void Show_Hex_In_Dec(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void Show_G_Force(uint16_t, uint16_t, int16_t, int16_t, uint8_t);
void Show_Mincell(uint16_t, uint16_t, uint16_t, uint8_t);
void Show_BatVoltage(uint16_t, uint16_t, uint16_t, uint8_t);
void Show_InverterErr(uint16_t, uint16_t, uint16_t*);
void Show_LV_BatVolt(uint16_t, uint16_t, uint16_t, uint8_t);
void Show_Oil_Pressure(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);

#endif /* GUI_GUI_H_ */
