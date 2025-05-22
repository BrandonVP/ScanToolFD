/*
 ===========================================================================
 Name        : CANBusCapture.cpp
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus capture functions
 ===========================================================================
 */

#define _CANBUSCAPTURE_C
#include <SD.h>
#include <TimeLib.h>
#include <algorithm>
#include <stdio.h> 
#include <string.h>
#include "CANBusCapture.h"
#include "KeyInput.h"
#include "icons.h"
#include "App.h"
#include "appManager.h"
#include "baudRate.h"

extern App app;

 // States for serial transfer
#define START_BYTE              (0)
#define PACKET_LENGTH           (1)
#define CAN_BUS_ID1             (2)
#define CAN_BUS_ID2             (3)
#define CAN_BUS_LENGTH          (4)
#define CAN_BUS_DATA            (5)
#define END_BYTE                (6)
// Serial transfer
#define STARTING_BYTE           (0xFE)
#define ENDING_BYTE             (0xFD)
#define PACKET_SIZE             (0x0A)

// TODO: What is going on with all these names? Move names to SDCard class and make set/get functions
static char filename[8] = { 'F', 'i', 'l', 'e', 'n', 'a', 'm', 'e' };
static char newFilename[8];
static char SDfilename[50];


typedef char MyArray[20][13];
char CAPTURE_fileList[20][13];
static uint8_t CAPTURE_state = 0;
static uint8_t CAPTURE_index = 0;
static uint32_t CAPTURE_messageNum = 0;
static char CAPTURE_printString[16][64];
bool CAPTURE_isRunning = false;

uint32_t CAPTURE_hexTotal = 0;
extern File myFile;

// Assign app pointer
void CAPTURE_setPtrs(Icons* icons)
{
	CAPTURE_iconPtr = icons;
}

//
void CAPTURE_clearLocalVar()
{
	CAPTURE_state = 0;
	CAPTURE_index = 0;
	CAPTURE_hexTotal = 0;
}

//
uint8_t CAPTURE_createCaptureBtns()
{

	userInterfaceButton[0].setButton( 10,  55, 170,  70,  0, false, 20, F("Input"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[0].setClickable(false);
	userInterfaceButton[1].setButton(175,  55, 335,  70,  0, false, 20, F("Output"),      ALIGN_CENTER, themeBackground, themeBackground, menuBackground, menuBtnTextColor);
	userInterfaceButton[1].setClickable(false);
	userInterfaceButton[2].setButton( 10,  75, 170, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, BlackBtnColor, menuBackground, menuBackground);
	userInterfaceButton[2].setClickable(false);
	userInterfaceButton[3].setButton(175,  75, 335, 315,  0, false, 0, F(""),            ALIGN_CENTER, menuBackground, BlackBtnColor, menuBackground, menuBackground);
	userInterfaceButton[3].setClickable(false);

	userInterfaceButton[BTN_config_input_C1]       .setButton( 10,  85, 170, 120, BTN_config_input_C1, false, 0, F("CAN1"),     ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_C2]       .setButton( 10, 125, 170, 160, BTN_config_input_C2, false, 0, F("CAN2"),        ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_C3]       .setButton( 10, 165, 170, 200, BTN_config_input_C3, false, 0, F("CAN3 FD"),        ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_Wireless] .setButton( 10, 205, 170, 240, BTN_config_input_Wireless, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_input_C2tx]     .setButton( 10, 245, 170, 280, BTN_config_input_C2tx, false, 0, F("C2 Out Only"), ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);

	userInterfaceButton[BTN_config_output_LCD]     .setButton(175, 85, 335, 120, BTN_config_output_LCD, false, 0, F("LCD"),         ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_Serial]  .setButton(175, 125, 335, 160, BTN_config_output_Serial, false, 0, F(" Serial"),      ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_SDCard]  .setButton(175, 165, 335, 200, BTN_config_output_SDCard, false, 0, F("SD Card"),     ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_output_Wireless].setButton(175, 205, 335, 240, BTN_config_output_Wireless, false, 0, F("Wireless"),    ALIGN_CENTER, themeBackground, BlackBtnColor, menuBtnColor, menuBtnText);

	userInterfaceButton[BTN_config_state_Start]    .setButton(340, 230, 475, 270, BTN_config_state_Start, true, 0, F("START"),        ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
	userInterfaceButton[BTN_config_state_Start].setClickable(false);


	if (CAPTURE_isRunning)
	{
		userInterfaceButton[BTN_config_state_Stop].setButton(340, 275, 475, 315, BTN_config_state_Stop, true, 0, F("STOP"), ALIGN_CENTER, OrangeBtnColor, BlackBtnColor, OrangeBtnColor, menuBtnText);
	}
	else
	{
		userInterfaceButton[BTN_config_state_Stop].setButton(340, 275, 475, 315, BTN_config_state_Stop, true, 0, F("STOP"), ALIGN_CENTER, themeBackground, BlackBtnColor, OrangeBtnColor, menuBtnText);
		userInterfaceButton[BTN_config_state_Stop].setClickable(false);
	}

	userInterfaceButton[BTN_config_state_Filename].setButton(340, 75, 475, 110, BTN_config_state_Filename, true, 0, "", ALIGN_CENTER, themeBackground, themeBackground, menuBtnColor, themeBackground); // filename
	userInterfaceButton[BTN_config_state_Filename].setClickable(false);

	userInterfaceButton[BTN_config_state_Filename_Accept].setButton(340, 115, 475, 150, BTN_config_state_Filename_Accept, true, 0, F("Accept"), ALIGN_CENTER, themeBackground, themeBackground, menuBtnColor, themeBackground); 
	userInterfaceButton[BTN_config_state_Filename_Accept].setClickable(false);

	// Highlight current selected
	//(CAPTURE_input_config >= BTN_config_input_C1) ? userInterfaceButton[CAPTURE_input_config].setBgColor(menuBtnColor) : 0;
	if (CAPTURE_input_config >= BTN_config_input_C1)
	{
		userInterfaceButton[CAPTURE_input_config].setBgColor(menuBtnColor);
	}
	//(CAPTURE_output_config >= BTN_config_output_LCD) ? userInterfaceButton[CAPTURE_output_config].setBgColor(menuBtnColor) : false;
	if (CAPTURE_output_config >= BTN_config_output_LCD)
	{
		userInterfaceButton[CAPTURE_output_config].setBgColor(menuBtnColor);
	}

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
void CAPTURE_enableDisableConfigBtn(bool isEnabled)
{
	for (uint8_t i = BTN_config_input_C1; i <= BTN_config_state_Start; i++)
	{
		userInterfaceButton[i].setClickable(isEnabled);
	}
}

//
void CAPTURE_processSerialCapture()
{
	char buffer[128];

	if ((CAPTURE_isRunning) && (CAPTURE_output_config == BTN_config_output_Serial))
	{
		switch (CAPTURE_input_config)
		{
		case BTN_config_input_C1: // Start
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				Serial.print(buffer);
			}
			break;
		case BTN_config_input_C2:
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				Serial.print(buffer);
			}
			break;
		case BTN_config_input_C3:
			if (can3Buffer.stack_size_cb() > 0)
			{
				CAN_Frame_FD msg;
				can3Buffer.pop_cb(&msg);

				sprintf(buffer, "%8ld    %9ld    %04lX   %d   ", ++CAPTURE_messageNum, millis(), msg.id, msg.length);
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
				sprintf(buffer, "%8ld    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
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

void CAPTURE_processSDCapture(int userInput)
{
	char buffer[80];
	//static bool isFilename = false;
	//const uint8_t MAX_INPUT_SIZE = 4;
	uint8_t change; // For keypad controller output

	if ((CAPTURE_state == BTN_config_state_keyInput) && (CAPTURE_output_config == BTN_config_output_SDCard))
	{
		change = KEYINPUT_keyboardController(CAPTURE_index, newFilename);
		if (change == KEY_CHANGE)
		{
			display.useFrameBuffer(false);
			GUI_drawRoundBtn(340, 75, 475, 110, newFilename, themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
			display.useFrameBuffer(true);
			GUI_drawRoundBtn(340, 75, 475, 110, newFilename, themeBackground, frameBorder, menuBtnText, ALIGN_CENTER, 0);
		}
		if (userInput == BTN_config_state_Filename_Accept)
		{
			//memcpy(filename, newFilename, 8);

			display.useFrameBuffer(true);
			userInterfaceButton[BTN_config_state_Filename].setText(newFilename);
			graphicLoaderState = 0;
			GUI_drawRoundBtn(0, 163, 479, 319, F(""), themeBackground, themeBackground, themeBackground, ALIGN_CENTER, 0);
			while (GUI_drawPage(userInterfaceButton, graphicLoaderState, BTN_config_output_Wireless + 1));
			CAPTURE_enableDisableConfigBtn(true);
			CAPTURE_activateStartBtn();
			CAPTURE_deactivateStopBtn();
			display.updateScreen();

			SD.mkdir("canlog");

			for (uint8_t i = 0; i < 50; i++)
			{
				SDfilename[i] = '\0';
			}

			memcpy(SDfilename, "canlog/", 7);
			strcat(SDfilename, newFilename);
			strcat(SDfilename, ".txt");


			//Serial.printf("SDfilename: %s\n", SDfilename);
			//Serial.printf("filename: %s\n", filename);
			//Serial.printf("newFilename: %s\n", newFilename);
			//Serial.println(userInterfaceButton[BTN_config_state_Filename].getBtnText());


			CAPTURE_state = BTN_config_state_Filename_Accept;
		}
	}

	if ((CAPTURE_state == BTN_config_state_Start) && (CAPTURE_output_config == BTN_config_output_SDCard))
	{
		myFile = SD.open(SDfilename, FILE_WRITE);

		//if (!myFile)
		//{
		//	Serial.println("Failed to open");
		//	return;
		//}
		

		switch (CAPTURE_input_config)
		{
		case BTN_config_input_C1: // Start
			if (can1Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can1Buffer.pop_cb(&msg);
				sprintf(buffer, "%8lu   %9ld   %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.println(buffer);
			}
			break;
		case BTN_config_input_C2:
			if (can2Buffer.stack_size_cb() > 0)
			{
				CAN_Frame msg;
				can2Buffer.pop_cb(&msg);
				sprintf(buffer, "%8lu    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
				myFile.println(buffer);
			}
			break;
		case BTN_config_input_C3:
			if (can3Buffer.stack_size_cb() > 0)
			{
				CAN_Frame_FD msg;
				can3Buffer.pop_cb(&msg);
				sprintf(buffer, "%8lu    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
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
				sprintf(buffer, "%8lu    %9ld    %04lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", ++CAPTURE_messageNum, millis(), msg.id, msg.length, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
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
	GUI_drawRoundBtn(340, 230, 475, 270, F("START"), OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_deactivateStartBtn()
{
	userInterfaceButton[BTN_config_state_Start].setClickable(false);
	GUI_drawRoundBtn(340, 230, 475, 270, F("START"), themeBackground, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_activateStopBtn()
{
	userInterfaceButton[BTN_config_state_Stop].setClickable(true);
	GUI_drawRoundBtn(340, 275, 475, 315, F("STOP"), OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
}

//
void CAPTURE_deactivateStopBtn()
{
	userInterfaceButton[BTN_config_state_Stop].setClickable(false);
	GUI_drawRoundBtn(340, 275, 475, 315, F("STOP"), themeBackground, BlackBtnColor, menuBtnText, ALIGN_CENTER, 0);
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
		if ((CAPTURE_input_config > 0) && (CAPTURE_output_config > 0) && (!CAPTURE_isRunning))
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
#if 0
			// Check for sd button status
			if (BTN_config_output_SDCard == userInput)
			{
				CAPTURE_printFilenameBtns();
			}
			else
			{
				CAPTURE_removeFilenameBtn();
			}
#endif
			// Assign new output
			CAPTURE_output_config = userInput;
		}

		// Check for start/stop button highlights
		if ((CAPTURE_input_config > 0) && (CAPTURE_output_config > 0) && (!CAPTURE_isRunning))
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
		CAPTURE_isRunning = true;
		CAPTURE_messageNum = 0;

		can1Buffer.clear_buffer_cb();
		can2Buffer.clear_buffer_cb();
		can3Buffer.clear_buffer_cb();

		if (CAPTURE_input_config == BTN_config_input_C3)
		{
			setCANBusFD(CLK_60MHz, BAUD_getBaudRateFD());
		}
		else
		{
			setCANBusFD(CLK_24MHz, BAUD_getBaudRateFD());
		}

		if (CAPTURE_output_config == BTN_config_output_LCD)
		{
			CAPTURE_LCD_clear();
			app.newApp((APP_labels)APP_CAPTURE_LCD);
			return;
		}

		if ((CAPTURE_output_config >= BTN_config_output_Serial) && (CAPTURE_output_config <= BTN_config_output_Wireless))
		{
			display.useFrameBuffer(true);
			display.fillCircle(51, 21, 10, ILI9488_BLACK);
			display.fillCircle(51, 21, 7, ILI9488_RED);
		}

		CAPTURE_enableDisableConfigBtn(false);
		CAPTURE_deactivateStartBtn();
		CAPTURE_activateStopBtn();

		display.updateScreen();
	}

	// -----------STOP-----------
	if (userInput == BTN_config_state_Stop)
	{
		display.useFrameBuffer(true);
		CAPTURE_isRunning = false;
		CAPTURE_enableDisableConfigBtn(true);

		CAPTURE_activateStartBtn();
		CAPTURE_deactivateStopBtn();

		display.fillCircle(50, 20, 12, menuBackground);

		CAPTURE_state = BTN_config_state_Stop;
		display.updateScreen();
	}
	
	// -----------PRINT KEYBOARD-----------
	if (userInput == BTN_config_state_Filename)
	{
		//display.useFrameBuffer(true);
		CAPTURE_enableDisableConfigBtn(false);
		graphicLoaderState = 1;
		keyPadButtons = KEYINPUT_createKeyboardButtons();
		//Serial.println(keyPadButtons);
		while (GUI_drawPage(userKeyButtons, graphicLoaderState, keyPadButtons));
		CAPTURE_state = BTN_config_state_keyInput;
		display.updateScreen();
	}
	// -----------PROCESS-----------
	//CAPTURE_processSDCapture(userInput);
}

//
void CAPTURE_LCD_clear()
{
	CAPTURE_index = 0;
	for (uint8_t i = 0; i < 16; i++)
	{
		for (uint8_t j = 0; j < 64; j++)
		{
			CAPTURE_printString[i][j] = '\0';
		}
	}
}

// Print RF packets to the LCD
void CAPTURE_LCD_Print(uint32_t id, uint8_t length, uint8_t * data)
{
	int timerin = millis();

	sprintf(CAPTURE_printString[CAPTURE_index++], "%03lX   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", id, length, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

	//char printString[128];
	display.setTextColor(menuBtnText);
	display.setFont(Michroma_9);
	uint8_t temp = 0;
	display.useFrameBuffer(false);
	if (CAPTURE_index == 0)
	{
		temp = 15;
	}
	else
	{
		temp = CAPTURE_index - 1;
	}
	if (CAPTURE_index > 15)
	{
		CAPTURE_index = 0;
	}

	for (uint16_t i = 0, j = 295, k = temp; i < 15; i++, j = j - 17)
	{
		if (j == 295)
		{
			GUI_drawSquareBtn(2, j, 410, 319, "", menuBackground, BlackBtnColor, menuBtnTextColor, ALIGN_LEFT);
			display.drawString(CAPTURE_printString[k], 10, j + 6);
		}
		else
		{
			display.fillRect(7, j, 403, 15, themeBackground);
			display.drawString(CAPTURE_printString[k], 10, j);
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
	
	//display.updateScreen();
	display.setFont(Michroma_11);
	display.useFrameBuffer(true);

	Serial.println(millis() - timerin);
}

uint8_t WIFI_state = 0;
uint8_t packetIndex = 0;
uint32_t wifiID = 0;
uint8_t wifiLength = 0;
uint8_t wifiData[8] = {0};

void CAPTURE_processWiFi()
{
	/*if (Serial2.available())
	{
		Serial.println(Serial2.read(), 16);
	}*/
	
	if (Serial2.available())
	{
		uint8_t recByte = Serial2.read();
		//Serial.println(recByte, 16);
		
		switch (WIFI_state)
		{
		case START_BYTE:
			if (recByte == STARTING_BYTE)
			{
				WIFI_state = PACKET_LENGTH;
				return;
			}
			break;
		case PACKET_LENGTH:
			WIFI_state = CAN_BUS_ID1;
			if (recByte == PACKET_SIZE)
			{
				packetIndex = 0;
				return;
			}
			else
			{
				// Bad packet
				WIFI_state = START_BYTE;
			}
			break;
		case CAN_BUS_ID1:
			wifiID = recByte;
			WIFI_state = CAN_BUS_ID2;
			break;
		case CAN_BUS_ID2:
			wifiID += (recByte << 8);
			WIFI_state = CAN_BUS_LENGTH;
			break;
		case CAN_BUS_LENGTH:
			wifiLength = recByte;
			WIFI_state = CAN_BUS_DATA;
			break;
		case CAN_BUS_DATA:
			wifiData[packetIndex] = recByte;
			packetIndex++;
			if (packetIndex == PACKET_SIZE - 2)
			{
				WIFI_state = END_BYTE;
			}
			break;
		case END_BYTE:
			if (recByte == ENDING_BYTE)
			{
				// Successful packet
				WIFI_state = START_BYTE;

				
				CAPTURE_LCD_Print(wifiID, wifiLength, wifiData);


			}
			else
			{
				// packet failed restart
				WIFI_state = START_BYTE;
			}
			break;
		}
		
	}
	
}

// 
void CAPTURE_LCD_scan(int userInput)
{
	if (userInput == BTN_capture_start)
	{
		can1Buffer.clear_buffer_cb();
		can2Buffer.clear_buffer_cb();
		can3Buffer.clear_buffer_cb();
		CAPTURE_isRunning = true;
	}
	else if (userInput == BTN_capture_stop)
	{
		CAPTURE_isRunning = false;
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
		app.newApp((APP_labels)APP_CAPTURE);
	}

	if (CAPTURE_isRunning)
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
			CAPTURE_processWiFi();
			
		
			
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

//
bool CAPTURE_isCaptureFile(const char* filename) {
	int8_t len = strlen(filename);
	char copy[32];
	memcpy(copy, copy, strlen(filename));
	bool result = false;
	if (strstr(strlwr(copy + (len - 4)), ".txt"))
	{
		result = true;
	}
	return result;
}

//
uint8_t printDirectory(File dir, MyArray& list)
{
	const String str1 = "CANLOG";
	uint8_t count = 0;
	
	while (true)
	{
		File entry = dir.openNextFile();
		//Serial.printf("Count: %d   Name: %s   is?: %d\n", count, entry.name(), CAPTURE_isCaptureFile(entry.name()));
		//Serial.println();
		if (!entry || count > 9)
		{
			break;
		}
		if (CAPTURE_isCaptureFile(entry.name()) && !entry.isDirectory())
		{
			sprintf(list[count], "%s", entry.name());
			count++;
		}
		if (entry.isDirectory()) {
			printDirectory(entry, list);
		}
		entry.close();
	}
	return count;
}

//
uint32_t fileSize(char* filename)
{
	Serial.printf("filename: %s\n", filename);
	char temp[40];
	strcpy(temp, "canlog/");
	strcat(temp, filename);
	Serial.printf("temp: %s\n", temp);
	File myFile = SD.open(filename, FILE_READ);
	//File myFile = SD.open(temp, FILE_READ);
	if (myFile)
	{
		Serial.printf("Opened\n");
	}
	else
	{
		Serial.printf("Failed\n");
	}
	uint32_t size = myFile.size();
	DateTimeFields test;
	//myFile.setCreateTime();
	myFile.getCreateTime(test);
	Serial.printf("FileSize: %d   %d\n", size, test.year);
	return size;
}

// Draw playable filelist
uint8_t CAPTURE_drawCANLogScroll()
{
	// Scan card for .txt files
	/*
	File root1 = SD.open("/");
	if (root1)
	{
		printDirectory(root1, CAPTURE_fileList);
		//uint8_t files = printDirectory(root1, fileList);
		root1.close();
	}
	else
	{
		return 0;
	}
	*/

	// Starting y location for list
	uint16_t y = 78;

	CAPTURE_index = 0;

	GUI_drawSquareBtn(10, y - 25, 140, y, "Filename", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(140, y - 25, 260, y, "Size", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(260, y - 25, 420, y, "Date", frameBorder, BlackBtnColor, menuBtnText, ALIGN_CENTER);

	GUI_drawSquareBtn(425, 55, 475, 180, "", menuBackground, frameBorder, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(425, 185, 475, 310, "", menuBackground, frameBorder, menuBtnText, ALIGN_CENTER);

	CAPTURE_iconPtr->printIcon(433, 100, upArrow_bits, 32, 4, menuBtnTextColor, 1);
	CAPTURE_iconPtr->printIcon(433, 230, downArrow_bits, 32, 4, menuBtnTextColor, 1);

	GUI_drawSquareBtn(10, 280, 105, 318, "View", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(105, 280, 210, 318, "Play", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(210, 280, 315, 318, "Split", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
	GUI_drawSquareBtn(315, 280, 420, 318, "Delete", OrangeBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);

	// Draw the scroll window
	for (uint8_t i = 0; i < 6; i++)
	{
		if ((CAPTURE_index + i < 20))
		{
			if (CAPTURE_fileList[(CAPTURE_index + i)][0] != '\0')
			{
				// Copy filename to make edits
				char fullFileName[13];
				strcpy(fullFileName, CAPTURE_fileList[(CAPTURE_index + i)]);

				// Remove.txt from copied filename
				char editedFilename[13];
				char* remove = strtok(fullFileName, ".");
				strcpy(editedFilename, remove);

				// Open file
				char directory[40];
				strcpy(directory, "canlog/");
				strcat(directory, CAPTURE_fileList[(CAPTURE_index + i)]);
				File myFile = SD.open(directory, FILE_READ);

				// Create size string 
				int fileSize = myFile.size() / 1024;
				char fileSizeChar[10];
				itoa(fileSize, fileSizeChar, 10);
				strcat(fileSizeChar, "kb");

				//DateTimeFields currentTime;
				//myFile.getCreateTime(currentTime);

				//Serial.println(directory);
				//Serial.printf("FileSize: %d   %d\n", fileSize, myFile);

				GUI_drawSquareBtn(10, y, 140, y + 35, editedFilename, menuBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
				GUI_drawSquareBtn(140, y, 260, y + 35, fileSizeChar, menuBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
				GUI_drawSquareBtn(260, y, 420, y + 35, "", menuBtnColor, BlackBtnColor, menuBtnText, ALIGN_CENTER);
			}
			else
			{
				//char temp[13];
				//sprintf(temp, "%s", fileList[(g_var8[POS0] + i)]);
				GUI_drawSquareBtn(10, y, 420, y + 35, "", menuBackground, BlackBtnColor, menuBtnText, ALIGN_LEFT);
			}
		}
		else
		{
			GUI_drawSquareBtn(10, y, 300, y + 35, "", menuBackground, BlackBtnColor, menuBtnText, ALIGN_LEFT);
		}
		y = y + 33;
	}
	display.updateScreen();
	return 0;
}
