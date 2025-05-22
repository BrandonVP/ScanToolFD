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
	uint8_t btnPos = 0;
	uint16_t y = 78;
	GUI_drawSquareBtn(5, y - 25, 160, y, "Send", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(160, y - 25, 250, y, "Cycle", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(250, y - 25, 320, y, "Edit", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(320, y - 25, 420, y, "Channel", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);

	GUI_drawSquareBtn(425, 55, 475, 180, "", menuBackground, frameBorder, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(425, 185, 475, 310, "", menuBackground, frameBorder, menuBtnText, ALIGN_CENTER);

	//ICONS_printIcon(433, 100, upArrow_bits, 32, 4, menuBtnTextColor, 1);
	//ICONS_printIcon(433, 230, downArrow_bits, 32, 4, menuBtnTextColor, 1);

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
	return btnPos;
}

/*
void loadCANBusTXMsg()
{
	//uiSD.readSendMsg(CANBusSendList, MAX_STORED_MESSAGES);
}

void saveCANBusTXMsg()
{
	//uiSD.writeSendMsg(CANBusSendList, MAX_STORED_MESSAGES);
}
*/
