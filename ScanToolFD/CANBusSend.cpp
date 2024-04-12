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
#include "icons.h"

//
uint8_t SEND_createSendBtns()
{
	uint16_t y = 78;
	GUI_drawSquareBtn(5, y - 25, 160, y, "Send", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(160, y - 25, 250, y, "Cycle", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(250, y - 25, 320, y, "Edit", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(320, y - 25, 420, y, "Channel", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);

	GUI_drawSquareBtn(425, 55, 475, 180, "", menuBackground, frameBorder, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(425, 185, 475, 310, "", menuBackground, frameBorder, menuBtnText, ALIGN_CENTER);

	ICONS_printIcon(433, 100, upArrow_bits, 32, 4, menuBtnTextColor, 1);
	ICONS_printIcon(433, 230, downArrow_bits, 32, 4, menuBtnTextColor, 1);

	//GUI_drawSquareBtn(10, 280, 105, 318, "View", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	//GUI_drawSquareBtn(105, 280, 210, 318, "Play", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	//GUI_drawSquareBtn(210, 280, 315, 318, "Split", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	//GUI_drawSquareBtn(315, 280, 420, 318, "Delete", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);


	// Draw the scroll window
	for (uint8_t i = 0; i < 6; i++)
	{
		GUI_drawSquareBtn(5, y, 420, y + 40, "", menuBackground, BlackBtnColor, menuBtnText, ALIGN_LEFT);
		GUI_drawSquareBtn(7, y + 2, 159, y + 39 - 2, "abc", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
		GUI_drawSquareBtn(161, y + 2, 249, y + 39 - 2, "OFF", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
		GUI_drawSquareBtn(251, y + 2, 319, y + 39 - 2, "EDIT", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
		GUI_drawSquareBtn(321, y + 2, 418, y + 39 - 2, "WiFi", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
		y = y + 39;
	}
	display.updateScreen();


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
	/*
	userInterfaceButton[btnPos++].setButton(5, R1_S, 180, R1_E, 0, false, 0, F(""), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);
	userInterfaceButton[btnPos++].setButton(180, R1_S, 260, R1_E, 0, false, 0, F("OFF"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);
	userInterfaceButton[btnPos++].setButton(260, R1_S, 340, R1_E, 0, false, 0, F("EDIT"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);
	userInterfaceButton[btnPos++].setButton(340, R1_S, 420, R1_E, 0, false, 0, F("DATA"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, BlackBtnColor);

	userInterfaceButton[btnPos++].setButton(5, R2_S, 400, R2_E, 0, false, 0, F(""), ALIGN_LEFT, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	userInterfaceButton[btnPos++].setButton(5, R3_S, 400, R3_E, 0, false, 0, F(""), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	userInterfaceButton[btnPos++].setButton(5, R4_S, 400, R4_E, 0, false, 0, F(""), ALIGN_LEFT, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
	userInterfaceButton[btnPos++].setButton(5, R5_S, 400, R5_E, 0, false, 0, F(""), ALIGN_LEFT, OrangeBtnColor, BlackBtnColor, menuBtnText, menuBtnText);
		/**/
	return btnPos;
}