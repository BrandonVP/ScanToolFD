/*
 ===========================================================================
 Name        : CANBusSend.cpp
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus send functions
 ===========================================================================
 */

#define _CANBUS_SEND_c
#include "CANBusSend.h"

//
uint8_t SEND_createSendBtns()
{
	const int height = 45;
	const int interval = 6;

	const int R1_S = 55;
	const int R1_E = R1_S + height;
	const int R2_S = R1_E + interval;
	const int R2_E = R2_S + height;
	const int R3_S = R2_E + interval;
	const int R3_E = R3_S + height;
	const int R4_S = R3_E + interval;
	const int R4_E = R4_S + height;
	const int R5_S = R4_E + interval;
	const int R5_E = R5_S + height;

	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(5, R1_S, 180, R1_E, 0, false, 0, F(""), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);
	userInterfaceButton[btnPos++].setButton(180, R1_S, 260, R1_E, 0, false, 0, F("OFF"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);
	userInterfaceButton[btnPos++].setButton(260, R1_S, 340, R1_E, 0, false, 0, F("EDIT"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);
	userInterfaceButton[btnPos++].setButton(340, R1_S, 420, R1_E, 0, false, 0, F("DATA"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);

	userInterfaceButton[btnPos++].setButton(5, R2_S, 400, R2_E, 0, false, 0, F(""), ALIGN_LEFT, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	userInterfaceButton[btnPos++].setButton(5, R3_S, 400, R3_E, 0, false, 0, F(""), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	userInterfaceButton[btnPos++].setButton(5, R4_S, 400, R4_E, 0, false, 0, F(""), ALIGN_LEFT, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	userInterfaceButton[btnPos++].setButton(5, R5_S, 400, R5_E, 0, false, 0, F(""), ALIGN_LEFT, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	return btnPos;
}