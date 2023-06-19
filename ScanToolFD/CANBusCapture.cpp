/*
 Name:    CANBusCapture.cpp
 Created: 11/15/2020 8:54:18 AM
 Author:  Brandon Van Pelt
*/

#define _CANBUSCAPTURE_C
#include "CANBusCapture.h"

#define CAPTURE_START_POS 13
#define CAPTURE_STOP_POS 14

uint8_t CAPTURE_input_config = 0;
uint8_t CAPTURE_output_config = 0;

void CAPTURE_createMenuBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(55, 80, 220, 120, 1, true, F("Capture"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 80, 425, 120, 2, true, F("Send"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 140, 220, 180, 3, true, F("Files"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 140, 425, 180, 0, true, F("Baud"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 200, 220, 240, 0, true, F("Filter Mask"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Auto Baud"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 260, 220, 300, 0, true, F(""), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
}

void CAPTURE_createCaptureBtns()
{
	userInterfaceCaptureButton[0]. setButton( 10,  55, 170,  70,  0, true, 20, F("Input"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceCaptureButton[0]. setClickable(false);
	userInterfaceCaptureButton[1]. setButton(175,  55, 335,  70,  0, true, 20, F("Output"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceCaptureButton[1]. setClickable(false);
	userInterfaceCaptureButton[2]. setButton( 10,  75, 170, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceCaptureButton[2]. setClickable(false);
	userInterfaceCaptureButton[3]. setButton(175,  75, 335, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceCaptureButton[3]. setClickable(false);
	userInterfaceCaptureButton[4]. setButton( 10,  75, 170, 110,  4, false, 0, F("CAN0 FD"),     ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[5]. setButton( 10, 115, 170, 150,  5, false, 0, F("CAN1"),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[6]. setButton( 10, 155, 170, 190,  6, false, 0, F("CAN2"),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[7]. setButton( 10, 195, 170, 230,  7, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[8]. setButton( 10, 235, 170, 270,  8, false, 0, F("C2 Out Only"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[9]. setButton(175,  75, 335, 110,  9, false, 0, F("LCD"),         ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[10].setButton(175, 115, 335, 150, 10, false, 0, F("Serial"),      ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[11].setButton(175, 155, 335, 190, 11, false, 0, F("SD Card"),     ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[12].setButton(175, 195, 335, 230, 12, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[13].setButton(340, 230, 475, 270, 13, true, 0, F("START"),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[13].setClickable(false);
	userInterfaceCaptureButton[14].setButton(340, 275, 475, 315, 14, true, 0, F("STOP"),         ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceCaptureButton[14].setClickable(false);
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
			clearAppSpace();
		}
		messageNum = 0;
		CANBusOut = CAPTURE_output_config;
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
