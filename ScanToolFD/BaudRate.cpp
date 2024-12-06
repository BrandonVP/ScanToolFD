/*
 ===========================================================================
 Name        : baudRate.c
 Author      : Brandon Van Pelt
 Created	 : 12/6/2024
 Description : Set CAN Bus baud rates
 ===========================================================================
 */

#define _BAUDRATE_C

#include "baudRate.h"

// BAUD_Baud state machine variables
static uint8_t BAUD_baudInput = 0xFF;
static uint8_t BAUD_baudSpeed = 0;

static uint32_t BAUD_CAN1BaudRate = 500000;
static uint32_t BAUD_CAN2BaudRate = 500000;
static uint32_t BAUD_CANBusFDBaudRate = 1000000;

// Create the baud rate page buttons
uint8_t BAUD_createBaudBtns()
{
	uint16_t lStartX = 255;
	uint16_t lEndX = 460;
	uint16_t rStartX = 20;
	uint16_t rEndX = 235;

	userInterfaceButton[BTN_baud_label1].setButton(rStartX, 60, rEndX, 90, BTN_baud_label1, false, 0, F("CAN1"), ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label1].setClickable(false);
	userInterfaceButton[BTN_baud_can1].setButton(rStartX, 90, rEndX, 130, BTN_baud_can1, false, 0, Can1.getBaudRate(), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label2].setButton(rStartX, 130, rEndX, 160, BTN_baud_label2, false, 0, F("CAN2"), ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label2].setClickable(false);
	userInterfaceButton[BTN_baud_can2].setButton(rStartX, 160, rEndX, 200, BTN_baud_can2, false, 0, Can2.getBaudRate(), ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label3].setButton(rStartX, 200, rEndX, 230, BTN_baud_label3, false, 0, F("CAN3 FD"), ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label3].setClickable(false);
	userInterfaceButton[BTN_baud_can3].setButton(rStartX, 230, rEndX, 270, BTN_baud_can3, false, 0, BAUD_CANBusFDBaudRate, ALIGN_CENTER, menuBtnColor, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_canSet].setButton(rStartX, 270, rEndX, 315, BTN_baud_canSet, false, 0, F("Set"), ALIGN_CENTER, themeBackground, BlackBtnColor, BlackBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_canSet].setClickable(false);

	userInterfaceButton[BTN_baud_table].setButton(lStartX, 60, lEndX, 300, BTN_baud_table, false, BTN_baud_table, F(""), ALIGN_CENTER, menuBackground, BlackBtnColor, menuBackground, menuBackground);
	userInterfaceButton[BTN_baud_table].setClickable(false);
	userInterfaceButton[BTN_baud_5m].setButton(lStartX, 60, lEndX, 92, BTN_baud_5m, false, 0, F("5M"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_2m].setButton(lStartX, 92, lEndX, 124, BTN_baud_2m, false, 0, F("2M"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_1m].setButton(lStartX, 124, lEndX, 156, BTN_baud_1m, false, 0, F("1M"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_800k].setButton(lStartX, 156, lEndX, 188, BTN_baud_800k, false, 0, F("800K"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_500k].setButton(lStartX, 188, lEndX, 220, BTN_baud_500k, false, 0, F("500K"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_250k].setButton(lStartX, 220, lEndX, 252, BTN_baud_250k, false, 0, F("250K"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_125k].setButton(lStartX, 252, lEndX, 284, BTN_baud_125k, false, 0, F("125K"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_100k].setButton(lStartX, 284, lEndX, 315, BTN_baud_100k, false, 0, F("100K"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);

	return BTN_baud_button_count;
}

// Main baud function
void BAUD_Baud(int userInput)
{
	// -----------Ports-----------
	if (userInput >= BTN_baud_can1 && (userInput <= BTN_baud_can3))
	{
		//Serial.println(userInput);
		// Dehighlight current selected port
		if (BAUD_baudInput == userInput)
		{
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(menuBtnColor);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Set selection to none
			BAUD_baudInput = 0xFF;
		}
		else
		{
			delay(100);
			// Dehighlight last selected port
			graphicLoaderState = BAUD_baudInput + 1;
			userInterfaceButton[BAUD_baudInput].setBgColor(menuBtnColor);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Highlight current selected port
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(OrangeBtnColor);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Assign new port
			BAUD_baudInput = userInput;
		}

		// Check for start/stop button highlights
		if ((BAUD_baudSpeed > 0) && (BAUD_baudInput < 0xFF))
		{
			userInterfaceButton[BTN_baud_canSet].setClickable(true);
			GUI_drawRoundBtn(20, 270, 235, 315, F("Set"), menuBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
		}
		else
		{
			userInterfaceButton[BTN_baud_canSet].setClickable(false);
			GUI_drawRoundBtn(20, 270, 235, 315, F("Set"), themeBackground, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
		}

		display.updateScreen();
		return;
	}

	// -----------Speeds-----------
	if (userInput >= BTN_baud_5m && (userInput <= BTN_baud_100k))
	{
		// Dehighlight current selected baud
		if (BAUD_baudSpeed == userInput)
		{
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Set selection to none
			BAUD_baudSpeed = 0;
		}
		else
		{
			// Dehighlight last selected baud
			graphicLoaderState = BAUD_baudSpeed + 1;
			userInterfaceButton[BAUD_baudSpeed].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Highlight current selected baud
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(OrangeBtnColor);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Assign new baud
			BAUD_baudSpeed = userInput;
		}

		// Check for start/stop button highlights
		if ((BAUD_baudSpeed > 0) && (BAUD_baudInput < 0xFF))
		{
			userInterfaceButton[BTN_baud_canSet].setClickable(true);
			GUI_drawRoundBtn(20, 270, 235, 315, F("Set"), menuBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
		}
		else
		{
			userInterfaceButton[BTN_baud_canSet].setClickable(false);
			GUI_drawRoundBtn(20, 270, 235, 315, F("Set"), themeBackground, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
		}
		display.updateScreen();
		return;
	}

	// -----------Set-----------
	if (userInput == BTN_baud_canSet)
	{
		switch (BAUD_baudInput)
		{
		case BTN_baud_can1:
			BAUD_CAN1BaudRate = BAUD_convertBaudRate((baud_btn)BAUD_baudSpeed);
			Can1.setBaudRate(BAUD_convertBaudRate((baud_btn)BAUD_baudSpeed));
			userInterfaceButton[BTN_baud_can1].setText(Can1.getBaudRate());
			GUI_drawRoundBtn(20, 90, 235, 130, Can1.getBaudRate(), OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			break;
		case BTN_baud_can2:
			BAUD_CAN2BaudRate = BAUD_convertBaudRate((baud_btn)BAUD_baudSpeed);
			Can2.setBaudRate(BAUD_convertBaudRate((baud_btn)BAUD_baudSpeed));
			userInterfaceButton[BTN_baud_can2].setText(Can2.getBaudRate());
			GUI_drawRoundBtn(20, 160, 235, 200, Can2.getBaudRate(), OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			break;
		case BTN_baud_can3:
			BAUD_CANBusFDBaudRate = BAUD_convertBaudRate((baud_btn)BAUD_baudSpeed);
			userInterfaceButton[BTN_baud_can3].setText(BAUD_CANBusFDBaudRate);
			GUI_drawRoundBtn(20, 230, 235, 270, BAUD_CANBusFDBaudRate, OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			setCANBusFD(CLK_24MHz, BAUD_CANBusFDBaudRate);
			break;
		}
		display.updateScreen();
	}
}

// Convert from button return value to actual baud value (scales up)
int BAUD_convertBaudRate(int baud)
{
	switch (baud)
	{
	case BTN_baud_100k:
		return 100000;
	case BTN_baud_125k:
		return 125000;
	case BTN_baud_250k:
		return 250000;
	case BTN_baud_500k:
		return 500000;
	case BTN_baud_800k:
		return 800000;
	case BTN_baud_1m:
		return 1000000;
	case BTN_baud_2m:
		return 5000000;
	case BTN_baud_5m:
		return 5000000;
	}
	return 5000000;
}

// TODO
uint32_t BAUD_getBaudRate1()
{
	return BAUD_CAN1BaudRate;
}

uint32_t BAUD_getBaudRate2()
{
	return BAUD_CAN2BaudRate;
}

uint32_t BAUD_getBaudRateFD()
{
	return BAUD_CANBusFDBaudRate;
}