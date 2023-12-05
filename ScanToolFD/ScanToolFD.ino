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
 Wires config
 C2 out only config
 Baud
 filter mask
 SD FD format for 64 byte data
 read sd logs on card
 -delete
 -replay
 -view
 
 Tools 
 -MSG spam
 -OBD Sim?
 -VIN

 Bug
 Moving off button after selecting will prevent deselect
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

#include "SDCard.h"
#include <MTP_Teensy.h>
#include <TimeLib.h>
#include <FlexCAN_T4.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> // Touch
#include <ILI9488_t3.h>      // Display

#include "appManager.h"
#include "batteryMonitor.h"
#include "CANBusCapture.h"
#include "CANBus.h"
#include "config.h"
#include "gui.h"
#include "KeyInput.h"
#include "RGB_LED.h"
#include "serialTransfer.h"
#include "variableLock.h"
#include "cbBuffer.h"
#include "cbBufferFD.h"

#include "ili9488_t3_font_Arial.h"
//#include "ili9488_t3_font_ComicSansMS.h"
//#include "ili9488_t3_font_ArialBold.h"
//#include "font_AwesomeF180.h"
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

*/

ILI9488_t3 display = ILI9488_t3(&SPI, TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

APP_labels nextApp = APP_CANBUS;
APP_labels activeApp = APP_CANBUS;

// Used for page control
bool hasDrawn = false;

// Use to load pages in pieces to prevent blocking while loading entire page
uint8_t graphicLoaderState = 0;
uint8_t buttonsOnPage = 0;

std::vector<appManager> myApps;

UserInterfaceClass userInterfaceButton[APP_BUTTON_SIZE];
UserInterfaceClass userInterfaceCaptureButton[CAPTURE_BUTTON_SIZE];
UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
//FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can3;
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> Can3; // FD

cbBuffer can1Buffer;
cbBuffer can2Buffer;
cbBufferFD can3Buffer;
bool enableCB1 = true;
bool enableCB2 = true;
bool enableCB3 = true;

const int chipSelect = 2;
File myFile;

// Icon
const uint8_t battery_bits[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0x07,0xFE,0xFF,0xFF,0x07,
  0x8E,0xE3,0x18,0x07,0x8E,0xE3,0x18,0x07,0x8E,0xE3,0x18,0x7F,
  0x8E,0xE3,0x18,0x7F,0x8E,0xE3,0x18,0x7F,0x8E,0xE3,0x18,0x7F,
  0x8E,0xE3,0x18,0x7F,0x8E,0xE3,0x18,0x7F,0x8E,0xE3,0x18,0x7F,
  0x8E,0xE3,0x18,0x7F,0x8E,0xE3,0x18,0x7F,0x8E,0xE3,0x18,0x7F,
  0x8E,0xE3,0x18,0x07,0x8E,0xE3,0x18,0x07,0xFE,0xFF,0xFF,0x07,
  0xFE,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// Icon print sub function
unsigned int reverseBits(uint8_t num)
{
	unsigned int NO_OF_BITS = sizeof(num) * 2;
	unsigned int reverse_num = 0, i, temp;

	for (i = 0; i < NO_OF_BITS; i++)
	{
		temp = (num & (1 << i));
		if (temp) reverse_num |= (1 << ((NO_OF_BITS - 1) - i));
	}
	return reverse_num;
}

// Then wrote this function to print the bits (located in icons.h)
void print_icon(uint16_t x, uint16_t y, const uint8_t icon[], uint8_t maxRow, uint8_t maxColumn, uint16_t color1, uint16_t color2)
{
	int16_t i = 0, row, column, bit, temp;
	for (row = 0; row < maxRow; row++) //64
	{
		for (column = 0; column < maxColumn; column++) // 8
		{
			temp = icon[i];
			for (bit = 7; bit >= 0; bit--)
			{
				if (temp & 1)
				{
					display.drawPixel(x + (column * 8) + (8 - bit), y + row, color1);
				}
				else
				{
					//display.drawPixel(x + (column * 8) + (8 - bit), y + row, menuBtnColor);
				}
				temp >>= 1;
			}
			i++;
		}
	}
}

// Arduino library time update
time_t getTeensy3Time()
{
	return Teensy3Clock.get();
}

// Resets variables for page change
void appTransition()
{
	hasDrawn = false;
	graphicLoaderState = 0;
	activeApp = nextApp;
	GUI_stopLoadBarTimed();
	display.useFrameBuffer(true);
}

//
uint8_t createToolBtns()
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
	return btnPos;
}

//
uint8_t createSettingsBtns()
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
	return btnPos;
}

//
void loadApps()
{
	myApps.reserve(15);
	appManager appObj0(MENU_canBus, "CAN Bus", APP_CANBUS, CAPTURE_CANBus, CAPTURE_createMenuBtns);
	myApps.push_back(appObj0);
	appManager appObj1(MENU_tools, "Tools", APP_TOOLS, someFn, createToolBtns);
	myApps.push_back(appObj1);
	appManager appObj2(MENU_settings, "Settings", APP_SETTINGS, someFn, createSettingsBtns);
	myApps.push_back(appObj2);
	appManager appObj3(MENU_canBus, "Capture", APP_CAPTURE, CAPTURE_captureConfig, CAPTURE_createCaptureBtns);
	myApps.push_back(appObj3);
	appManager appObj4(MENU_sub, "CaptureLCD", APP_CAPTURE_LCD, CAPTURE_LCD_scan, CAPTURE_createLCDBtns);
	myApps.push_back(appObj4);
	appManager appObj5(MENU_canBus, "Files", APP_FILES, someFn, KEYINPUT_createKeyboardButtons);
	myApps.push_back(appObj5);
	appManager appObj6(MENU_canBus, "FilterMask", APP_FILTER_MASK, someFn, KEYINPUT_createKeyboardButtons);
	myApps.push_back(appObj6);
	appManager appObj7(MENU_canBus, "Send", APP_SEND, someFn, KEYINPUT_createKeyboardButtons);
	myApps.push_back(appObj7);	
	appManager appObj8(MENU_canBus, "BaudRate", APP_BAUD_RATE, CAPTURE_Baud, CAPTURE_createBaudBtns);
	myApps.push_back(appObj8);
	
	//myApps.insert((myApps.begin() + 3), appObj4);
}

//
void drawMenu()
{
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

	print_icon(5, 5, battery_bits, 32, 4, menuBtnTextColor, 1);
}

//
void setCANBusFD(FLEXCAN_CLOCK clock, int baud)
{
	CANFD_timings_t config;
	config.clock = clock; // CLK_24MHz;// CLK_60MHz;
	config.baudrate = baud;
	config.baudrateFD = baud;
	config.propdelay = 190;
	config.bus_length = 1;
	config.sample = 70;
	Can3.setBaudRate(config);
}

// -------------------------------------------------------------
void setup(void)
{
	Serial.begin(115200); // USB is always 12 or 480 Mbit/sec
	//Serial2.begin(115200); // ESP8266

	//while (!Serial);

	// Update time
	setSyncProvider(getTeensy3Time);

	// Unused pins on back
	// 27 28 29 32 33
	// 
	// if removed LED
	// 24, 25, 26

	// SPI0 
	// SCK0  13 // Orange
	// MISO0 12 // Blue
	// MOSI0 11 // White/Pink
	// CS0   10 // Brown

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

	// Blue   - MISO
	// White  - MOSI
	// Orange - SCK
	// Brown  - CS

	LED_initialize();

	LED_RGB((RGB)LED_RED);
	delay(200);
	LED_RGB((RGB)LED_OFF);

	LED_RGB((RGB)LED_GREEN);
	delay(200);
	LED_RGB((RGB)LED_OFF);

	LED_RGB((RGB)LED_BLUE);
	delay(200);
	LED_RGB((RGB)LED_OFF);

	pinMode(LCD_BL, OUTPUT);
	digitalWrite(LCD_BL, HIGH);

	ts.begin(40);
	display.begin();
	display.useFrameBuffer(true);
	display.fillScreen(ILI9488_BLACK);
	display.setRotation(1);

	//display.setFont(Arial_14);
	//display.setFont(AwesomeF180_12);
	display.setFont(Michroma_11);
	//display.setFont(ComicSansMS_12);
	//display.setFont(OpenSans12);
	//display.setFont(Crystal_18_Italic);
	//display.setFont(Michroma_12);

	Can1.begin();
	Can1.setBaudRate(500000);
	Can1.setMaxMB(16);
	Can1.enableFIFO();
	Can1.enableFIFOInterrupt();
	Can1.onReceive(CANBus1_IRQHandler);
	Can1.mailboxStatus();

	Can2.begin();
	Can2.setBaudRate(500000);
	Can2.setMaxMB(16);
	Can2.enableFIFO();
	Can2.enableFIFOInterrupt();
	Can2.onReceive(CANBus2_IRQHandler);
	Can2.mailboxStatus();
	Can2.disableFIFOInterrupt();


	/* // Start as nonFD
	Can3.begin();
	Can3.setBaudRate(500000);
	Can3.setMaxMB(16);
	Can3.enableFIFO();
	Can3.enableFIFOInterrupt();
	Can3.onReceive(CANBus3_IRQHandler);
	Can3.mailboxStatus();
	Can3.disableFIFOInterrupt();
	*/

	Can3.begin();
	/*
	CANFD_timings_t config;
	config.clock = CLK_24MHz;
	config.baudrate = 1000000;
	config.baudrateFD = 2000000;
	config.propdelay = 190;
	config.bus_length = 1;
	config.sample = 70;
	FD.setBaudRate(config);
	*/
	setCANBusFD(CLK_24MHz, CANBusFDBaudRate); //CLK_24MHz;// CLK_60MHz;
	Can3.setRegions(64);

	
	CAPTURE_createCaptureBtns();

	// mandatory to begin the MTP session.
	MTP.begin();

	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	SD.begin(chipSelect);
	
	MTP.addFilesystem(SD, "SD Card");
	myFile = SD.open("a.txt", FILE_WRITE);

	// if the file opened okay, write to it:
	if (myFile) {
		Serial.print("Writing to test.txt...");
		myFile.println("testing 1, 2, 3.");
		// close the file:
		myFile.close();
		Serial.println("done.");
	}
	else {
		// if the file didn't open, print an error:
		Serial.println("error opening a.txt");
	}

	// re-open the file for reading:
	myFile = SD.open("a.txt");
	if (myFile) {
		Serial.println("a.txt:");

		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			Serial.write(myFile.read());
		}
		// close the file:
		myFile.close();
	}
	else {
		// if the file didn't open, print an error:
		Serial.println("error opening a.txt");
	}
	
	pinMode(6, OUTPUT);
	digitalWrite(6, LOW);


	loadApps();

	drawMenu();




	/*
  display.setTextColor(ILI9488_WHITE);  
  display.setTextSize(4);
  display.enableScroll();
  display.setScrollTextArea(0,0,120,240);
 
  display.setScrollBackgroundColor(ILI9488_GREEN);
  display.setCursor(180, 100);
  display.print("Fixed text");
  display.setCursor(0, 0);
  display.setTextColor(ILI9488_BLACK);
  display.fillRect(5, 5, 80, 100, menuBackground);

  for(int i=0;i<20;i++){
	display.print("  this is line ");
	display.println(i);
	display.updateScreen();
	delay(100);
  }
  */

}

//
void someFn(int a)
{
	// Temporary holder 
}

//
void CAPTURE_CANBus(int userInput)
{
	if (userInput >= 0)
	{
		nextApp = (APP_labels)userInput;
		Serial.printf("nextApp: %d \n", nextApp);
	}
}

//
void appLoader()
{
	// Draw page and lock variables
	if (!hasDrawn)
	{
		if (graphicLoaderState == 0)
		{
			GUI_clearAppSpace();
			buttonsOnPage = myApps[(int)activeApp].printButtons();
			graphicLoaderState++;
			return;
		}
		if (GUI_drawPage(userInterfaceButton, graphicLoaderState, buttonsOnPage))
		{
			return;
		}
		Serial.printf("App: %d\n", (int)activeApp);
		Serial.println(myApps[(int)activeApp].getName());
		hasDrawn = true;
		display.updateScreen();
	}

	// Call buttons or page method
	myApps[activeApp].runApp(GUI_subMenuButtonMonitor(userInterfaceButton, buttonsOnPage));

	// Load next selected app
	if (nextApp != activeApp)
	{
		appTransition();
	}
}

/*
// Manages the loading and unloading of different user Apps
void appManager1()
{
	int results = 0;
	//error_t e = 0;

	switch (activeApp)
	{
	case APP_CANBUS_TOOLS: 
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				buttonsOnPage = CAPTURE_createMenuBtns();
				GUI_clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceButton, graphicLoaderState, buttonsOnPage))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_buttonMonitor(userInterfaceButton, 6);

		// Release any variable locks if page changed
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case APP_CAPTURE_CONFIG: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				buttonsOnPage = myApps[0].printButtons();
				GUI_clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userInterfaceCaptureButton, graphicLoaderState, buttonsOnPage))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		//CAPTURE_captureConfig();
		myApps[0].runApp(1);

		// Release any variable locks if page changed
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case APP_CAPTURE_LCD: // 
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createLCDBtns();
				GUI_clearAppSpace();
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
		if (nextApp != activeApp)
		{
			if (CANBusOut == 9)
			{
				CANBusOut = 0;
			}
			LCDPos = 60;
			display.setFont(Michroma_11);
			appTransition();
		}
		break;
	case APP_CAPTURE_FILES: // 
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				KEYINPUT_createKeyboardButtons();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userKeyButtons, graphicLoaderState, 42))
			{
				break;
			}
			Serial.println("hasDrawn");
			hasDrawn = true;
		}

		// Call buttons or page method
		results = GUI_subMenuButtonMonitor(userKeyButtons, 42);

		if (results == 0xcc)
		{
			KEYINPUT_createUpperCaseButtons();
			graphicLoaderState = 3;
			while(GUI_drawPage(userKeyButtons, graphicLoaderState, 36));
		}

		if (results == 0xaa)
		{
			KEYINPUT_createKeyboardButtons();
			graphicLoaderState = 3;
			while (GUI_drawPage(userKeyButtons, graphicLoaderState, 36));
		}

		// Release any variable locks if page changed
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case APP_CAPTURE_BAUD: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createBaudBtns();
				GUI_clearAppSpace();
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
		GUI_subMenuButtonMonitor(userInterfaceButton, 21);

		// Release any variable locks if page changed
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case APP_CAPTURE_SEND: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				KEYINPUT_createNumpadButtons();
				GUI_clearAppSpace();
				graphicLoaderState++;
				break;
			}
			if (GUI_drawPage(userKeyButtons, graphicLoaderState, 21))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		GUI_subMenuButtonMonitor(userKeyButtons, 21);

		// Release any variable locks if page changed
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case APP_CAPTURE_FILTERMASK: // CAN Bus Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createBaudBtns();
				GUI_clearAppSpace();
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
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case APP_CAPTURE_AUTOBAUD: // CAN Bus Capture
		// Draw page and lock variables
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
		CPU_RESTART;
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				CAPTURE_createBaudBtns();
				GUI_clearAppSpace();
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
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case 9: // Tools
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createToolBtns();
				GUI_clearAppSpace();
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
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	case 36: // Settings
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createSettingsBtns();
				GUI_clearAppSpace();
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

			


		}

		// Call buttons or page method


		// Release any variable locks if page changed
		if (nextApp != activeApp)
		{
			appTransition();
		}
		break;
	}
}
*/

//
void createMenuBtns()
{
	// Create Menu Buttons
	uint8_t menuPosition = 0;

	userInterfaceMenuButton[menuPosition++].setButton(160, 0, 260, 40, APP_CANBUS, true, 0, F("CAN Bus"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
	userInterfaceMenuButton[menuPosition++].setButton(270, 0, 370, 40, APP_TOOLS, true, 0, F("Tools"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
	userInterfaceMenuButton[menuPosition++].setButton(375, 0, 475, 40, APP_SETTINGS, true, 0, F("Settings"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
}

//
void CANBus1_IRQHandler(const CAN_message_t& msg)
{
	LED_pulse((RGB)LED_RED);
	Serial.printf("1: %03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	can1Buffer.push_cb(msg.id, msg.len, msg.buf);
}

//
void CANBus2_IRQHandler(const CAN_message_t& msg)
{
	LED_pulse((RGB)LED_BLUE);
	Serial.printf("2: %03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	can2Buffer.push_cb(msg.id, msg.len, msg.buf);
}

//
void CANBus3_IRQHandler()
{
	CANFD_message_t msg;
	if (Can3.read(msg))
	{
		LED_pulse((RGB)LED_GREEN);
		//Serial.print(msg.id, HEX);
		//Serial.print("   ");
		//for (int i = 0; i < msg.len; i++)
		//{
		//	Serial.print(msg.buf[i], HEX);
		//	Serial.print("  ");
		//}
		//Serial.println();
		can3Buffer.push_cb(msg.id, msg.len, msg.buf);
	}
	//Serial.printf("%03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	
}

//
void CANBus3_IRQHandler2()
//void CANBus3_IRQHandler(const CANFD_message_t& msg)
{
	CANFD_message_t msg;
	if (Can3.read(msg)) 
	{
		LED_pulse((RGB)LED_GREEN);
		Serial.print(msg.id, HEX);
		Serial.print("   ");
		for (int i = 0; i < msg.len; i++) 
		{
			Serial.print(msg.buf[i], HEX);
			Serial.print("  ");
		}
		Serial.println();
	}
}


/*=========================================================
	Background Processes
===========================================================*/
// All background process should be called from here
void backgroundProcess()
{
	GUI_buttonMonitor(userInterfaceMenuButton, MENU_BUTTON_SIZE);
	BATTERY_printLevel();
	updateTime();
	MTP.loop();

	LED_strobe((RGB)LED_OFF);
	
	CANBus3_IRQHandler();
	CAPTURE_processSerialCapture();
	CAPTURE_processWirelessCapture();
	//timedTXSend();
}

// Displays time in menu
void updateTime()
{
	static uint32_t updateClock = 0;
	if (millis() - updateClock > 999)
	{
		display.setTextColor(menuBtnTextColor);
		char printStringT[64];
		char printStringB[64];

		display.setFont(Michroma_8);

		 // menuBackground

		sprintf(printStringT, "%2d:%2d:%2d", hour(), minute(), second());
		sprintf(printStringB, "%2d/%2d/%2d", month(), day(), year());

		// Print to screen
		display.useFrameBuffer(false);
		display.fillRect(59, 8, 88, 27, menuBackground);
		display.drawString(printStringT, 60, 10);
		display.drawString(printStringB, 60, 25);
		display.useFrameBuffer(true);

		// Update buffer but DO NOT print buffer
		display.fillRect(59, 8, 88, 27, menuBackground);
		display.drawString(printStringT, 60, 10);
		display.drawString(printStringB, 60, 25);

		display.setFont(Michroma_11);
		updateClock = millis();
	}
}

/*=========================================================
	Main loop
===========================================================*/
// Main loop runs the user interface and calls for background processes
void loop(void)
{
	appLoader();
	backgroundProcess();

	Can1.events();
	Can2.events();
	//Can3.events();

	static uint32_t timeout123 = millis();
	static uint16_t rotatingID = 0;
	if (millis() - timeout123 > 2000)
	{
		if (CAPTURE_input_config == BTN_config_input_C3)
		{
			CANFD_message_t msg;
			msg.id = 0x4CA;
			msg.len = 64;
			for (uint8_t i = 0; i < 64; i++) msg.buf[i] = i + 0x25;
			Can3.write(msg);
		}
		else if (CAPTURE_input_config == BTN_config_input_C2)
		{
			CAN_message_t msg;
			msg.id = rotatingID++;
			msg.len = 8;
			for (uint8_t i = 0; i < 8; i++) msg.buf[i] = (uint8_t)millis() + (i * 0x3C);
			Can2.write(msg);
		}
		else if (CAPTURE_input_config == BTN_config_input_C1)
		{
			CAN_message_t msg;
			msg.id = rotatingID++;
			msg.len = 8;
			for (uint8_t i = 0; i < 8; i++) msg.buf[i] = (uint8_t)millis() + (i * 0x3C);
			Can1.write(msg);
		}

		//drawMenu();
		
		//Serial.printf("activeApp: %d\n", activeApp);
		//Serial.printf("isCaptureRunning: %d   CAPTURE_input_config: %d   CAPTURE_input_config: %d \n", isCaptureRunning, CAPTURE_input_config, CAPTURE_output_config);
		timeout123 = millis();
	}
}


// works
/*

  tft.setTextColor(ILI9488_WHITE);  tft.setTextSize(4);
  tft.enableScroll();
  tft.setScrollTextArea(0,0,120,240);
  tft.setScrollBackgroundColor(ILI9488_GREEN);

  tft.setCursor(180, 100);

  tft.setFont(ComicSansMS_12);
  tft.print("Fixed text");

  tft.setCursor(0, 0);

  tft.setTextColor(ILI9488_BLACK);

  for(int i=0;i<20;i++){
	tft.print("  this is line ");
	tft.println(i);
	tft.updateScreen();
	delay(100);
  }

  tft.fillScreen(ILI9488_BLACK);
  tft.setScrollTextArea(40,50,120,120);
  tft.setScrollBackgroundColor(ILI9488_GREEN);
  tft.setFont(ComicSansMS_10);

  tft.setTextSize(1);
  tft.setCursor(40, 50);
  tft.updateScreen();
  for(int i=0;i<2000;i++){
	tft.print("  this is line ");
	tft.println(i);
	  tft.updateScreen();
	//delay(100);
  }
*/