/*
 ===========================================================================
 Name        : ScanToolFD.ino
 Author      : Brandon Van Pelt
 Created	 : 3/24/2023
 Description : Main
 ===========================================================================
 */

 /*=========================================================
	 Todo List
 ===========================================================
	//userInterfaceMenuButton[menuPosition++].setButton(5, 88, 125, 140, false, VEHTOOL_MAIN, (void*)testf);

 ===========================================================
	 End Todo List
 =========================================================*/

 /*=========================================================
	 README
 ===========================================================
	To create a new page:
	1.) Assign page in page control
	2.) Create buttons
	3.) Call function to draw buttons in assigned page switch statement hasDrawn section
	4.) Call function to monitor button press in assigned page switch statement loop section

	Swap SD SPI port in SdSpiTeensy3.cpp
	Change: m_spi = &SPI;
	To: m_spi = &SPI2;
 ===========================================================
	 End README
 =========================================================*/

#include <FlexCAN_T4.h>
 //#include <kinetis_flexcan.h>
 //#include <isotp_server.h>
 //#include <isotp.h>
 //#include <imxrt_flexcan.h>
 //#include <circular_buffer.h>

 //#include <Adafruit_GFX.h>
#include "KeyInput.h"
#include "serialTransfer.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> // Touch
#include <ILI9488_t3.h>      // Display

#include "CANBusCapture.h"
#include "CANBus.h"
#include "variableLock.h"
#include "config.h"
#include "gui.h"
#include "KeyInput.h"

#include "ili9488_t3_font_Arial.h"
#include "ili9488_t3_font_ComicSansMS.h"
#include "ili9488_t3_font_ArialBold.h"
#include "font_AwesomeF180.h"
#include "font_Michroma.h"

/*
#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_ComicSansMS.h"
#include "font_OpenSans.h"
#include "font_DroidSans.h"
#include "font_Michroma.h"
#include "font_Crystal.h"
#include "font_ChanceryItalic.h"

#include "Fonts/FreeSansOblique12pt7b.h"
*/

ILI9488_t3 display = ILI9488_t3(&SPI, TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

int oldTouchX = 0;
int oldTouchY = 0;

// For touch controls
int x, y;

// Used for page control
bool hasDrawn = false;

uint8_t CANBusOut = 0;
uint8_t CANBusIn = 0;
uint32_t messageNum = 0;
uint16_t LCDPos = 60;

// Use to load pages in pieces to prevent blocking while loading entire page
uint8_t graphicLoaderState = 0;

// *Used by background process*
//uint8_t selectedChannelOut = 0;
//uint8_t selectedSourceOut = 0;
//uint32_t updateClock = 0;
//bool isSerialOut = false;
//bool isSDOut = false;
//bool isMSGSpam = false;

// General use variables
//uint8_t state = 0;


//uint16_t buttonsOnPage = 0;

UserInterfaceClass userInterfaceButton[APP_BUTTON_SIZE];
UserInterfaceClass userInterfaceCaptureButton[CAPTURE_BUTTON_SIZE];
UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> Can3; // FD

// Simplifies getting x and y coords
bool Touch_getXY()
{
	if (ts.touched())
	{
		TS_Point p = ts.getPoint();
		x = p.y;
		y = SCREEN_HEIGHT - p.x;
		return true;
	}
	return false;
}

// Resets variables for page change
void pageTransition()
{
	hasDrawn = false;
	graphicLoaderState = 0;
	page = nextPage;
}

void createCANBusBaudBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(140, 275, 300, 315, 0, true, F("Set CAN0"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(305, 275, 475, 315, 0, true, F("Set CAN1"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(305, 60, 475, 100, 0, true, F("CAN0"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(305, 150, 475, 190, 0, true, F("CAN1"), ALIGN_CENTER);
}

void createToolBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(55, 80, 220, 120, 1, true, F("PID Scan"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 80, 425, 120, 2, true, F("PID Stream"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 140, 220, 180, 0, true, F("Get VIN"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 140, 425, 180, 0, true, F("DTC"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 200, 220, 240, 0, true, F("OBD Simulator"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Gauges"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 260, 220, 300, 0, true, F("TX Spam"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
}

void createSettingsBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(55, 80, 220, 120, 1, true, F("Memory"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 80, 425, 120, 2, true, F("About"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 140, 220, 180, 0, true, F("WiFi MAC"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 140, 425, 180, 0, true, F("Overclock"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 200, 220, 240, 0, true, F("Reset WiFi"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Set Time"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 260, 220, 300, 0, true, F("Dongle"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 260, 425, 300, 0, true, F("Reset"), ALIGN_CENTER);
}

void clearAppSpace()
{
	//GUI_drawSquareBtn(0, 51, 480, 320, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
	display.fillRect(0, 51, 480, 269, themeBackground);
}

// Manages the loading and unloading of different user Apps
void appManager()
{
	int8_t results = 0;
	error_t e = 0;

	switch (page)
	{
	case APP_CANBUS_TOOLS: /*========== CANBUS ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createMenuBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, 6))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_buttonMonitor(userInterfaceButton, 6);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case APP_CAPTURE_CONFIG: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, 15))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		CAPTURE_captureConfig();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case APP_CAPTURE_LCD: // 
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createLCDBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			display.setFont(Michroma_9);
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, 6))
			{
				break;
			}
			

			messageNum = 0;
			CANBusOut = CAPTURE_output_config;

			hasDrawn = true;
			display.setFont(Arial_12);
		}

		// Call buttons or page method
		results = GUI_subMenuButtonMonitor(userInterfaceButton, 4);
		if (results == 1)
		{
			CANBusOut = 9;
		}
		else if (results == 2)
		{
			CANBusOut = 0;
		}
		else if (results == 3)
		{
			Can2.disableFIFO();
			Can2.disableFIFOInterrupt();
			Can1.disableFIFO();
			Can1.disableFIFOInterrupt();
			GUI_drawSquareBtn(0, 51, 408, 320, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
			LCDPos = 60;
			Can2.enableFIFO();
			Can2.enableFIFOInterrupt();
			Can1.enableFIFO();
			Can1.enableFIFOInterrupt();
		}

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			if (CANBusOut == 9)
			{
				CANBusOut = 0;
			}
			LCDPos = 60;
			display.setFont(Michroma_11);
			pageTransition();
		}
		break;
	case APP_CAPTURE_FILES: // 
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createKeyboardButtons();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userKeyButtons, graphicLoaderState, 42))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_subMenuButtonMonitor(userKeyButtons, 42);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case APP_CAPTURE_BAUD: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createBaudBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, 23))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_subMenuButtonMonitor(userInterfaceButton, 20);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case APP_CAPTAPP_CAPTURE_FILTERMASKURE_BAUD: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createBaudBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, 23))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_subMenuButtonMonitor(userInterfaceButton, 20);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case APP_CAPTURE_AUTOBAUD: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createBaudBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, 23))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_subMenuButtonMonitor(userInterfaceButton, 20);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case 9: // Tools
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createToolBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, 8))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method


		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case 36: // Settings
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createSettingsBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (0)//drawPage(userInterfaceButtons, graphicLoaderState, 8))
			{
				break;
			}
			hasDrawn = true;

			/*
			display.fillScreen(ILI9488_BLACK);
			display.setTextColor(ILI9488_WHITE);  display.setTextSize(4);
			display.enableScroll();
			display.setScrollTextArea(20, 20, 120, 318);
			display.setScrollBackgroundColor(ILI9488_GREEN);


			display.setCursor(20, 20);

			display.setTextColor(ILI9488_BLACK);

			for (int i = 0; i < 5; i++) {
				display.print("  this is line ");
				display.println(i);
				delay(500);
			}

			display.setCursor(100, 20);

			for (int i = 5; i < 8; i++) {
				display.print("  this is line ");
				display.println(i);
				delay(500);
			}

			display.setCursor(140, 20);

			for (int i = 8; i < 12; i++) {
				display.print("  this is line too ");
				display.println(i);
				delay(500);
			}


			delay(4000);
			nextPage = 0;

			*/


		}

		// Call buttons or page method


		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	}
}

//
void createMenuBtns()
{
	// Create Menu Buttons
	uint8_t menuPosition = 0;

	userInterfaceMenuButton[menuPosition++].setButton(160, 0, 260, 40, CANBUS_MAIN, true, 0, F("CAN Bus"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
	userInterfaceMenuButton[menuPosition++].setButton(270, 0, 370, 40, VEHTOOL_MAIN, true, 0, F("Tools"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
	userInterfaceMenuButton[menuPosition++].setButton(375, 0, 475, 40, SETTING_MAIN, true, 0, F("Settings"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
}


void canSniff1(const CAN_message_t& msg) 
{
	if (CANBusOut == 10)
	{
		Serial.printf("%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	}
	else if ((CANBusOut == 9) && (CANBusIn == 4) && (page == APP_CAPTURE_LCD))
	{
		if (LCDPos == 60)
		{
			display.fillRect(5, 300, 10, 10, themeBackground);
			display.fillRect(5, LCDPos, 10, 10, 0x0000);
			display.fillRect(15, LCDPos, 385, 15, themeBackground);
		}
		else
		{
			display.fillRect(5, LCDPos - 15, 10, 10, themeBackground);
			display.fillRect(5, LCDPos, 10, 10, 0x0000);
			display.fillRect(15, LCDPos, 385, 15, themeBackground);
		}

		char printString[40];
		display.setTextColor(menuBtnText);
		sprintf(printString, "%03X", msg.id);
		display.drawString(printString, 15, LCDPos);
		sprintf(printString, "%d", msg.len);
		display.drawString(printString, 60, LCDPos);
		sprintf(printString, "%02X ", msg.buf[0]);
		display.drawString(printString, 90, LCDPos);
		sprintf(printString, "%02X ", msg.buf[1]);
		display.drawString(printString, 130, LCDPos);
		sprintf(printString, "%02X ", msg.buf[2]);
		display.drawString(printString, 170, LCDPos);
		sprintf(printString, "%02X ", msg.buf[3]);
		display.drawString(printString, 210, LCDPos);
		sprintf(printString, "%02X ", msg.buf[4]);
		display.drawString(printString, 250, LCDPos);
		sprintf(printString, "%02X ", msg.buf[5]);
		display.drawString(printString, 290, LCDPos);
		sprintf(printString, "%02X ", msg.buf[6]);
		display.drawString(printString, 330, LCDPos);
		sprintf(printString, "%02X ", msg.buf[7]);
		display.drawString(printString, 370, LCDPos);
		//sprintf(printString, "%03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);

		if (LCDPos < 300)
		{
			LCDPos += 15;
		}
		else
		{
			LCDPos = 60;
		}
	}
}


void canSniff2(const CAN_message_t& msg) 
{
	if (CANBusOut == 10)
	{
		Serial.printf("%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	}
	else if ((CANBusOut == 9) && (CANBusIn == 5) && (page == APP_CAPTURE_LCD))
	{
		if (LCDPos == 60)
		{
			display.fillRect(5, 300, 10, 10, themeBackground);
			display.fillRect(5, LCDPos, 10, 10, 0x0000);
			display.fillRect(15, LCDPos, 385, 15, themeBackground);
		}
		else
		{
			display.fillRect(5, LCDPos - 15, 10, 10, themeBackground);
			display.fillRect(5, LCDPos, 10, 10, 0x0000);
			display.fillRect(15, LCDPos, 385, 15, themeBackground);
		}

		char printString[40];
		display.setTextColor(menuBtnText);
		sprintf(printString, "%03X", msg.id);
		display.drawString(printString, 15, LCDPos);
		sprintf(printString, "%d", msg.len);
		display.drawString(printString, 60, LCDPos);   
		sprintf(printString, "%02X ", msg.buf[0]);
		display.drawString(printString, 90, LCDPos);
		sprintf(printString, "%02X ", msg.buf[1]);
		display.drawString(printString, 130, LCDPos);
		sprintf(printString, "%02X ", msg.buf[2]);
		display.drawString(printString, 170, LCDPos);
		sprintf(printString, "%02X ", msg.buf[3]);
		display.drawString(printString, 210, LCDPos);
		sprintf(printString, "%02X ", msg.buf[4]);
		display.drawString(printString, 250, LCDPos);
		sprintf(printString, "%02X ", msg.buf[5]);
		display.drawString(printString, 290, LCDPos);
		sprintf(printString, "%02X ", msg.buf[6]);
		display.drawString(printString, 330, LCDPos);
		sprintf(printString, "%02X ", msg.buf[7]);
		display.drawString(printString, 370, LCDPos);
		//sprintf(printString, "%03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
		
		if (LCDPos < 300)
		{
			LCDPos += 15;
		}
		else
		{
			LCDPos = 60;
		}
	}
}


void canSniff3()
//void canSniff3(const CANFD_message_t& msg)
{
	if (!CANBusIn == 6)
	{
		return;
	}

	CANFD_message_t msg;
	if (!Can3.read(msg))
	{
		return;
	}
	/*
	if (Can3.read(msg)) 
	{
		Serial.print(msg.id, HEX);
		Serial.print("   ");
		for (int i = 0; i < msg.len; i++) 
		{
			Serial.print(msg.buf[i], HEX);
			Serial.print("  ");
		}
		Serial.println();
	}
	else
	{
		return;
	}
		*/

	if (CANBusOut == 8)
	{
		Serial.print(msg.id, HEX);
		Serial.print("   ");
		for (int i = 0; i < msg.len; i++)
		{
			Serial.print(msg.buf[i], HEX);
			Serial.print("  ");
		}
		Serial.println();
	}
	else if ((CANBusOut == 9) && (page == APP_CAPTURE_LCD))
	{
		if (LCDPos == 60)
		{
			display.fillRect(5, 300, 10, 10, themeBackground);
			display.fillRect(5, LCDPos, 10, 10, 0x0000);
			display.fillRect(15, LCDPos, 385, 15, themeBackground);
		}
		else
		{
			display.fillRect(5, LCDPos - 15, 10, 10, themeBackground);
			display.fillRect(5, LCDPos, 10, 10, 0x0000);
			display.fillRect(15, LCDPos, 385, 15, themeBackground);
		}

		char printString[40];
		display.setTextColor(menuBtnText);
		sprintf(printString, "%03X", msg.id);
		display.drawString(printString, 15, LCDPos);
		sprintf(printString, "%d", msg.len);
		display.drawString(printString, 60, LCDPos);
		sprintf(printString, "%02X ", msg.buf[0]);
		display.drawString(printString, 95, LCDPos);
		sprintf(printString, "%02X ", msg.buf[1]);
		display.drawString(printString, 135, LCDPos);
		sprintf(printString, "%02X ", msg.buf[2]);
		display.drawString(printString, 175, LCDPos);
		sprintf(printString, "%02X ", msg.buf[3]);
		display.drawString(printString, 215, LCDPos);
		sprintf(printString, "%02X ", msg.buf[4]);
		display.drawString(printString, 255, LCDPos);
		sprintf(printString, "%02X ", msg.buf[5]);
		display.drawString(printString, 295, LCDPos);
		sprintf(printString, "%02X ", msg.buf[6]);
		display.drawString(printString, 335, LCDPos);
		sprintf(printString, "%02X ", msg.buf[7]);
		display.drawString(printString, 375, LCDPos);

		if (LCDPos < 300)
		{
			LCDPos += 15;
		}
		else
		{
			LCDPos = 60;
		}
	}
}


// -------------------------------------------------------------
void setup(void)
{
	Serial.begin(115200); // USB is always 12 or 480 Mbit/sec
	//Serial2.begin(115200); // ESP8266

	//SPI2.begin();
	//pinMode(2, OUTPUT);
	//digitalWrite(2, 1);
	//pinMode(3, OUTPUT);
	//pinMode(4, OUTPUT);
	//pinMode(5, OUTPUT);
	//pinMode(6, OUTPUT);
	//pinMode(7, OUTPUT);
	//pinMode(8, OUTPUT);
	//pinMode(16, OUTPUT);
	//pinMode(17, OUTPUT);

	// Unused pins on back
	// 27 28 29 32 33
	// 
	// if removed LED
	// 24, 25, 26

	// SPI0 
	// SCK0  13
	// MISO0 12
	// MOSI0 11
	// CS0   10
	 
	// SPI1
	// SCK1  27
	// MISO1 1
	// MOSI1 26
	// CS1   0
	
	// SPI2  
	// SCK2  37
	// MISO2 34
	// MOSI2 35
	// CS2   36
	/*
	digitalWrite(6, 0);
	delay(1);
	SPI1.transfer(0x55);
	delay(1);
	digitalWrite(6, 1);
	*/
	

	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
	pinMode(LCD_BL, OUTPUT);

	digitalWrite(LCD_BL, HIGH);
	digitalWrite(LED_B, OFF);
	digitalWrite(LED_G, OFF);
	digitalWrite(LED_R, OFF);
	digitalWrite(LED_R, ON);
	delay(200);
	digitalWrite(LED_R, OFF);

	digitalWrite(LED_G, ON);
	delay(200);
	digitalWrite(LED_G, OFF);

	digitalWrite(LED_B, ON);
	delay(200);
	digitalWrite(LED_B, OFF);

	ts.begin(40);
	display.begin();
	display.fillScreen(ILI9488_BLACK);
	display.setRotation(1);
	//display.setFont(Arial_14);
	//display.setFont(AwesomeF180_12);
	display.setFont(Michroma_11);
	//display.setFont(ComicSansMS_12);
	//display.setFont(OpenSans12);
	//display.setFont(Crystal_18_Italic);
	//display.setFont(Michroma_12);

	// 
	GUI_drawSquareBtn(0, 0, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
	GUI_drawSquareBtn(0, 0, 480, 45, "", menuBackground, menuBackground, menuBackground, ALIGN_CENTER);
	GUI_drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
	GUI_drawSquareBtn(160, 45, 260, 50, "", menuBtnColor, menuBtnColor, menuBtnColor, ALIGN_CENTER);

	// Create button objects
	createMenuBtns();

	// Draw Menu
	while (GUI_drawPage(userInterfaceMenuButton, graphicLoaderState, 3));
	graphicLoaderState = 0;

	Can1.begin();
	Can1.setBaudRate(500000);
	Can1.setMaxMB(16);
	Can1.enableFIFO();
	Can1.enableFIFOInterrupt();
	Can1.onReceive(canSniff1);
	Can1.mailboxStatus();

	Can2.begin();
	Can2.setBaudRate(500000);
	Can2.setMaxMB(16);
	Can2.enableFIFO();
	Can2.enableFIFOInterrupt();
	Can2.onReceive(canSniff2);
	Can2.mailboxStatus();
	Can2.disableFIFOInterrupt();

	Can3.begin();
	CANFD_timings_t config;
	config.clock = CLK_60MHz;
	config.baudrate = 500000;
	config.baudrateFD = 5000000;
	config.propdelay = 190;
	config.bus_length = 1;
	config.sample = 70;
	Can3.setBaudRate(config);
	Can3.setRegions(64);
	//Can3.onReceive(canSniff3);

	CAPTURE_createCaptureBtns();
}


/*=========================================================
	Background Processes
===========================================================*/
// All background process should be called from here
void backgroundProcess()
{
	GUI_buttonMonitor(userInterfaceMenuButton, MENU_BUTTON_SIZE);
	//updateTime();
	//serialOut();
	//SDCardOut();
	//timedTXSend();
	canSniff3();
}

/*=========================================================
	Main loop
===========================================================*/
// Main loop runs the user interface and calls for background processes
int i = 0;
void loop(void)
{
	appManager();
	backgroundProcess();

	//Can1.events();
	//Can2.events();
	//Can3.events();

	static uint32_t timeout123 = millis();
	if (millis() - timeout123 > 1000)
	{
		CANFD_message_t msg;
		msg.id = 0x4CA;
		msg.len = 64;
		for (uint8_t i = 0; i < 64; i++) msg.buf[i] = i + 0x25;
		Can3.write(msg);
		timeout123 = millis();
	}

	/*
	if (millis() - timeout123 > 2000)
	{
		CAN_message_t msg;
		msg.id = 1;
		for (uint8_t i = 0; i < 8; i++) msg.buf[i] = i + 1;
		Can1.write(msg);
		msg.id = 2;
		Can2.write(msg);
		timeout123 = millis();
	}
	*/
}
/*

	Can1.events();
	Can2.events();
	//Can3.events();

	static uint32_t timeout = millis();
	if (millis() - timeout > 1000)
	{

		CAN_message_t msg;
		msg.id = 1;
		for (uint8_t i = 0; i < 8; i++) msg.buf[i] = i + 1;
		Can1.write(msg);
		msg.id = 2;
		Can2.write(msg);
		/*
		CANFD_message_t msg;
		msg.id = 0x4CA;
		msg.len = 8;
		for (uint8_t i = 0; i < 8; i++) msg.buf[i] = i + 1;
		Can3.write(msg);
		timeout = millis();
		
	}
*/