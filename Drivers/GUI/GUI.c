/*
 * GUI.c
 *
 *  Created on: Sep 30, 2025
 *      Author: 20948
 */
#include "GUI.h"


//Refresh0: Page structure
//Refresh1: Status
//Refresh2:	Variables
void GUI_Refresh(uint8_t Weigh){

		if(Weigh <= 0){
			LCD_FillScreen(BLACK);
			//LCD_DrawString(10, 300, "PAGE 1", WHITE, BLACK, 1);
			}

			Show_SC(SC_adc_to_int(SC_ADC),320,290);
			Show_Speed(Wheel_Speed_v01, 180, 30);
			Show_Pedal(Pedal_ADC_1_v11, Pedal_1_up, Pedal_1_down, Pedal_ADC_2_v12, Pedal_2_up, Pedal_2_down);
			Show_Unreliable(Unreliable_Status1, Unreliable_Status2, 70, 260);
			Show_G_Force(360, 120, 0, 0, Weigh);
			Show_Mincell(360, 10, MinCellVolt_v05, Weigh);
			Show_BatVoltage(420, 10, BatVoltage_v02, Weigh);
			Show_InverterErr(280, 240, InverterErr);
			Show_LV_BatVolt(100,280,LV_volt,Weigh);
			Show_Oil_Pressure(100, 200, Oil_ADC1, Oil_ADC2, Weigh);
			//Show_CAN_unit(0x888, _CAN_unit_to_show, 70, 160);

			//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void User_input(){
	return;
}

int8_t SC = -1;
void Show_SC(int8_t _num_of_high, uint16_t x0, uint16_t y0){
	if(_num_of_high == SC) return;
	LCD_DrawRectangle(x0 + 15, y0 - 25, x0 + 15 + 150, y0 - 25 +16, BLACK, 1);
	SC = _num_of_high;
	switch(_num_of_high){
		case 0:	LCD_DrawString(x0 + 15, y0 - 25, "BSPD off      ", RED, BLACK, 1);	break;//0ff:0x076B 1899
		case 1:	LCD_DrawString(x0 + 15, y0 - 25, "BMS off       ", RED, BLACK, 1);	break;//2213
		case 2:	LCD_DrawString(x0 + 15, y0 - 25, "IMD off       ", RED, BLACK, 1);	break;//off:0x0816 2.527
		case 3:	LCD_DrawString(x0 + 15, y0 - 25, "RING off      ", RED, BLACK, 1);	break;//0ff:0x0918 2.842 +315
		case 4:	LCD_DrawString(x0 + 15, y0 - 25, "FRONT_SC off  ", RED, BLACK, 1);	break;//0ff:0x0A19 3.156 +314
		case 5:	LCD_DrawString(x0 + 15, y0 - 25, "HV BOX off    ", RED, BLACK, 1);	break;//3470
		case 6:	LCD_DrawString(x0 + 15, y0 - 25, "SIDE PANEL off", RED, BLACK, 1);	break;//3784
		case 7:	LCD_DrawString(x0 + 15, y0 - 25, "REAR SC off   ", RED, BLACK, 1);	break;//4098
		case 8:	LCD_DrawString(x0 + 15, y0 - 25, "BATT off      ", RED, BLACK, 1);	break;//4312
		case 9: LCD_DrawString(x0 + 15, y0 - 25, "All OK        ", GREEN, BLACK, 1);break;//4628
		default: LCD_DrawString(x0 + 15, y0 - 25, "SC SIGNAL ERROR", ORANGE, BLACK, 1);
		for(int i = 9;i > 0;i--){
				LCD_DrawCircle(x0 + 15*i, y0, 6, (i > 0)?(RED):GREEN, 1);
			}
			return;
	}
	for(int i = 9;i > 0;i--){
		LCD_DrawCircle(x0 + 15*i, y0, 6, (i > _num_of_high)?(GRAY):GREEN, 1);
	}
	return;
}

uint8_t digit001[4] = {-1,-1,-1,-1};
void Show_Speed(uint8_t _speed, uint16_t x0, uint16_t y0){//265 110
	//if(_speed > 25) _speed = 25; //新国标电动车

	if(digit001[3] != 0){LCD_DrawRectangle(x0 - 105, y0 - 10, x0 + 55, y0 + 110,WHITE,0); digit001[3] = 0;}
	uint8_t digit = _speed % 10;
	if(digit != digit001[0]){
	LCD_DrawChar(x0, y0, digit + 0x30, WHITE, BLACK, 6);
	digit001[0] = digit;
	}
	_speed = _speed / 10;
	digit = _speed % 10;
	if(digit != digit001[1]){
	LCD_DrawChar(x0 - 50, y0, digit + 0x30, WHITE, BLACK, 6);
	digit001[1] = digit;
	}
	_speed = _speed / 10;
	digit = _speed % 10;
	if(digit != digit001[2]){
	LCD_DrawChar(x0 - 100, y0, digit + 0x30, WHITE, BLACK, 6);
	digit001[2] = digit;
	}
}

void Show_CAN_unit(uint32_t can_rx_id, uint8_t *data, uint16_t x0, uint16_t y0){
	uint8_t R;
	uint8_t L;
	for(int i = 8; i > 0; i--){
		 R = *(data + i - 1)%0x10;
		LCD_DrawCharHex(x0 + i*20, y0, R, WHITE, BLACK, 1);
		 L = *(data + i - 1)/0x10;
		LCD_DrawCharHex(x0 + i*20-8, y0, L, WHITE, BLACK, 1);
	}
}

uint8_t _padel_cache = 1;
uint16_t _min1_cache = 0;
uint16_t _max1_cache = 0;
uint16_t _min2_cache = 0;
uint16_t _max2_cache = 0;
uint16_t _pedal_1_pixel_cache = 286;
uint16_t _pedal_2_pixel_cache = 286;
void Show_Pedal(uint16_t Pedal_ADC_1, uint16_t Pedal_1_up, uint16_t Pedal_1_down, uint16_t Pedal_ADC_2, uint16_t Pedal_2_up, uint16_t Pedal_2_down){
	if(_padel_cache){
		LCD_DrawRectangle(5, 20, 25, 288, WHITE, 0);
		LCD_DrawRectangle(30, 20, 50, 288, WHITE, 0);
		LCD_DrawLine(24, 40, 31, 40, RED);
		LCD_DrawLine(24, 268, 31, 268, RED);
		LCD_DrawString(9, 300, "Pedal", WHITE, BLACK, 1);
		_padel_cache = 0;
	}
		_min1_cache = Pedal_1_down*4096/500;
		_max1_cache = Pedal_1_up*4096/500;
		_min2_cache = Pedal_2_down*4096/500;
		_max2_cache = Pedal_2_up*4096/500;
	//LCD_DrawRectangle(7, 22, 23, 286, WHITE, 1);
	//LCD_DrawRectangle(32, 22, 48, 286, WHITE, 1);
		double _Pedal_1 = ((int)Pedal_ADC_1 - _min1_cache)/(double)((int)_max1_cache - (int)_min1_cache);
		//double _Pedal_1 = 1;
		double _p_pixel1 = 268 - _Pedal_1*228;
		_p_pixel1 = (_p_pixel1 < 22) ? 22 : ((_p_pixel1 > 286) ? 286 : _p_pixel1);
		uint16_t pixel1 = _p_pixel1;
		if(pixel1 < _pedal_1_pixel_cache){
			LCD_DrawRectangle(7, pixel1, 23, _pedal_1_pixel_cache, WHITE, 1);
			_pedal_1_pixel_cache = pixel1;
		}

		if(pixel1 > _pedal_1_pixel_cache){
			LCD_DrawRectangle(7, _pedal_1_pixel_cache, 23, pixel1 - 1, BLACK, 1);
			_pedal_1_pixel_cache = pixel1;
		}

		float _Pedal_2 = ((int)Pedal_ADC_2 - _min2_cache)/(double)((int)_max2_cache - (int)_min2_cache);
		double _p_pixel2 = 268 - _Pedal_2*228;
		_p_pixel2 = (_p_pixel2 < 22) ? 22 : ((_p_pixel2 > 286) ? 286 : _p_pixel2);
		uint16_t pixel2 = _p_pixel2;
		if(pixel2 < _pedal_2_pixel_cache){
			LCD_DrawRectangle(32, pixel2, 48, _pedal_2_pixel_cache, WHITE, 1);
			_pedal_2_pixel_cache = pixel2;
		}

		if(pixel2 > _pedal_2_pixel_cache){
			LCD_DrawRectangle(32, _pedal_2_pixel_cache, 48, pixel2 - 1, BLACK, 1);
			_pedal_2_pixel_cache = pixel2;
		}
		return;
}

int8_t SC_adc_to_int(uint16_t volt){
	volt -= 0x5EA;
	return volt/0x102;
}

uint8_t _present_type1 = 0;
uint8_t _present_type2 = 0;
void Show_Unreliable(uint8_t status1,uint8_t status2, uint16_t x0, uint16_t y0){
	//status1 = status2 = 1;
	if(status1 != _present_type1) {
		LCD_DrawCircle(x0, y0, 5, RED, 1);
		LCD_DrawCircle(x0, y0+12, 5, (status1)?RED:BLACK, 1);
		_present_type1 = status1;
	}
	x0 += 12;
	if(status2 != _present_type2) {
		LCD_DrawCircle(x0, y0, 5, WHITE, 1);
		LCD_DrawCircle(x0, y0+12, 5, (status1)?WHITE:BLACK, 1);
		_present_type2 = status2;
	}
};


void Show_Hex_Figure(uint16_t _uint16, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t size){
	LCD_DrawCharHex(x0, y0, _uint16%0x10, color, bg_color, size);
	_uint16 = _uint16/0x10;
	LCD_DrawCharHex(x0-10, y0, _uint16%0x10, color, bg_color, size);
	_uint16 = _uint16/0x10;
	LCD_DrawCharHex(x0-20, y0, _uint16%0x10, color, bg_color, size);
	_uint16 = _uint16/0x10;
	LCD_DrawCharHex(x0-30, y0, _uint16%0x10, color, bg_color, size);
}
void Show_Hex_In_Dec(uint16_t _uint16, uint16_t x0, uint16_t y0, uint16_t color, uint16_t bg_color, uint8_t size){
	LCD_DrawCharHex(x0, y0, _uint16%10, color, bg_color, size);
	_uint16 = _uint16/10;
	LCD_DrawCharHex(x0-10, y0, _uint16%10, color, bg_color, size);
	_uint16 = _uint16/10;
	LCD_DrawCharHex(x0-20, y0, _uint16%10, color, bg_color, size);
	_uint16 = _uint16/10;
	LCD_DrawCharHex(x0-30, y0, _uint16%10, color, bg_color, size);
}

int16_t _gx = 0;
int16_t _gy = 0;
void Show_G_Force(uint16_t x0, uint16_t y0, int16_t gx, int16_t gy, uint8_t _weigh){
	if(!_weigh) LCD_DrawCircle(x0, y0, 80, WHITE, 0);
	//LCD_DrawCircle(x0 + _gx, y0 + _gy, 5, BLACK, 1);
	//LCD_DrawCircle(x0 + gx, y0 + gy, 5, WHITE, 1);
	_gx = gx;	_gy = gy;

}

int16_t _mincellcache = -10;
void Show_Mincell(uint16_t x0, uint16_t y0, uint16_t _MinCellVolt, uint8_t _weigh){
	if(_weigh){
		LCD_DrawRectangle(x0, y0, x0 + 8, y0 + 15, WHITE, 0);
		LCD_DrawRectangle(x0 + 2, y0 - 1, x0 + 6, y0 - 2, WHITE, 0);
		LCD_DrawRectangle(x0 + 1, y0 + 11, x0 + 7, y0 + 14, GREEN, 1);
	}
	if(-5 < (_MinCellVolt - _mincellcache)&&(_MinCellVolt - _mincellcache) < 5) return;
	Show_Hex_In_Dec(_MinCellVolt, x0 + 44, y0, (_MinCellVolt > 3600&&_MinCellVolt < 4180)?WHITE:RED, BLACK, 1);
	LCD_DrawPoint(x0 + 23, y0 + 13, WHITE);
	_mincellcache = _MinCellVolt;
}
int16_t _batvoltcache = -10;
void Show_BatVoltage(uint16_t x0, uint16_t y0, uint16_t _BatVolt, uint8_t _weigh){
	if(_weigh){
		LCD_DrawRectangle(x0, y0, x0 + 8, y0 + 15, WHITE, 0);
		LCD_DrawRectangle(x0 + 2, y0 - 1, x0 + 6, y0 - 2, WHITE, 0);
		LCD_DrawRectangle(x0 + 1, y0 + 3, x0 + 7, y0 + 14, BLUE, 1);
	}
	if(-5 < (_BatVolt - _batvoltcache)&&(_BatVolt - _batvoltcache) < 5) return;
	Show_Hex_In_Dec(_BatVolt, x0 + 44, y0, (_BatVolt > 4600&&_BatVolt < 5400)?WHITE:RED, BLACK, 1);
	LCD_DrawPoint(x0 + 43, y0 + 13, WHITE);
	_batvoltcache = _BatVolt;
}

uint16_t _invertererrcache[4] = {1, 1, 1, 1};
void Show_InverterErr(uint16_t x0, uint16_t y0, uint16_t* _Errof4){
	if(_invertererrcache[0] != _Errof4[0]){
		LCD_DrawString(x0 - 50, y0, "FR", BLACK, (_Errof4[0] == 0)?GREEN:RED, 1);
		Show_Hex_In_Dec(_Errof4[0], x0, y0, WHITE, BLACK, 1);	//FR
		_invertererrcache[0] = _Errof4[0];
	}
	if(_invertererrcache[1] != _Errof4[1]){
		LCD_DrawString(x0 - 50, y0 + 16, "FL", BLACK, (_Errof4[1] == 0)?GREEN:RED, 1);
		Show_Hex_In_Dec(_Errof4[1], x0, y0 + 16, WHITE, BLACK, 1);	//FL
		_invertererrcache[1] = _Errof4[1];
	}
	if(_invertererrcache[2] != _Errof4[2]){
		LCD_DrawString(x0 - 50, y0 + 32, "RR", BLACK, (_Errof4[2] == 0)?GREEN:RED, 1);
		Show_Hex_In_Dec(_Errof4[2], x0, y0 + 32, WHITE, BLACK, 1);	//RR
		_invertererrcache[2] = _Errof4[2];
	}
	if(_invertererrcache[3] != _Errof4[3]){
		LCD_DrawString(x0 - 50, y0 + 48, "RL", BLACK, (_Errof4[3] == 0)?GREEN:RED, 1);
		Show_Hex_In_Dec(_Errof4[3], x0, y0 + 48, WHITE, BLACK, 1);	//RL
		_invertererrcache[3] = _Errof4[3];
	}
}

int16_t _LVBatVoltcache = -10;
void Show_LV_BatVolt(uint16_t x0, uint16_t y0, uint16_t _LVBatVolt, uint8_t _weigh){
	if(_weigh){
		LCD_DrawRectangle(x0, y0, x0 + 8, y0 + 15, WHITE, 0);
		LCD_DrawRectangle(x0 + 2, y0 - 1, x0 + 6, y0 - 2, WHITE, 0);
		LCD_DrawRectangle(x0 + 1, y0 + 3, x0 + 7, y0 + 14, BLUE, 1);
	}
	if(-10 < (_LVBatVolt - _LVBatVoltcache)&&(_LVBatVolt - _LVBatVoltcache) < 10) return;
	Show_Hex_In_Dec(_LVBatVolt, x0 + 44, y0, (_LVBatVolt > 2300&&_LVBatVolt < 4180)?WHITE:RED, BLACK, 1);
	LCD_DrawPoint(x0 + 33, y0 + 13, WHITE);
	_LVBatVoltcache = _LVBatVolt;

}

int16_t _oiladccache1 = -10;
int16_t _oiladccache2 = -10;
void Show_Oil_Pressure(uint16_t x0, uint16_t y0, uint16_t Oil_ADC1, uint16_t Oil_ADC2, uint8_t _weigh){
	if(!(-10 < (Oil_ADC1 - _oiladccache1)&&(Oil_ADC1 - _oiladccache1) < 10)){
	Show_Hex_In_Dec(Oil_ADC1, x0 + 44, y0, (Oil_ADC1 > 0000&&Oil_ADC1 < 4000)?WHITE:RED, BLACK, 1);
	LCD_DrawPoint(x0 + 23, y0 + 13, WHITE);
	_oiladccache1 = Oil_ADC1;
	}
	if(!(-10 < (Oil_ADC2 - _oiladccache2)&&(Oil_ADC2 - _oiladccache2) < 10)){
	Show_Hex_In_Dec(Oil_ADC2, x0 + 44, y0 + 16, (Oil_ADC2 > 0000&&Oil_ADC2 < 4000)?WHITE:RED, BLACK, 1);
	LCD_DrawPoint(x0 + 23, y0 + 29, WHITE);
	_oiladccache2 = Oil_ADC2;
	}
}





























