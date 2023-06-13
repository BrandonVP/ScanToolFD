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

#include "ili9488_t3_font_Arial.h"
#include "ili9488_t3_font_ComicSansMS.h"
#include "ili9488_t3_font_ArialBold.h"
#include "font_OpenSans.h"
#include "font_DroidSans.h"
#include "font_Michroma.h"
#include "font_Crystal.h"
#include "font_ChanceryItalic.h"
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

UserInterfaceClass userInterfaceButtons[APP_BUTTON_SIZE];
UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can1;

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
	userInterfaceButtons[btnPos++].setButton(140, 275, 300, 315, 0, true, F("Set CAN0"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(305, 275, 475, 315, 0, true, F("Set CAN1"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(305, 60, 475, 100, 0, true, F("CAN0"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(305, 150, 475, 190, 0, true, F("CAN1"), ALIGN_CENTER);
}

void createCANBusCaptureBtns()
{
	uint8_t btnPos = 0;
	//10, 75, 160, 270
	//(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, String btnText, uint8_t alignText);
	userInterfaceButtons[btnPos++].setButton(100, 275, 220, 315, 0, true, F("START"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 275, 380, 315, 0, true, F("STOP"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(10, 80, 160, 120, 0, false, F("CAN0 FD"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(10, 120, 160, 160, 0, false, F("CAN1"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(10, 160, 160, 200, 0, false, F("CAN2"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(10, 200, 160, 240, 0, false, F("Wireless"), ALIGN_CENTER);
}

void createToolBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButtons[btnPos++].setButton(55, 80, 220, 120, 1, true, F("PID Scan"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 80, 425, 120, 2, true, F("PID Stream"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 140, 220, 180, 0, true, F("Get VIN"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 140, 425, 180, 0, true, F("DTC"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 200, 220, 240, 0, true, F("OBD Simulator"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Gauges"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 260, 220, 300, 0, true, F("TX Spam"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
}

void createSettingsBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButtons[btnPos++].setButton(55, 80, 220, 120, 1, true, F("Memory"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 80, 425, 120, 2, true, F("About"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 140, 220, 180, 0, true, F("WiFi MAC"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 140, 425, 180, 0, true, F("Overclock"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 200, 220, 240, 0, true, F("Reset WiFi"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Set Time"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 260, 220, 300, 0, true, F("Dongle"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 260, 425, 300, 0, true, F("Reset"), ALIGN_CENTER);
}

void clearAppSpace()
{
	drawSquareBtn(0, 51, 480, 320, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
}

// Manages the different App pages
void pageControl()
{
	int8_t results = 0;
	switch (page)
	{
	case 0: /*========== CANBUS ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createCANBusBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (drawPage(userInterfaceButtons, graphicLoaderState, 6))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		buttonMonitor(userInterfaceButtons, 6);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	case 1: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createCANBusCaptureBtns();
				clearAppSpace();

				// Menu frames
				drawSquareBtn(10, 75, 160, 270, "", menuBackground, frameBorder, menuBackground, ALIGN_CENTER);
				drawSquareBtn(165, 75, 315, 270, "", menuBackground, frameBorder, menuBackground, ALIGN_CENTER);
				drawSquareBtn(320, 75, 470, 270, "", menuBackground, frameBorder, menuBackground, ALIGN_CENTER);
				drawRoundBtn(10, 55, 160, 70, "Source", themeBackground, themeBackground, menuBtnTextColor, ALIGN_LEFT);
				drawRoundBtn(165, 55, 315, 70, "Output", themeBackground, themeBackground, menuBtnTextColor, ALIGN_LEFT);
				drawRoundBtn(320, 55, 470, 70, "Selected", themeBackground, themeBackground, menuBtnTextColor, ALIGN_LEFT);

				graphicLoaderState++;
				break;
			}
			if (drawPage(userInterfaceButtons, graphicLoaderState, 6))
			{
				break;
			}
			hasDrawn = true;
		}


		results = subMenuButtonMonitor(userInterfaceButtons, 6);



		// Release any variable locks if page changed
		if (nextPage != page)
		{
			Serial.println("Leaving");
			pageTransition();
		}
		break;
	case 2: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createCANBusBaudBtns();
				clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (drawPage(userInterfaceButtons, graphicLoaderState, 4))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		buttonMonitor(userInterfaceButtons, 4);

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
			if (drawPage(userInterfaceButtons, graphicLoaderState, 8))
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


			display.fillScreen(ILI9488_BLACK);
			display.setTextColor(ILI9488_WHITE);  display.setTextSize(4);
			display.enableScroll();
			display.setScrollTextArea(20, 20, 120, 240);
			display.setScrollBackgroundColor(ILI9488_GREEN);


			display.setCursor(20, 20);

			display.setTextColor(ILI9488_BLACK);

			for (int i = 0; i < 20; i++) {
				display.print("  this is line ");
				display.println(i);
				delay(500);
			}

			display.fillScreen(ILI9488_BLACK);
			display.setScrollTextArea(40, 50, 120, 120);
			display.setScrollBackgroundColor(ILI9488_GREEN);

			display.setTextSize(1);
			display.setCursor(40, 50);

			for (int i = 0; i < 20; i++) {
				display.print("  this is line ");
				display.println(i);
				delay(500);
			}




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

	userInterfaceMenuButton[menuPosition++].setButton(160, 0, 260, 40, CANBUS_MAIN, true, F("CAN Bus"), ALIGN_CENTER);
	userInterfaceMenuButton[menuPosition++].setButton(270, 0, 370, 40, VEHTOOL_MAIN, true, F("Tools"), ALIGN_CENTER);
	userInterfaceMenuButton[menuPosition++].setButton(375, 0, 475, 40, SETTING_MAIN, true, F("Settings"), ALIGN_CENTER);
}

void canSniff1(const CAN_message_t& msg) {
	Serial.print("1 MB "); Serial.print(msg.mb);
	Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
	Serial.print("  LEN: "); Serial.print(msg.len);
	Serial.print(" EXT: "); Serial.print(msg.flags.extended);
	Serial.print(" TS: "); Serial.print(msg.timestamp);
	Serial.print(" ID: "); Serial.print(msg.id, HEX);
	Serial.print(" Buffer: ");
	for (uint8_t i = 0; i < msg.len; i++) {
		Serial.print(msg.buf[i], HEX); Serial.print(" ");
	} Serial.println();
}

void canSniff2(const CAN_message_t& msg) {
	Serial.print("2 MB "); Serial.print(msg.mb);
	Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
	Serial.print("  LEN: "); Serial.print(msg.len);
	Serial.print(" EXT: "); Serial.print(msg.flags.extended);
	Serial.print(" TS: "); Serial.print(msg.timestamp);
	Serial.print(" ID: "); Serial.print(msg.id, HEX);
	Serial.print(" Buffer: ");
	for (uint8_t i = 0; i < msg.len; i++) {
		Serial.print(msg.buf[i], HEX); Serial.print(" ");
	} Serial.println();
}

// -------------------------------------------------------------
void setup(void)
{
	Serial.begin(9600); // USB is always 12 or 480 Mbit/sec

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
	display.setFont(ComicSansMS_12);
	//display.setFont(Michroma_12);
	//display.setFont(Crystal_18_Italic);
	//display.setFont(Michroma_12);

	// Clear LCD
	drawSquareBtn(0, 0, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
	drawSquareBtn(0, 0, 480, 45, "", menuBackground, menuBackground, menuBackground, ALIGN_CENTER);
	drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
	drawSquareBtn(160, 45, 260, 50, "", menuBtnColor, menuBtnColor, menuBtnColor, ALIGN_CENTER);

	// Create button objects
	createMenuBtns();

	// Draw Menu
	while (drawPage(userInterfaceMenuButton, graphicLoaderState, 3));
	graphicLoaderState = 0;

	Can0.begin();
	Can0.setBaudRate(500000);
	Can0.setMaxMB(16);
	Can0.enableFIFO();
	Can0.enableFIFOInterrupt();
	Can0.onReceive(canSniff1);
	Can0.mailboxStatus();

	Can1.begin();
	Can1.setBaudRate(500000);
	Can1.setMaxMB(16);
	Can1.enableFIFO();
	Can1.enableFIFOInterrupt();
	Can1.onReceive(canSniff2);
	Can1.mailboxStatus();
	Can1.disableFIFOInterrupt();
}


/*=========================================================
	Background Processes
===========================================================*/
// All background process should be called from here
void backgroundProcess()
{
	buttonMonitor(userInterfaceMenuButton, MENU_BUTTON_SIZE);
	//updateTime();
	//serialOut();
	//SDCardOut();
	//timedTXSend();
}

/*=========================================================
	Main loop
===========================================================*/
// Main loop runs the user interface and calls for background processes
void loop(void)
{
	pageControl();
	backgroundProcess();

	Can0.events();

	static uint32_t timeout = millis();

	if (millis() - timeout > 200)
	{
		CAN_message_t msg;
		msg.id = 1;
		for (uint8_t i = 0; i < 8; i++) msg.buf[i] = i + 1;
		Can0.write(msg);
		msg.id = 2;
		Can1.write(msg);
		timeout = millis();
	}
}