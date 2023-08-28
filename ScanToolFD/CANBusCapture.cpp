/*
 ===========================================================================
 Name        : CANBusCapture.cpp
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus functions
 ===========================================================================
 */

#define _CANBUSCAPTURE_C
#include "CANBusCapture.h"

uint8_t CAPTURE_createMenuBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(55, 80, 220, 120, APP_CAPTURE_CONFIG, true, 20, F("Capture"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(260, 80, 425, 120, APP_CAPTURE_SEND, true, 20, F("Send"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(55, 140, 220, 180, APP_CAPTURE_FILES, true, 20, F("Files"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(260, 140, 425, 180, APP_CAPTURE_BAUD, true, 20, F("Baud"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(55, 200, 220, 240, APP_CAPTURE_FILTERMASK, true, 20, F("Filter Mask"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(260, 200, 425, 240, APP_CAPTURE_AUTOBAUD, true, 20, F("Auto Baud"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(55, 260, 220, 300, 0, true, F(""), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
	return btnPos;
}

uint8_t CAPTURE_createCaptureBtns()
{
	uint8_t btnPos = 0;
	userInterfaceCaptureButton[btnPos]. setButton( 10,  55, 170,  70,  0, true, 20, F("Input"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceCaptureButton[btnPos++]. setClickable(false);
	userInterfaceCaptureButton[btnPos]. setButton(175,  55, 335,  70,  0, true, 20, F("Output"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceCaptureButton[btnPos++]. setClickable(false);
	userInterfaceCaptureButton[btnPos]. setButton( 10,  75, 170, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceCaptureButton[btnPos++]. setClickable(false);
	userInterfaceCaptureButton[btnPos]. setButton(175,  75, 335, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceCaptureButton[btnPos++]. setClickable(false);
	userInterfaceCaptureButton[btnPos++]. setButton( 10,  75, 170, 110,  4, false, 0, F("CAN1 "),     ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++]. setButton( 10, 115, 170, 150,  5, false, 0, F("CAN2 "),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++]. setButton( 10, 155, 170, 190,  6, false, 0, F("CAN3 FD "),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++]. setButton( 10, 195, 170, 230,  7, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++]. setButton( 10, 235, 170, 270,  8, false, 0, F("C2 Out Only"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	
	userInterfaceCaptureButton[btnPos++]. setButton(175,  75, 335, 110,  9, false, 0, F("LCD"),         ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++].setButton(175, 115, 335, 150, 10, false, 0, F("Serial"),      ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++].setButton(175, 155, 335, 190, 11, false, 0, F("SD Card"),     ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++].setButton(175, 195, 335, 230, 12, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos].setButton(340, 230, 475, 270, 13, true, 0, F("START "),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++].setClickable(false);
	userInterfaceCaptureButton[btnPos].setButton(340, 275, 475, 315, 14, true, 0, F("STOP "),         ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[btnPos++].setClickable(false);
	return btnPos;
}

void CAPTURE_captureConfig()
{
	int userInput = GUI_subMenuButtonMonitor(userInterfaceCaptureButton, 15);
	if (userInput > -1 && (userInput < 9))
	{
		if (CAPTURE_input_config != userInput)
		{
			graphicLoaderState = CAPTURE_input_config + 1;
			userInterfaceCaptureButton[CAPTURE_input_config].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);


			graphicLoaderState = userInput + 1;
			userInterfaceCaptureButton[userInput].setBgColor(menuBtnColor);
			GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);
			CAPTURE_input_config = userInput;
		}
		else
		{
			if (userInterfaceCaptureButton[userInput].getBtnColor() == menuBtnColor)
			{
				graphicLoaderState = userInput + 1;
				userInterfaceCaptureButton[userInput].setBgColor(themeBackground);
				GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);
				CAPTURE_input_config = 0;
			}
			else
			{
				graphicLoaderState = userInput + 1;
				userInterfaceCaptureButton[userInput].setBgColor(menuBtnColor);
				GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);
				CAPTURE_input_config = userInput;
			}
		}
		userInput = -1;

		if ((CAPTURE_input_config > 0) && (CAPTURE_output_config > 0))
		{
			userInterfaceCaptureButton[CAPTURE_START_POS].setClickable(true);
		}
		else
		{
			userInterfaceCaptureButton[CAPTURE_START_POS].setClickable(false);
		}
	}
	if (userInput > -1 && (userInput < CAPTURE_START_POS))
	{
		if (CAPTURE_output_config != userInput)
		{
			graphicLoaderState = CAPTURE_output_config + 1;
			userInterfaceCaptureButton[CAPTURE_output_config].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);


			graphicLoaderState = userInput + 1;
			userInterfaceCaptureButton[userInput].setBgColor(menuBtnColor);
			GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);
			CAPTURE_output_config = userInput;
		}
		else
		{
			if (userInterfaceCaptureButton[userInput].getBtnColor() == menuBtnColor)
			{
				graphicLoaderState = userInput + 1;
				userInterfaceCaptureButton[userInput].setBgColor(themeBackground);
				GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);
				CAPTURE_output_config = 0;
			}
			else
			{
				graphicLoaderState = userInput + 1;
				userInterfaceCaptureButton[userInput].setBgColor(menuBtnColor);
				GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 1);
				CAPTURE_output_config = userInput;
			}
		}
		userInput = -1;

		if ((CAPTURE_input_config > 0) && (CAPTURE_output_config > 0))
		{
			userInterfaceCaptureButton[CAPTURE_START_POS].setClickable(true);
		}
		else
		{
			userInterfaceCaptureButton[CAPTURE_START_POS].setClickable(false);
		}
	}
	if (userInput == CAPTURE_START_POS)
	{
		if (CAPTURE_output_config == 9)
		{
			nextApp = APP_CAPTURE_LCD;
		}
		else
		{
			CANBusOut = CAPTURE_output_config;
		}
		CANBusIn = CAPTURE_input_config;
		userInterfaceCaptureButton[CAPTURE_START_POS].setClickable(false);
		userInterfaceCaptureButton[CAPTURE_STOP_POS].setClickable(true);
		
	}
	if (userInput == CAPTURE_STOP_POS)
	{
		CANBusOut = 0;
		userInterfaceCaptureButton[CAPTURE_START_POS].setClickable(true);
		userInterfaceCaptureButton[CAPTURE_STOP_POS].setClickable(false);
	}
}

void CAPTURE_createLCDBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos].setButton(440, 50, 480, 320, 0, false, 0, F(""), ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos++].setButton(408, 60, 479, 105, 1, false, 0, F("Start"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(408, 110, 479, 155, 2, false, 0, F("Stop"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(408, 160, 479, 205, 3, false, 0, F("Clear"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(408, 210, 479, 255, 3, false, 0, F("Config"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(408, 260, 479, 305, 3, false, 0, F("Back"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
}

void CAPTURE_createBaudBtns()
{
	uint8_t btnPos = 0;
	uint16_t lStartX = 10;
	uint16_t lEndX = 180;
	uint16_t rStartX = 190;
	uint16_t rEndX = 380;
	userInterfaceButton[btnPos].  setButton(lStartX,  55, lEndX, 70, 0, true, 20, F("Baud"), ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos].  setButton(lStartX,  75, lEndX, 315, 0, false, 0, F(""), ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos++].setButton(lStartX,  75, lEndX, 105, 4, false, 0, F("5M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 105, lEndX, 135, 5, false, 0, F("2M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 135, lEndX, 165, 6, false, 0, F("1M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 165, lEndX, 195, 7, false, 0, F("800K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 195, lEndX, 225, 8, false, 0, F("500K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 225, lEndX, 255, 8, false, 0, F("250K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 255, lEndX, 285, 8, false, 0, F("125K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 285, lEndX, 315, 8, false, 0, F("100K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos].  setButton(rStartX,  55, 380,  70, 0, true, 20, F("CAN Bus Port"), ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos++].setButton(rStartX,  75, rEndX, 105, 9, false, 0, F("CAN1"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 105, rEndX, 135, 9, false, 0, Can1.getBaudRate(), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rStartX, 135, rEndX, 165, 10, false, 0, F("CAN2"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 165, rEndX, 195, 10, false, 0, Can2.getBaudRate(), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rStartX, 195, rEndX, 225, 11, false, 0, F("CAN3"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 225, rEndX, 255, 11, false, 0, "-", ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rStartX, 255, rEndX, 285, 11, false, 0, F("CAN3 FD"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 285, rEndX, 315, 11, false, 0, "-", ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rEndX, 105, 470, 135, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rEndX, 165, 470, 195, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rEndX, 225, 470, 255, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rEndX, 285, 470, 315, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
}