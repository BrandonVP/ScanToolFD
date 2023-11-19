/*
 ===========================================================================
 Name        : CANBusCapture.cpp
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus capture functions
 ===========================================================================
 */

#define _CANBUSCAPTURE_C
#include "CANBusCapture.h"
#include "KeyInput.h"
#include <SD.h>

static uint8_t CAPTURE_state = 0;
static int printIndex = 0;
static char printString[16][64];
static bool isCaptureRunning = false;
static uint32_t messageNum = 0;
static uint8_t inputIndex = 0;
static char *filename = "Filename";
static char newFilename[8] = "";

void CAPTURE_clearLocalVar()
{
	CAPTURE_state = 0;
}

//
uint8_t CAPTURE_createMenuBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(45, 75, 230, 125, APP_CAPTURE, true, 10, F("Capture"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 135, 230, 185, APP_FILES, true, 10, F("Files"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 195, 230, 245, APP_FILTER_MASK, true, 10, F("Filter Mask"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	//userInterfaceButton[btnPos++].setButton(45, 260, 220, 300, 0, true, F(""), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(250, 75, 435, 125, APP_SEND, true, 10, F("Send"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(250, 135, 435, 185, APP_BAUD_RATE, true, 10, F("Baud"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos].setButton(250, 195, 435, 245, 0, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setClickable(false);
	//userInterfaceButton[btnPos++].setButton(270, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
	return btnPos;
}

//
uint8_t CAPTURE_createCaptureBtns()
{
	userInterfaceButton[0].setButton( 10,  55, 170,  70,  0, true, 20, F("Input"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[0].setClickable(false);
	userInterfaceButton[1].setButton(175,  55, 335,  70,  0, true, 20, F("Output"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[1].setClickable(false);
	userInterfaceButton[2].setButton( 10,  75, 170, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[2].setClickable(false);
	userInterfaceButton[3].setButton(175,  75, 335, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[3].setClickable(false);
	userInterfaceButton[BTN_config_input_C1].setButton( 10,  75, 170, 110, BTN_config_input_C1, false, 0, F("CAN1"),     ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_C2].setButton( 10, 115, 170, 150, BTN_config_input_C2, false, 0, F("CAN2"),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_C3].setButton( 10, 155, 170, 190, BTN_config_input_C3, false, 0, F("CAN3 FD"),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_Wireless].setButton( 10, 195, 170, 230, BTN_config_input_Wireless, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_C2tx].setButton( 10, 235, 170, 270, BTN_config_input_C2tx, false, 0, F("C2 Out Only"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_LCD].setButton(175,  75, 335, 110, BTN_config_output_LCD, false, 0, F("LCD"),         ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_Serial].setButton(175, 115, 335, 150, BTN_config_output_Serial, false, 0, F(" Serial"),      ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_SDCard].setButton(175, 155, 335, 190, BTN_config_output_SDCard, false, 0, F("SD Card"),     ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_Wireless].setButton(175, 195, 335, 230, BTN_config_output_Wireless, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_state_Start].setButton(340, 230, 475, 270, BTN_config_state_Start, true, 0, F("START"),        ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_state_Start].setClickable(false);
	userInterfaceButton[BTN_config_state_Stop].setButton(340, 275, 475, 315, BTN_config_state_Stop, true, 0, F("STOP"),         ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_state_Stop].setClickable(false);
	userInterfaceButton[BTN_config_state_Filename].setButton(340, 75, 475, 110, BTN_config_state_Filename, true, 0, filename, ALIGN_CENTER, themeBackground, themeBackground, menuBtnColor, themeBackground);
	userInterfaceButton[BTN_config_state_Filename].setClickable(false);
	userInterfaceButton[BTN_config_state_Filename_Accept].setButton(340, 115, 475, 150, BTN_config_state_Filename_Accept, true, 0, F("Accept"), ALIGN_CENTER, themeBackground, themeBackground, menuBtnColor, themeBackground);
	userInterfaceButton[BTN_config_state_Filename].setClickable(false);
	return BTN_config_button_count;
}

//
uint8_t CAPTURE_createLCDBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos].setButton(440, 50, 480, 320, 0, false, 0, F(""), ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos].setButton(415, 60, 479, 105,  BTN_capture_start, false, 0, F(" Start"), ALIGN_LEFT, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setTextSize(9);
	userInterfaceButton[btnPos].setButton(415, 110, 479, 155, BTN_capture_stop, false, 0, F("Stop"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setTextSize(9);
	userInterfaceButton[btnPos].setButton(415, 160, 479, 205, BTN_capture_clear, false, 0, F(" Clear"), ALIGN_LEFT, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setTextSize(9);
	userInterfaceButton[btnPos].setButton(415, 210, 479, 255, BTN_capture_config, false, 0, F("Config"), ALIGN_LEFT, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setTextSize(9);
	userInterfaceButton[btnPos].setButton(415, 260, 479, 305, BTN_capture_back, false, 0, F("Back"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setTextSize(9);
	return btnPos;
}

//
uint8_t CAPTURE_createBaudBtns()
{
	uint8_t btnPos = 0;
	uint16_t lStartX = 10;
	uint16_t lEndX = 180;
	uint16_t rStartX = 190;
	uint16_t rEndX = 380;
	userInterfaceButton[btnPos].setButton(lStartX, 55, lEndX, 70, 0, true, 20, F("Baud"), ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos].setButton(lStartX, 75, lEndX, 315, 0, false, 0, F(""), ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos++].setButton(lStartX, 75, lEndX, 105, 4, false, 0, F("5M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 105, lEndX, 135, 5, false, 0, F("2M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 135, lEndX, 165, 6, false, 0, F("1M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 165, lEndX, 195, 7, false, 0, F("800K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 195, lEndX, 225, 8, false, 0, F("500K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 225, lEndX, 255, 8, false, 0, F("250K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 255, lEndX, 285, 8, false, 0, F("125K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(lStartX, 285, lEndX, 315, 8, false, 0, F("100K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos].setButton(rStartX, 55, 380, 70, 0, true, 20, F("CAN Bus Port"), ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[btnPos++].setClickable(false);
	userInterfaceButton[btnPos++].setButton(rStartX, 75, rEndX, 105, 9, false, 0, F("CAN1"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 105, rEndX, 135, 9, false, 0, Can1.getBaudRate(), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rStartX, 135, rEndX, 165, 10, false, 0, F("CAN2"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 165, rEndX, 195, 10, false, 0, Can2.getBaudRate(), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rStartX, 195, rEndX, 225, 11, false, 0, F("CAN3 FD"), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rStartX, 225, rEndX, 255, 11, false, 0, "-", ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);

	userInterfaceButton[btnPos++].setButton(rEndX, 105, 470, 135, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rEndX, 165, 470, 195, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(rEndX, 225, 470, 255, 12, false, 0, F("Set"), ALIGN_CENTER, OrangeBtnColor, frameBorder, BlackBtnColor, menuBtnText);
	return btnPos;
}

//
void CAPTURE_enableDisableConfigBtn(bool isEnabled)
{
	for (uint8_t i = BTN_config_input_C1; i <= BTN_config_state_Start; i++)
	{
		userInterfaceButton[i].setClickable(isEnabled);
	}
}

void CAPTURE_processSerialCapture()
{
	char buffer[128];

	if ((CAPTURE_state == BTN_config_state_Start) && (CAPTURE_output_config == BTN_config_output_Serial))
	{
		switch (CAPTURE_input_config)
		{
		case BTN_config_input_C1: // Start
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				Serial.print(buffer);
			}
			break;
		case BTN_config_input_C2:
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				Serial.print(buffer);
			}
			break;
		case BTN_config_input_C3:
			if (can3Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can3Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				Serial.print(buffer);
			}
			break;
		case BTN_config_input_Wireless:

			break;
		case BTN_config_input_C2tx:

			break;
		}
	}
}

extern File myFile;
void CAPTURE_processSDCapture(int userInput)
{
	char buffer[80];
	static bool isFilename = false;
	const uint8_t MAX_INPUT_SIZE = 4;
	uint8_t change; // For keypad controller output

	if ((CAPTURE_state == BTN_config_state_keyInput) && (CAPTURE_output_config == BTN_config_output_SDCard))
	{
		change = KEYINPUT_keyboardController(inputIndex, newFilename);
		if (change == KEY_CHANGE)
		{
			GUI_drawRoundBtn(340, 75, 475, 110, newFilename, themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
		}
		if (userInput == BTN_config_state_Filename_Accept)
		{
			CAPTURE_state = BTN_config_state_Filename_Accept;
			graphicLoaderState = 0;
			GUI_drawRoundBtn(0, 163, 479, 319, F(""), themeBackground, themeBackground, themeBackground, ALIGN_CENTER, 0);
			while (GUI_drawPage(userInterfaceButton, graphicLoaderState, BTN_config_output_Wireless+1));
			CAPTURE_enableDisableConfigBtn(true);
			CAPTURE_activateStartBtn();
			CAPTURE_deactivateStopBtn();
			display.updateScreen();
		}
	}

	if ((CAPTURE_state == BTN_config_state_Start) && (CAPTURE_output_config == BTN_config_output_SDCard))
	{
		myFile = SD.open("log.txt", FILE_WRITE);
		if (!myFile)
		{
			Serial.println("returning");
			return;
		}

		switch (CAPTURE_input_config)
		{
		case BTN_config_input_C1: // Start
			if (can1Buffer.stack_size_cb() > 0)
			{
				Serial.println("in");
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.write(buffer, 70);
			}
			break;
		case BTN_config_input_C2:
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.write(buffer, 70);
			}
			break;
		case BTN_config_input_C3:
			if (can3Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can3Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.write(buffer, 70);
			}
			break;
		case BTN_config_input_Wireless:

			break;
		case BTN_config_input_C2tx:

			break;
		}
	}
	myFile.close();
}

//
void CAPTURE_processWirelessCapture()
{
	if ((CAPTURE_state == BTN_config_state_Start) && (CAPTURE_output_config == BTN_config_output_Wireless))
	{

	}
}

//
void CAPTURE_printFilenameBtns()
{
	userInterfaceButton[BTN_config_state_Filename].setClickable(true);
	userInterfaceButton[BTN_config_state_Filename_Accept].setClickable(true);
	GUI_drawRoundBtn(340, 75, 475, 110, filename, themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
	GUI_drawRoundBtn(340, 115, 475, 150, F("Accept"), themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_removeFilenameBtn()
{
	userInterfaceButton[BTN_config_state_Filename].setClickable(false);
	userInterfaceButton[BTN_config_state_Filename_Accept].setClickable(false);
	GUI_drawRoundBtn(340, 75, 475, 110, filename, themeBackground, themeBackground, themeBackground, ALIGN_CENTER, 0);
	GUI_drawRoundBtn(340, 115, 475, 150, F("Accept"), themeBackground, themeBackground, themeBackground, ALIGN_CENTER, 0);
}

//
void CAPTURE_activateStartBtn()
{
	userInterfaceButton[BTN_config_state_Start].setClickable(true);
	GUI_drawRoundBtn(340, 230, 475, 270, F("START"), OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_deactivateStartBtn()
{
	userInterfaceButton[BTN_config_state_Start].setClickable(false);
	GUI_drawRoundBtn(340, 230, 475, 270, F("START"), themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_activateStopBtn()
{
	userInterfaceButton[BTN_config_state_Stop].setClickable(true);
	GUI_drawRoundBtn(340, 275, 475, 315, F("STOP"), OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_deactivateStopBtn()
{
	userInterfaceButton[BTN_config_state_Stop].setClickable(false);
	GUI_drawRoundBtn(340, 275, 475, 315, F("STOP"), themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_captureConfig(int userInput)
{
	// -----------INPUTS-----------
	if (userInput >= BTN_config_input_C1 && (userInput <= BTN_config_input_C2tx))
	{
		// Dehighlight current selected output
		if (CAPTURE_input_config == userInput)
		{
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Set selection to none
			CAPTURE_input_config = 0;
		}
		else
		{
			// Dehighlight last selected output
			graphicLoaderState = CAPTURE_input_config + 1;
			userInterfaceButton[CAPTURE_input_config].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Highlight current selected output
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(menuBtnColor);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Assign new output
			CAPTURE_input_config = userInput;
		}

		// Check for start/stop button highlights
		if ((CAPTURE_input_config > 0) && (CAPTURE_output_config > 0))
		{
			CAPTURE_activateStartBtn();
		}
		else
		{
			CAPTURE_deactivateStartBtn();
		}

		display.updateScreen();
		return;
	}

	// -----------OUTPUTS-----------
	if (userInput >= BTN_config_output_LCD && (userInput <= BTN_config_output_Wireless))
	{
		//Serial.printf("userInput: %d   CAPTURE_input_config: %d   CAPTURE_input_config: %d \n", userInput, CAPTURE_input_config, CAPTURE_output_config);
		if (CAPTURE_output_config == userInput)
		{
			// Dehighlight last selected output
			graphicLoaderState = CAPTURE_output_config + 1;
			userInterfaceButton[CAPTURE_output_config].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Also check for deselecting sd
			if (BTN_config_output_SDCard == userInput)
			{
				CAPTURE_removeFilenameBtn();
				GUI_drawRoundBtn(340, 75, 475, 150, F(""), themeBackground, themeBackground, themeBackground, ALIGN_CENTER, 0);
			}

			// Set selection to none
			CAPTURE_output_config = 0;
		}
		else
		{
			// Dehighlight last selected output
			graphicLoaderState = CAPTURE_output_config + 1;
			userInterfaceButton[CAPTURE_output_config].setBgColor(themeBackground);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Highlight current selected output
			graphicLoaderState = userInput + 1;
			userInterfaceButton[userInput].setBgColor(menuBtnColor);
			GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

			// Check for sd button status
			if (BTN_config_output_SDCard == userInput)
			{
				CAPTURE_printFilenameBtns();
			}
			else
			{
				CAPTURE_removeFilenameBtn();
			}

			// Assign new output
			CAPTURE_output_config = userInput;
		}

		// Check for start/stop button highlights
		if ((CAPTURE_input_config > 0) && (CAPTURE_output_config > 0))
		{
			CAPTURE_activateStartBtn();
		}
		else
		{
			CAPTURE_deactivateStartBtn();
		}

		display.updateScreen();
		return;
	}

	// -----------START-----------
	if (userInput == BTN_config_state_Start)
	{
		messageNum = 0;
		if (CAPTURE_output_config == BTN_config_output_LCD)
		{
			can1Buffer.clear_buffer_cb();
			can2Buffer.clear_buffer_cb();
			can3Buffer.clear_buffer_cb();
			
			nextApp = (APP_labels)APP_CAPTURE_LCD;
			return;
		}
		CAPTURE_enableDisableConfigBtn(false);

		CAPTURE_deactivateStartBtn();
		CAPTURE_activateStopBtn();

		// These two are duplicated
		isCaptureRunning = true;
		CAPTURE_state = BTN_config_state_Start;

		display.updateScreen();
	}

	// -----------STOP-----------
	if (userInput == BTN_config_state_Stop)
	{
		isCaptureRunning = false;
		CAPTURE_enableDisableConfigBtn(true);

		CAPTURE_activateStartBtn();
		CAPTURE_deactivateStopBtn();

		CAPTURE_state = BTN_config_state_Stop;
		display.updateScreen();
	}
	
	// -----------FILENAME-----------
	if (userInput == BTN_config_state_Filename)
	{
		CAPTURE_state = BTN_config_state_Filename;
		CAPTURE_enableDisableConfigBtn(false);
		graphicLoaderState = 0;
		keyPadButtons = KEYINPUT_createKeyboardButtons();
		while (GUI_drawPage(userKeyButtons, graphicLoaderState, keyPadButtons));
		CAPTURE_state = BTN_config_state_keyInput;
		display.updateScreen();
	}

	// -----------PROCESS-----------
	CAPTURE_processSerialCapture();
	CAPTURE_processSDCapture(userInput);
	CAPTURE_processWirelessCapture();
}

// Print RF packets to the LCD
void CAPTURE_LCD_Print(uint32_t id, uint8_t length, uint8_t * data)
{
	sprintf(printString[printIndex++], "%03lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", id, length, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

	//char printString[128];
	display.setTextColor(menuBtnText);
	display.setFont(Michroma_9);
	uint8_t temp = 0;
	if (printIndex == 0)
	{
		temp = 15;
	}
	else
	{
		temp = printIndex - 1;
	}
	if (printIndex > 15)
	{
		printIndex = 0;
	}

	for (uint16_t i = 0, j = 295, k = temp; i < 15; i++, j = j - 17)
	{
		if (j == 295)
		{
			GUI_drawSquareBtn(2, j, 410, 319, "", menuBackground, BlackBtnColor, menuBtnTextColor, ALIGN_LEFT);
			display.drawString(printString[k], 10, j + 6);
		}
		else
		{
			display.fillRect(7, j, 403, 15, themeBackground);
			display.drawString(printString[k], 10, j);
		}

		if (k == 0 || k > 15)
		{
			k = 15;
		}
		else
		{
			k--;
		}
	}
	display.updateScreen();
	display.setFont(Michroma_11);
}

// 
void CAPTURE_LCD_scan(int userInput)
{
	if (userInput == BTN_capture_start)
	{
		can1Buffer.clear_buffer_cb();
		can2Buffer.clear_buffer_cb();
		can3Buffer.clear_buffer_cb();
		isCaptureRunning = true;
	}
	else if (userInput == BTN_capture_stop)
	{
		isCaptureRunning = false;
	}
	else if (userInput == BTN_capture_clear) // clear
	{
		can1Buffer.clear_buffer_cb();
		can2Buffer.clear_buffer_cb();
		can3Buffer.clear_buffer_cb();

		for (uint8_t i = 0; i < 16; i++)
		{
			for (uint8_t j = 0; j < 64; j++)
			{
				printString[i][j] = '\0';
			}
		}
		GUI_drawSquareBtn(0, 51, 408, 320, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
		printIndex = 0;
		display.updateScreen();
	}
	else if (userInput == BTN_capture_config)
	{

	}
	else if (userInput == BTN_capture_back)
	{
		nextApp = (APP_labels)APP_CAPTURE;
	}

	if (isCaptureRunning)
	{
		switch (CAPTURE_input_config)
		{
		case BTN_config_input_C1: // Start
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame message;
				can1Buffer.pop_cb(&message);
				CAPTURE_LCD_Print(message.id, message.length, message.data);
			}
			break;
		case BTN_config_input_C2:
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame message;
				can2Buffer.pop_cb(&message);
				CAPTURE_LCD_Print(message.id, message.length, message.data);
			}
			break;
		case BTN_config_input_C3: 
			if (can3Buffer.stack_size_cb() > 0)
			{
				CAN_Frame message;
				can3Buffer.pop_cb(&message);
				CAPTURE_LCD_Print(message.id, message.length, message.data);
			}
			break;
		case BTN_config_input_Wireless:

			break;
		case BTN_config_input_C2tx:

			break;
		}
	}
}
