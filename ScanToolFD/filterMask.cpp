/*
 ===========================================================================
 Name        : filterMask.c
 Author      : Brandon Van Pelt
 Created	 : 12/6/2024
 Description : Set filter and mask
 ===========================================================================
 */

#define _FILTERMASK_C

#include "filterMask.h"
#include "KeyInput.h"

static uint8_t FILTERMASK_state = 0;
static uint8_t FILTERMASK_index = 0;
static uint32_t FILTERMASK_hexTotal = 0;
static uint32_t FILTERMASK_filterTable[2][4] = { {0x00 , 0x00 , 0x00 , 0x00}, {0x1FFFFFFF, 0x1FFFFFFF, 0x1FFFFFFF, 0x1FFFFFFF} };

//
void FILTERMASK_clearLocalVar()
{
	FILTERMASK_state = 0;
	FILTERMASK_index = 0;
	FILTERMASK_hexTotal = 0;
}

 //
uint8_t FILTERMASK_createFilterMaskBtns()
{
	uint16_t lStartX = 5;
	uint16_t lEndX = 98;

	uint16_t fStartX = 98;
	uint16_t fEndX = 238;

	uint16_t mStartX = 238;
	uint16_t mEndX = 388;

	uint16_t oStartX = 388;
	uint16_t oEndX = 485;

	// Column 1
	userInterfaceButton[BTN_filterMask_CANLabel].setButton(lStartX, 60, lEndX, 110, BTN_filterMask_CANLabel, false, 0, F("CAN"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_CANLabel].setClickable(false);
	userInterfaceButton[BTN_filterMask_CAN1Label].setButton(lStartX, 110, lEndX, 160, BTN_filterMask_CAN1Label, false, 0, F("1"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_CAN1Label].setClickable(false);
	userInterfaceButton[BTN_filterMask_CAN2Label].setButton(lStartX, 160, lEndX, 210, BTN_filterMask_CAN2Label, false, 0, F("2"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_CAN2Label].setClickable(false);
	userInterfaceButton[BTN_filterMask_CAN3Label].setButton(lStartX, 210, lEndX, 260, BTN_filterMask_CAN3Label, false, 0, F("3"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_CAN3Label].setClickable(false);
	userInterfaceButton[BTN_filterMask_WiFiLabel].setButton(lStartX, 260, lEndX, 310, BTN_filterMask_WiFiLabel, false, 0, F("WiFi"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_WiFiLabel].setClickable(false);

	// Column 2
	userInterfaceButton[BTN_filterMask_filterLabel].setButton(fStartX, 60, fEndX, 110, BTN_filterMask_filterLabel, false, 0, F("Min"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_filterLabel].setClickable(false);
	userInterfaceButton[BTN_filterMask_filter1].setButton(fStartX, 110, fEndX, 160, BTN_filterMask_filter1, false, 0, String(FILTERMASK_filterTable[0][0], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_filter2].setButton(fStartX, 160, fEndX, 210, BTN_filterMask_filter2, false, 0, String(FILTERMASK_filterTable[0][1], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_filter3].setButton(fStartX, 210, fEndX, 260, BTN_filterMask_filter3, false, 0, String(FILTERMASK_filterTable[0][2], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_filterWiFi].setButton(fStartX, 260, fEndX, 310, BTN_filterMask_filterWiFi, false, 0, String(FILTERMASK_filterTable[0][3], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);

	// Column 3
	userInterfaceButton[BTN_filterMask_maskLabel].setButton(mStartX, 60, mEndX, 110, BTN_filterMask_maskLabel, false, 0, F("Max"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_maskLabel].setClickable(false);
	userInterfaceButton[BTN_filterMask_mask1].setButton(mStartX, 110, mEndX, 160, BTN_filterMask_mask1, false, 0, String(FILTERMASK_filterTable[1][0], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_mask2].setButton(mStartX, 160, mEndX, 210, BTN_filterMask_mask2, false, 0, String(FILTERMASK_filterTable[1][1], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_mask3].setButton(mStartX, 210, mEndX, 260, BTN_filterMask_mask3, false, 0, String(FILTERMASK_filterTable[1][2], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_maskWiFi].setButton(mStartX, 260, mEndX, 310, BTN_filterMask_maskWiFi, false, 0, String(FILTERMASK_filterTable[1][3], 16), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);

	// Column 4
	userInterfaceButton[BTN_filterMask_openLabel].setButton(oStartX, 60, oEndX, 110, BTN_filterMask_openLabel, false, 0, F("Reset"), ALIGN_CENTER, frameBorder, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_openLabel].setClickable(false);
	userInterfaceButton[BTN_filterMask_open1].setButton(oStartX, 110, oEndX, 160, BTN_filterMask_open1, false, 0, F("All"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_open2].setButton(oStartX, 160, oEndX, 210, BTN_filterMask_open2, false, 0, F("All"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_open3].setButton(oStartX, 210, oEndX, 260, BTN_filterMask_open3, false, 0, F("All"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_filterMask_openWifi].setButton(oStartX, 260, oEndX, 310, BTN_filterMask_openWifi, false, 0, F("All"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, OrangeBtnColor, menuBtnText);

	return BTN_filterMask_button_count;
}

//
void FILTERMASK_filterMask(int userInput)
{
	const uint8_t MAX_SIZE_EXT_ID = 7;
	const uint8_t ALIGN_KEYPAD_LEFT = 244;
	const uint8_t ALIGN_KEYPAD_RIGHT = 4;

	// Print the hex keypad
	if (FILTERMASK_state == 0 && (userInput >= BTN_filterMask_filter1) && (userInput <= BTN_filterMask_maskWiFi))
	{
		display.useFrameBuffer(false);
		if (userInput < BTN_filterMask_maskLabel)
		{
			keyPadButtons = KEYINPUT_createHexpadButtons(ALIGN_KEYPAD_RIGHT);
		}
		else
		{
			keyPadButtons = KEYINPUT_createHexpadButtons(ALIGN_KEYPAD_LEFT);
		}
		graphicLoaderState = 1;
		while (GUI_drawPage(userKeyButtons, graphicLoaderState, keyPadButtons));
		GUI_isButtonsEnabled(userInterfaceButton, BTN_filterMask_CANLabel, BTN_filterMask_openWifi, false);
		FILTERMASK_state = userInput;
		display.useFrameBuffer(true);
	}

	// Do something with the hex keypad input
	if ((FILTERMASK_state > BTN_filterMask_filterLabel) && (FILTERMASK_state < BTN_filterMask_openLabel))
	{
		uint8_t change = KEYINPUT_keypadHexController(FILTERMASK_hexTotal, FILTERMASK_index, MAX_SIZE_EXT_ID);
		if (change == KEY_CHANGE)
		{
			graphicLoaderState = 1;
			userInterfaceButton[FILTERMASK_state].setText(String(FILTERMASK_hexTotal, 16));
			while (GUI_drawPage(&userInterfaceButton[FILTERMASK_state], graphicLoaderState, 1));
		}
		if (change == KEY_ACCEPT)
		{
			if ((FILTERMASK_state >= BTN_filterMask_filter1) && (FILTERMASK_state <= BTN_filterMask_filterWiFi))
			{
				GUI_drawSquareBtn(245 - ALIGN_KEYPAD_RIGHT, 53, 480 - ALIGN_KEYPAD_RIGHT, 319, themeBackground, themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
				FILTERMASK_filterTable[0][FILTERMASK_state - BTN_filterMask_filter1] = FILTERMASK_hexTotal;
				// Print right side
				graphicLoaderState = 1;
				FILTERMASK_createFilterMaskBtns();
				while (GUI_drawPage(&userInterfaceButton[BTN_filterMask_maskLabel], graphicLoaderState, 10));
			}
			else
			{
				GUI_drawSquareBtn(245 - ALIGN_KEYPAD_LEFT, 53, 480 - ALIGN_KEYPAD_LEFT, 319, themeBackground, themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
				FILTERMASK_filterTable[1][FILTERMASK_state - BTN_filterMask_mask1] = FILTERMASK_hexTotal;
				// Print left side
				graphicLoaderState = 1;
				FILTERMASK_createFilterMaskBtns();
				while (GUI_drawPage(&userInterfaceButton[BTN_filterMask_CANLabel], graphicLoaderState, 10));
			}

			if ((FILTERMASK_state == BTN_filterMask_filter1) || (FILTERMASK_state == BTN_filterMask_mask1))
			{
				Can1.setMBFilter(MB0, (FILTERMASK_filterTable[0][0] & 0x7FF), (FILTERMASK_filterTable[1][0] & 0x7FF));
				Can1.setMBFilter(MB1, (FILTERMASK_filterTable[0][0] & 0x1FFFFFFF), (FILTERMASK_filterTable[1][0] & 0x1FFFFFFF));
			}
			else if ((FILTERMASK_state == BTN_filterMask_filter2) || (FILTERMASK_state == BTN_filterMask_mask2))
			{
				Can2.setMBFilter(MB0, (FILTERMASK_filterTable[0][1] & 0x7FF), (FILTERMASK_filterTable[1][1] & 0x7FF));
				Can2.setMBFilter(MB1, (FILTERMASK_filterTable[0][1] & 0x1FFFFFFF), (FILTERMASK_filterTable[1][1] & 0x1FFFFFFF));
			}
			else if ((FILTERMASK_state == BTN_filterMask_filter3) || (FILTERMASK_state == BTN_filterMask_mask3))
			{
				//Can3.setMBFilter(MB0, (FILTERMASK_filterTable[0][1] & 0x7FF), (FILTERMASK_filterTable[1][1] & 0x7FF));
				//Can3.setMBFilter(MB1, (FILTERMASK_filterTable[0][1] & 0x1FFFFFFF), (FILTERMASK_filterTable[1][1] & 0x1FFFFFFF));
			}
			else if ((FILTERMASK_state == BTN_filterMask_filterWiFi) || (FILTERMASK_state == BTN_filterMask_maskWiFi))
			{

			}

			display.updateScreen();
			FILTERMASK_clearLocalVar();
			KEYINPUT_clearInput();
		}
		if (change == KEY_CANCEL)
		{
			if ((FILTERMASK_state >= BTN_filterMask_filter1) && (FILTERMASK_state <= BTN_filterMask_filterWiFi))
			{
				// Clear input from screen 
				graphicLoaderState = 1;
				userInterfaceButton[FILTERMASK_state].setText(String(FILTERMASK_filterTable[0][FILTERMASK_state - BTN_filterMask_filter1], 16));
				while (GUI_drawPage(&userInterfaceButton[FILTERMASK_state], graphicLoaderState, 1));
				GUI_drawSquareBtn(245 - ALIGN_KEYPAD_RIGHT, 53, 480 - ALIGN_KEYPAD_RIGHT, 319, themeBackground, themeBackground, themeBackground, themeBackground, ALIGN_CENTER);

				// Print right side
				graphicLoaderState = 1;
				FILTERMASK_createFilterMaskBtns();
				while (GUI_drawPage(&userInterfaceButton[BTN_filterMask_maskLabel], graphicLoaderState, 10));
			}
			else
			{
				// Clear input from screen 
				graphicLoaderState = 1;
				userInterfaceButton[FILTERMASK_state].setText(String(FILTERMASK_filterTable[1][FILTERMASK_state - BTN_filterMask_mask1], 16));
				while (GUI_drawPage(&userInterfaceButton[FILTERMASK_state], graphicLoaderState, 1));
				GUI_drawSquareBtn(245 - ALIGN_KEYPAD_LEFT, 53, 480 - ALIGN_KEYPAD_LEFT, 319, themeBackground, themeBackground, themeBackground, themeBackground, ALIGN_CENTER);

				// Print left side
				graphicLoaderState = 1;
				FILTERMASK_createFilterMaskBtns();
				while (GUI_drawPage(&userInterfaceButton[BTN_filterMask_CANLabel], graphicLoaderState, 10));
			}
			display.updateScreen();
			FILTERMASK_clearLocalVar();
			KEYINPUT_clearInput();
		}
	}

	if ((userInput >= BTN_filterMask_open1) && (userInput <= BTN_filterMask_openWifi))
	{
		if (userInput == BTN_filterMask_open1)
		{
			FILTERMASK_filterTable[0][0] = 0x00;
			FILTERMASK_filterTable[1][0] = 0x1FFFFFFF;
		}
		else if (userInput == BTN_filterMask_open2)
		{
			FILTERMASK_filterTable[0][1] = 0x00;
			FILTERMASK_filterTable[1][1] = 0x1FFFFFFF;
		}
		else if (userInput == BTN_filterMask_open3)
		{
			FILTERMASK_filterTable[0][2] = 0x00;
			FILTERMASK_filterTable[1][2] = 0x1FFFFFFF;
		}
		else if (userInput == BTN_filterMask_openWifi)
		{
			FILTERMASK_filterTable[0][3] = 0x00;
			FILTERMASK_filterTable[1][3] = 0x1FFFFFFF;
		}
		graphicLoaderState = 1;
		FILTERMASK_createFilterMaskBtns();
		while (GUI_drawPage(&userInterfaceButton[BTN_filterMask_filter1], graphicLoaderState, 9));
		display.updateScreen();
	}
}