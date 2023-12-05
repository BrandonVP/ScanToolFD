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
bool isCaptureRunning = false;
static uint32_t messageNum = 0;
static uint8_t inputIndex = 0;
static char* filename = "Filename";
//static char filename[9] = { 'F', 'i', 'l', 'e', 'n', 'a', 'm', 'e' };
static char newFilename[8];
static char SDfilename[50];
uint32_t CANBusFDBaudRate = 1000000;
static uint8_t baudInput = 0xFF;
static uint8_t baudSpeed = 0;

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
	uint16_t lStartX = 270;
	uint16_t lEndX = 460;
	uint16_t rStartX = 40;
	uint16_t rEndX = 220;
	
	userInterfaceButton[BTN_baud_label1].setButton(rStartX,  60, rEndX, 90, BTN_baud_label1, false, 0, F("CAN1"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label1].setClickable(false);
	userInterfaceButton[BTN_baud_can1]  .setButton(rStartX,  90, rEndX, 130, BTN_baud_can1, false, 0, Can1.getBaudRate(), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label2].setButton(rStartX, 130, rEndX, 160, BTN_baud_label2, false, 0, F("CAN2"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label2].setClickable(false);
	userInterfaceButton[BTN_baud_can2]  .setButton(rStartX, 160, rEndX, 200, BTN_baud_can2, false, 0, Can2.getBaudRate(), ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label3].setButton(rStartX, 200, rEndX, 230, BTN_baud_label3, false, 0, F("CAN3 FD"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_label3].setClickable(false);
	userInterfaceButton[BTN_baud_can3]  .setButton(rStartX, 230, rEndX, 270, BTN_baud_can3, false, 0, CANBusFDBaudRate, ALIGN_CENTER, menuBtnColor, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_canSet].setButton(rStartX, 270, rEndX, 315, BTN_baud_canSet, false, 0, F("Set"), ALIGN_CENTER, themeBackground, frameBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_canSet].setClickable(false);

	userInterfaceButton[BTN_baud_table].setButton(lStartX,  60, lEndX, 300, BTN_baud_table, false, BTN_baud_table, F(""), ALIGN_CENTER, menuBackground, frameBorder, menuBackground, menuBackground);
	userInterfaceButton[BTN_baud_table].setClickable(false);
	userInterfaceButton[BTN_baud_5m]   .setButton(lStartX,  60, lEndX,  92, BTN_baud_5m, false, 0, F("5M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_2m]   .setButton(lStartX,  92, lEndX, 124, BTN_baud_2m, false, 0, F("2M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_1m]   .setButton(lStartX, 124, lEndX, 156, BTN_baud_1m, false, 0, F("1M"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_800k] .setButton(lStartX, 156, lEndX, 188, BTN_baud_800k, false, 0, F("800K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_500k] .setButton(lStartX, 188, lEndX, 220, BTN_baud_500k, false, 0, F("500K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_250k] .setButton(lStartX, 220, lEndX, 252, BTN_baud_250k, false, 0, F("250K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_125k] .setButton(lStartX, 252, lEndX, 284, BTN_baud_125k, false, 0, F("125K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_baud_100k] .setButton(lStartX, 284, lEndX, 315, BTN_baud_100k, false, 0, F("100K"), ALIGN_CENTER, themeBackground, frameBorder, menuBtnColor, menuBtnText);
	
	return BTN_baud_button_count;
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

	if ((isCaptureRunning) && (CAPTURE_output_config == BTN_config_output_Serial))
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
				CAN_Frame_FD msg;
				can3Buffer.pop_cb(&msg);

				sprintf(buffer, "%8ld    %9ld    %04lX   %d   ", ++messageNum, millis(), msg.id, msg.length);
				for (int i = 0; i < msg.length; i++)
				{
					Serial.print(msg.data[i], HEX);
					Serial.print("  ");
				}
				Serial.println();
			}
			break;
		case BTN_config_input_Wireless:

			break;
		case BTN_config_input_C2tx:
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);

				CAN_message_t msgOut;
				msgOut.id = msg.id;
				msgOut.len = msg.length;
				memcpy(msgOut.buf, msg.data, msg.length);
				Can2.write(msgOut);
			}
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				Serial.print(buffer);

				CAN_message_t msgOut;
				msgOut.id = msg.id;
				msgOut.len = msg.length;
				memcpy(msgOut.buf, msg.data, msg.length);
				Can1.write(msgOut);
			}
			break;
		}
	}
}

/*
void hex2String(uint8_t value, char(&result)[N])
{
	if (((value >> 4) & 0x0f) < 10)
	{
		result[0] = (((value >> 4) & 0x0f) + 48);
	}
	else
	{
		result[0] = (((value >> 4) & 0x0f) + 55);
	}
	if ((value & 0x0f) < 10)
	{
		result[1] = (value & 0x0f) + 48;
	}
	else
	{
		result[1] = (value & 0x0f) + 55;
	}
}
*/

/*
void CAPTURE_setSDFilename(char* filename)
{
	for (uint8_t i = 0; i < 50; i++)
	{
		SDfilename[i] = '\0';
	}
	char* a1 = SDfilename;
	memcpy(a1, "canlog/", 7);
	strcat(SDfilename, filename);
	strncpy(filename, keyboardInput, 8);
	strcat(filename, ".txt");
}
*/

extern File myFile;
void CAPTURE_processSDCapture(int userInput)
{
	char buffer[70];
	//static bool isFilename = false;
	//const uint8_t MAX_INPUT_SIZE = 4;
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
			CAPTURE_state = BTN_config_state_Filename_Accept;
			display.updateScreen();

	
			//SD.mkdir("canlog");
			
			for (uint8_t i = 0; i < 50; i++)
			{
				SDfilename[i] = '\0';
			}

			char* a1 = SDfilename;
			memcpy(a1, "canlog/", 7);
			strcat(SDfilename, newFilename);
			strcat(SDfilename, ".txt");
			Serial.println(SDfilename);
		
		}
	}

	if ((CAPTURE_state == BTN_config_state_Start) && (CAPTURE_output_config == BTN_config_output_SDCard))
	{
		myFile = SD.open(SDfilename, FILE_WRITE);

		if (!myFile)
		{
			Serial.println("Failed to open");
			return;
		}
		

		switch (CAPTURE_input_config)
		{
		case BTN_config_input_C1: // Start
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);
				sprintf(buffer, "%8d   %9ld   %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.println(buffer);
			}
			break;
		case BTN_config_input_C2:
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8d    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.println(buffer);
			}
			break;
		case BTN_config_input_C3:
			if (can3Buffer.stack_size_cb() > 0)
			{
				CAN_Frame_FD msg;
				can3Buffer.pop_cb(&msg);
				sprintf(buffer, "%8d    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.println(buffer);
			}
			break;
		case BTN_config_input_Wireless:

			break;
		case BTN_config_input_C2tx:
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);
				//sprintf(buffer, "%8d   %9ld   %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				//myFile.println(buffer);

				CAN_message_t msgOut;
				msgOut.id = msg.id;
				msgOut.len = msg.length;
				memcpy(msgOut.buf, msg.data, msg.length);
				Can2.write(msgOut);
			}	
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8d    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.println(buffer);

				CAN_message_t msgOut;
				msgOut.id = msg.id;
				msgOut.len = msg.length;
				memcpy(msgOut.buf, msg.data, msg.length);
				Can1.write(msgOut);
			}
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
		// These two are duplicated
		CAPTURE_state = BTN_config_state_Start;
		isCaptureRunning = true;
		messageNum = 0;

		can1Buffer.clear_buffer_cb();
		can2Buffer.clear_buffer_cb();
		can3Buffer.clear_buffer_cb();

		if (CAPTURE_input_config == BTN_config_input_C3)
		{
			setCANBusFD(CLK_60MHz, CANBusFDBaudRate);
		}
		else
		{
			setCANBusFD(CLK_24MHz, CANBusFDBaudRate);
		}

		if (CAPTURE_output_config == BTN_config_output_LCD)
		{
			CAPTURE_LCD_clear();
			nextApp = (APP_labels)APP_CAPTURE_LCD;
			return;
		}

		CAPTURE_enableDisableConfigBtn(false);
		CAPTURE_deactivateStartBtn();
		CAPTURE_activateStopBtn();

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
	CAPTURE_processSDCapture(userInput);
}

//
void CAPTURE_LCD_clear()
{
	printIndex = 0;
	for (uint8_t i = 0; i < 16; i++)
	{
		for (uint8_t j = 0; j < 64; j++)
		{
			printString[i][j] = '\0';
		}
	}
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

		CAPTURE_LCD_clear();

		GUI_drawSquareBtn(0, 51, 408, 320, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
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
				CAN_Frame_FD message;
				can3Buffer.pop_cb(&message);
				CAPTURE_LCD_Print(message.id, message.length, message.data);
			}
			break;
		case BTN_config_input_Wireless:

			break;
		case BTN_config_input_C2tx:
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);

				CAN_message_t msgOut;
				msgOut.id = msg.id;
				msgOut.len = msg.length;
				memcpy(msgOut.buf, msg.data, msg.length);
				Can2.write(msgOut);
			}
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				CAPTURE_LCD_Print(msg.id, msg.length, msg.data);

				CAN_message_t msgOut;
				msgOut.id = msg.id;
				msgOut.len = msg.length;
				memcpy(msgOut.buf, msg.data, msg.length);
				Can1.write(msgOut);
			}
			break;
		}
	}
}



int CAPTURE_getBaudRate(capture_baud_btn baud)
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
}

void CAPTURE_Baud(int userInput)
{
		// -----------Ports-----------
		if (userInput >= BTN_baud_can1 && (userInput <= BTN_baud_can3))
		{
			// Dehighlight current selected port
			if (baudInput == userInput)
			{
				graphicLoaderState = userInput + 1;
				userInterfaceButton[userInput].setBgColor(menuBtnColor);
				GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

				// Set selection to none
				baudInput = 0xFF;
			}
			else
			{
				// Dehighlight last selected port
				graphicLoaderState = baudInput + 1;
				userInterfaceButton[baudInput].setBgColor(menuBtnColor);
				GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

				// Highlight current selected port
				graphicLoaderState = userInput + 1;
				userInterfaceButton[userInput].setBgColor(OrangeBtnColor);
				GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

				// Assign new port
				baudInput = userInput;
			}

			// Check for start/stop button highlights
			if ((baudSpeed > 0) && (baudInput < 0xFF))
			{
				userInterfaceButton[BTN_baud_canSet].setClickable(true);
				GUI_drawRoundBtn(40, 270, 220, 315, F("Set"), menuBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			}
			else
			{
				userInterfaceButton[BTN_baud_canSet].setClickable(false);
				GUI_drawRoundBtn(40, 270, 220, 315, F("Set"), themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			}

			display.updateScreen();
			return;
		}

		// -----------Speeds-----------
		if (userInput >= BTN_baud_5m && (userInput <= BTN_baud_100k))
		{
			// Dehighlight current selected baud
			if (baudSpeed == userInput)
			{
				graphicLoaderState = userInput + 1;
				userInterfaceButton[userInput].setBgColor(themeBackground);
				GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

				// Set selection to none
				baudSpeed = 0;
			}
			else
			{
				// Dehighlight last selected baud
				graphicLoaderState = baudSpeed + 1;
				userInterfaceButton[baudSpeed].setBgColor(themeBackground);
				GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

				// Highlight current selected baud
				graphicLoaderState = userInput + 1;
				userInterfaceButton[userInput].setBgColor(OrangeBtnColor);
				GUI_drawPage(userInterfaceButton, graphicLoaderState, 1);

				// Assign new baud
				baudSpeed = userInput;
			}

			// Check for start/stop button highlights
			if ((baudSpeed > 0) && (baudInput < 0xFF))
			{
				userInterfaceButton[BTN_baud_canSet].setClickable(true);
				GUI_drawRoundBtn(40, 270, 220, 315, F("Set"), menuBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			}
			else
			{
				userInterfaceButton[BTN_baud_canSet].setClickable(false);
				GUI_drawRoundBtn(40, 270, 220, 315, F("Set"), themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			}
			display.updateScreen();
			return;
		}

		// -----------Set-----------
		if (userInput == BTN_baud_canSet)
		{
			switch (baudInput)
			{
			case BTN_baud_can1:
				Can1.setBaudRate(CAPTURE_getBaudRate((capture_baud_btn)baudSpeed));
				userInterfaceButton[BTN_baud_can1].setText(Can1.getBaudRate());
				GUI_drawRoundBtn(40, 90, 220, 130, Can1.getBaudRate(), OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
				break;
			case BTN_baud_can2:
				Can2.setBaudRate(CAPTURE_getBaudRate((capture_baud_btn)baudSpeed));
				userInterfaceButton[BTN_baud_can2].setText(Can2.getBaudRate());
				GUI_drawRoundBtn(40, 160, 220, 200, Can2.getBaudRate(), OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
				break;
			case BTN_baud_can3:
				CANBusFDBaudRate = CAPTURE_getBaudRate((capture_baud_btn)baudSpeed);
				userInterfaceButton[BTN_baud_can3].setText(CANBusFDBaudRate);
				GUI_drawRoundBtn(40, 230, 220, 270, CANBusFDBaudRate, OrangeBtnColor, frameBorder, menuBtnText, ALIGN_CENTER, 0);
				setCANBusFD(CLK_24MHz, CANBusFDBaudRate);
				break;
			}
			display.updateScreen();
		}
}