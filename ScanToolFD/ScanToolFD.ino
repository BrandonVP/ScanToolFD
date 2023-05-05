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
 

 //#include <kinetis_flexcan.h>
//#include <isotp_server.h>
//#include <isotp.h>
//#include <imxrt_flexcan.h>


//#include <circular_buffer.h>
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

// *Used by background process*
uint8_t selectedChannelOut = 0;
uint8_t selectedSourceOut = 0;
uint32_t updateClock = 0;
bool isSerialOut = false;
bool isSDOut = false;
bool isMSGSpam = false;

// General use variables
uint8_t state = 0;

// Use to load pages in pieces to prevent blocking while loading entire page
uint8_t graphicLoaderState = 0;

uint16_t buttonsOnPage = 0;
UserInterfaceClass userInterfaceButtons[APP_BUTTON_SIZE];
UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

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
	userInterfaceButtons[btnPos++].setButton(140, 275, 300, 315, true, 0, NULL, F("Set CAN0"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(305, 275, 475, 315, true, 0, NULL, F("Set CAN1"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(305, 60, 475, 100, true, 0, NULL, F("CAN0"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(305, 150, 475, 190, true, 0, NULL, F("CAN1"), ALIGN_CENTER);
}

void createCANBusCaptureBtns()
{
	uint8_t btnPos = 0;
	
	userInterfaceButtons[btnPos++].setButton(135, 110, 235, 150, true, 0, NULL, F("CAN1FD"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(135, 150, 235, 190, true, 0, NULL, F("CAN2"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(135, 190, 235, 230, true, 0, NULL, F("CAN3"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(135, 230, 235, 270, true, 0, NULL, F("Wireless"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(135, 270, 235, 310, true, 0, NULL, F(""), ALIGN_CENTER);
	
	userInterfaceButtons[btnPos++].setButton(240, 110, 340, 150, true, 0, NULL, F("LCD"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(240, 150, 340, 190, true, 0, NULL, F("Serial"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(240, 190, 340, 230, true, 0, NULL, F("SD Card"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(240, 230, 340, 270, true, 0, NULL, F("Wireless"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(240, 270, 340, 310, true, 0, NULL, F(""), ALIGN_CENTER);

	// Non button prints
	userInterfaceButtons[btnPos++].setButton(135, 55, 235, 95, true, 0, NULL, F("INPUT"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(240, 55, 340, 95, true, 0, NULL, F("OUTPUT"), ALIGN_CENTER);


}


// Manages the different App pages
void pageControl()
{
	switch (page)
	{
	case 0: /*========== CANBUS ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (graphicLoaderState == 0)
			{
				createCANBusBtns();
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
		buttonMonitor(userInterfaceButtons, 8);

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
				graphicLoaderState++;
				break;
			}
			if (drawPage(userInterfaceButtons, graphicLoaderState, 12))
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		buttonMonitor(userInterfaceButtons, 10);

		// Release any variable locks if page changed
		if (nextPage != page)
		{
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
	case 9: // Filter Mask
		// Draw page and lock variables
		if (!hasDrawn)
		{
			drawRoundBtn(140, 80, 305, 130, F("b!"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
			if (1)
			{
				
				
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
	}
}

//
void createMenuBtns() 
{
	// Create Menu Buttons
	uint8_t menuPosition = 0;
	userInterfaceMenuButton[menuPosition++].setButton(5, 32, 125, 83, true, CANBUS_MAIN, NULL, F("CANBUS"), ALIGN_CENTER);
	userInterfaceMenuButton[menuPosition++].setButton(5, 88, 125, 140, true, VEHTOOL_MAIN, NULL, F("VEHTOOL"), ALIGN_CENTER);
	userInterfaceMenuButton[menuPosition++].setButton(5, 145, 125, 197, true, UTVTOOL_MAIN, NULL, F("UTVTOOL"), ALIGN_CENTER);
	userInterfaceMenuButton[menuPosition++].setButton(5, 202, 125, 254, true, TESTING_MAIN, NULL, F("TESTING"), ALIGN_CENTER);
	userInterfaceMenuButton[menuPosition++].setButton(5, 259, 125, 312, true, SETTING_MAIN, NULL, F("SETTING"), ALIGN_CENTER);
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

    if (!ts.begin(40)) {
        Serial.println("Unable to start touchscreen.");
    }
    else {
        Serial.println("Touchscreen started.");
    }

    display.begin();
    display.fillScreen(ILI9488_BLUE);
    display.setRotation(1);
    delay(1000);
    display.fillScreen(ILI9488_WHITE);
    delay(1000);

    display.setTextColor(ILI9488_RED);
    display.setFont(Arial_14_Bold);
    //display.setFont(ComicSansMS_12);
    display.println("Arial_12");
    delay(2000);

	// Clear LCD
	drawSquareBtn(0, 0, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
	drawSquareBtn(0, 0, 130, 319, "", menuBackground, menuBackground, menuBackground, ALIGN_CENTER);

	// Create button objects
	createMenuBtns();
    
	// Draw Menu
	while (drawPage(userInterfaceMenuButton, graphicLoaderState ,5));
	graphicLoaderState = 0;
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
}  
