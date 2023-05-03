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
 
#include "CANBusCapture.h"
 //#include <kinetis_flexcan.h>
//#include <isotp_server.h>
//#include <isotp.h>
//#include <imxrt_flexcan.h>
#include "CANBus.h"

//#include <circular_buffer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> // Touch
#include <ILI9488_t3.h>      // Display

#include "gui.h"
#include "test.h"
#include "variableLock.h"
#include "config.h"

#include "ili9488_t3_font_Arial.h"
#include "ili9488_t3_font_ComicSansMS.h"

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
		//CANBusButtons();

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
    display.setTextColor(ILI9488_BLACK);
    display.setFont(Arial_12);
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
