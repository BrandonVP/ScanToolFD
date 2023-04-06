#include "test.h"
#include "variableLock.h"
#include "userInterface.h"
#include "definitions.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> // Touch
#include <ILI9488_t3.h>      // Display
#include "ili9488_t3_font_Arial.h"
#include "ili9488_t3_font_ComicSansMS.h"

// Global LCD theme color variables
#define themeBackground 0xFFFF  // White
#define menuBtnText 0xFFFF      // White
#define menuBtnBorder 0x0000    // Black
#define menuBtnColor 0xFC00     // Orange
#define menuBackground 0xC618   //Silver

#define LED_R 24
#define LED_B 26
#define LED_G 25
#define LCD_BL 14
#define LCD_RST 15
#define ON LOW
#define OFF HIGH
#define TFT_CS 10
#define TFT_DC 9

ILI9488_t3 display = ILI9488_t3(&SPI, TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

int oldTouchX = 0;
int oldTouchY = 0;

// For touch controls
int x, y;

// Used for page control
uint8_t nextPage = 0;
uint8_t page = 0;
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

// Filter range / Filter Mask
uint32_t CAN0Filter = 0x000;
uint32_t CAN0Mask = 0x000;
uint32_t CAN1Filter = 0x000;
uint32_t CAN1Mask = 0x000;
uint32_t CANWiFiFilter = 0x000;
uint32_t CANWiFiMask = 0x000;

// Use to load pages in pieces to prevent blocking while loading entire page
uint8_t graphicLoaderState = 0;

const uint8_t RESERVED_MENU = 5;
uint16_t buttonOnPage = RESERVED_MENU;
UserInterfaceClass userInterfaceButtons[40]; // First 10 are reserverd for menu


// Simplifies getting x and y coords
bool Touch_getXY()
{
    if (ts.touched())
    {
        TS_Point p = ts.getPoint();
        x = p.y;
        y = SCREEN_HEIGHT - p.x;
        //Serial.print("X= ");
        //Serial.print(x);
        //Serial.print(" Y= ");
        //Serial.println(y);
        return true;
    }
    return false;
}

// Holds round button down while pressed
void waitForIt(int x_start, int y_start, int x_stop, int y_stop)
{
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, themeBackground);
    while (ts.touched())
    {
        // This is a blocking function so call backgroundProcess while looping
        backgroundProcess();
    }
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, menuBtnBorder);
}

// Holds rectangle button down while pressed
void waitForItRect(int x_start, int y_start, int x_stop, int y_stop)
{
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), themeBackground);
    while (ts.touched())
    {
        // This is a blocking function so call backgroundProcess while looping
        backgroundProcess();
    }
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), menuBtnBorder);
}

//
void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align)
{
    const uint8_t LETTER_WIDTH = 11;
    const uint8_t SIDE_OFFSET = 2;
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, btnBgColor);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, btnBorderColor);

    // Print String with desired alignment
    switch (align) 
    {
    case 1: // Left
        display.drawString(buttonText, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    case 2: // Center
        // Calculate center
        stringLength = buttonText.length() * LETTER_WIDTH;
        buttonWidth = (x_stop - SIDE_OFFSET) - (x_start + SIDE_OFFSET);
        offset = (x_start+SIDE_OFFSET) + (buttonWidth / 2) - (stringLength / 2);

        display.setTextColor(btnTxtColor);
        //display.setCursor(offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset, false);
        //display.println(button);
        display.drawString(buttonText, offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset);
        break;
    case 3: // Right
        display.drawString(buttonText, x_start + 55, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    default:
        break;
    }
}

//
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align)
{
    const uint8_t LETTER_WIDTH = 11;
    const uint8_t SIDE_OFFSET = 2;
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBgColor);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBorderColor);

    // Print String with desired alignment
    switch (align) 
    {
    case 1: // Left
        display.drawString(buttonText, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    case 2: // Center
        // Calculate center
        stringLength = buttonText.length() * LETTER_WIDTH;
        buttonWidth = (x_stop - SIDE_OFFSET) - (x_start + SIDE_OFFSET);
        offset = (x_start + SIDE_OFFSET) + (buttonWidth / 2) - (stringLength / 2);

        display.setTextColor(btnTxtColor);
        //display.setCursor(offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset, false);
        //display.println(button);
        display.drawString(buttonText, offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset);
        break;
    case 3: // Right
        display.drawString(buttonText, x_start + 55, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    default:
        break;
    }
}

// Manages the different App pages
void pageControl()
{
	switch (page)
	{
	case CANBUS_MAIN: /*========== CANBUS ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawCANBus())
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

	case 1: // Capture
		// TODO: Fix this hot mess. Need to be in standard format like the rest of the pages.
		// Draw page and lock variables
		/*
		switch (state)
		{
		case 0:
			if (!hasDrawn && graphicLoaderState < 1)
			{
				error_t e = false;
				(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16);
				if (e)
				{
					DEBUG_ERROR(F("Error: Variable locked"));
					delay(5000);
					graphicLoaderState = 0;
					nextPage = CANBUS_MAIN;
				}
			}
			if (!hasDrawn && drawCapture())
			{
				graphicLoaderState++;
			}
			else
			{
				state = 1;
				graphicLoaderState = 0;
				//drawCaptureSelected();
			}
			break;
		case 1:
			if (!hasDrawn && drawCaptureSource())
			{
				graphicLoaderState++;
			}
			else
			{
				hasDrawn = true;
			}
			break;
		case 2:
			if (!hasDrawn && state == 2)
			{
				if (drawCaptureOutput())
				{
					graphicLoaderState++;
				}
				else
				{
					hasDrawn = true;
					//drawCaptureSelected();
				}
			}
			break;
		}
		*/
		// Call buttons or page method
		//CaptureButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar16(POS0);
			pageTransition();
		}
		break;

	case 2: // Capture files
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawCANLog())
			{
				break;
			}

			error_t e = false;
			(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32); // File read index
			(lockVar32(POS1)) ? g_var32[POS1] = 0 : e = lockError(POS1, 32); // Time of last touch
			(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16); // fileList index
			(lockVar16(POS1)) ? g_var16[POS1] = 60 : e = lockError(POS1, 16); // LCD Print line starts at y coord 60
			(lockVar16(POS2)) ? g_var16[POS2] = 0 : e = lockError(POS2, 16); // Initial x postition
			(lockVar16(POS3)) ? g_var16[POS3] = 0 : e = lockError(POS3, 16); // Initial y postition
			(lockVar16(POS4)) ? g_var16[POS4] = 0 : e = lockError(POS4, 16); // Last x postition
			(lockVar16(POS5)) ? g_var16[POS5] = 0 : e = lockError(POS5, 16); // Last y postition
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8); // Scroll index
			(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8); // Swipe input direction
			(lockVar8(POS2)) ? g_var8[POS2] = 0 : e = lockError(POS2, 8); // Bool to track initial x/y position
			(lockVar8(POS3)) ? g_var8[POS3] = 0 : e = lockError(POS3, 8); // Bool to track end of file
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = CANBUS_MAIN;
			}

			//drawCANLogScroll();
			state = 0;
			hasDrawn = true;
		}

		// Call buttons or page method
		//playback();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS0);
			unlockVar8(POS1);
			unlockVar8(POS2);
			unlockVar8(POS3);
			unlockVar16(POS0);
			unlockVar16(POS1);
			unlockVar16(POS2);
			unlockVar16(POS3);
			unlockVar16(POS4);
			unlockVar16(POS5);
			unlockVar32(POS0);
			unlockVar32(POS1);
			pageTransition();
		}
		break;

	case 3: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			error_t e = false;

			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				nextPage = CANBUS_MAIN;
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

	case 4: // Filter Mask
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawFilterMask())
			{
				break;
			}

			error_t e = false;
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8); // Keypad return
			(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8); // Keypad index
			(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16); // Current total value
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = CANBUS_MAIN;
			}

			state = 1;
			hasDrawn = true;
		}

		// Call buttons or page method
		//filterMask();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS0);
			unlockVar8(POS1);
			unlockVar16(POS0);
			pageTransition();
		}
		break;

	case 5: // Set Baud
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawBaud())
			{
				break;
			}

			error_t e = false;
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = true; // Selected Baud rate array index
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = CANBUS_MAIN;
			}

			//drawBaudScroll();
			//drawCurrentBaud();

			hasDrawn = true;
		}

		// Call buttons or page method
		//baudButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS0);
			pageTransition();
		}
		break;

	case 6: // Timed TX
		// Draw page and lock variables
		if (!hasDrawn)
		{

			if (1)//drawTimedTX())
			{
				break;
			}

			error_t e = false;
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8);    // Selected index
			(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8);    // User input
			(lockVar8(POS2)) ? g_var8[POS2] = 0 : e = lockError(POS2, 8);    // Keypad index
			(lockVar8(POS3)) ? g_var8[POS3] = 0 : e = lockError(POS3, 8);    // Scroll index
			(lockVar8(POS4)) ? g_var8[POS4] = 0 : e = lockError(POS4, 8);    // Node position
			(lockVar8(POS5)) ? g_var8[POS5] = 0 : e = lockError(POS5, 8);    // Keyboard index
			(lockVar8(POS6)) ? g_var8[POS6] = 0 : e = lockError(POS6, 8);    // Delete node location
			(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16); // Total value
			(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32); // Timer to prevent double tap
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = CANBUS_MAIN;
			}

			//drawTXNode(g_var8[POS3]);

			state = 0;
			hasDrawn = true;
		}
		// Call buttons or page method
		//timedTX();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar32(POS0);
			unlockVar16(POS0);
			unlockVar8(POS0);
			unlockVar8(POS1);
			unlockVar8(POS2);
			unlockVar8(POS3);
			unlockVar8(POS4);
			unlockVar8(POS5);
			unlockVar8(POS6);
			pageTransition();
		}
		break;

	case 7: // LCD Capture
		// Draw page and lock variables
		if (!hasDrawn)
		{
			//drawReadInCANLCD();

			error_t e = false;
			(lockVar32(POS1)) ? g_var32[POS1] = 0 : e = lockError(POS1, 32);  // Time of last touch
			(lockVar16(POS0)) ? g_var16[POS0] = 60 : e = lockError(POS0, 16); // LCD Print line starts at y coord 60
			(lockVar16(POS1)) ? g_var16[POS1] = 0 : e = lockError(POS1, 16);  // Initial x postition
			(lockVar16(POS2)) ? g_var16[POS2] = 0 : e = lockError(POS2, 16);  // Initial y postition
			(lockVar16(POS3)) ? g_var16[POS3] = 0 : e = lockError(POS3, 16);  // Last x postition
			(lockVar16(POS4)) ? g_var16[POS4] = 0 : e = lockError(POS4, 16);  // Last y postition
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8);     // Swipe input direction
			(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8);     // Bool to track initial x/y position
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = CANBUS_MAIN;
			}

			isSerialOut = false;
			hasDrawn = true;
		}

		// Call buttons or page method
		g_var8[POS0] = swipe(g_var32[POS1], g_var8[POS1], g_var16[POS1], g_var16[POS2], g_var16[POS3], g_var16[POS4]);
		if ((g_var8[POS0] == SWIPE_DOWN || g_var8[POS0] == SWIPE_UP) && !Touch_getXY())
		{
			g_var16[POS0] = 60;
			//drawReadInCANLCD();
		}
		if (g_var8[POS0] == SWIPE_RIGHT && !Touch_getXY())
		{
			state = 0;
			nextPage = 1;
			graphicLoaderState = 0;
		}

		if (!Touch_getXY())
		{
			//readInCANMsg(selectedChannelOut);
		}
		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS0);
			unlockVar8(POS1);
			unlockVar16(POS0);
			unlockVar16(POS1);
			unlockVar16(POS2);
			unlockVar16(POS3);
			unlockVar16(POS4);
			unlockVar32(POS1);
			pageTransition();
		}
		break;

	case 8: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method


		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case VEHTOOL_MAIN: /*========== VEHTOOL ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawVehicleTools())
			{
				break;
			}

			hasDrawn = true;
		}

		// Call buttons or page method
		//VehicleToolButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 10: // PIDSCAN
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawPIDSCAN())
			{
				break;
			}

			// Lock global variables
			error_t e = false;
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8);	 // VIN request state
			(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8);    // Loading bar index
			(lockVar8(POS2)) ? g_var8[POS2] = 1 : e = lockError(POS2, 8);    // PIDSCAN state
			(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16); // PID request range
			(lockVar16(POS1)) ? g_var16[POS1] = 0 : e = lockError(POS1, 16); // PID request bank 
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = VEHTOOL_MAIN;
			}

			// Filter out some unwanted traffic
			//can1.setFilterMask0(0x7E0, 0x7D0);

			// Draw the load bar
			loadBar(g_var8[POS1]++);

			state = 0;
			hasDrawn = true;
		}

		// Call buttons or page method
		//startPIDSCAN();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			//can1.setFilterMask0(0x000, 0x000);
			unlockVar8(POS0);
			unlockVar8(POS1);
			unlockVar8(POS2);
			unlockVar16(POS0);
			unlockVar16(POS1);
			pageTransition();
		}
		break;

	case 11: // PIDSTRM
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (hasPID == true)
			{
				if (1)//drawPIDStream())
				{
					break;
				}

				error_t e = false;
				(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8); // Draw scroll index
				(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16); // Selected PID to request
				(lockVar16(POS1)) ? g_var16[POS1] = 0 : e = lockError(POS1, 16); // PID samples counter
				(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32); // PID request timer
				(lockVar32(POS1)) ? g_var32[POS1] = 0 : e = lockError(POS1, 32); // No PIDSCAN timer
				if (e)
				{
					DEBUG_ERROR(F("Error: Variable locked"));
					delay(5000);
					graphicLoaderState = 0;
					nextPage = VEHTOOL_MAIN;
				}

				// Filter out some unwanted traffic
				//can1.setFilterMask0(0x7E0, 0x1F0);

				// Read in the PID scan results
				for (int i = 0; i < 80; i++)
				{
					arrayIn[i] = 0x00;
				}
				//sdCard.readFile(can1.getFullDir(), arrayIn);

				// Draw the read in results to screen
				//drawPIDStreamScroll();
			}
			else
			{
				drawErrorMSG2(F("Error"), F("Please Perform"), F("PIDSCAN First"));
				g_var32[POS1] = millis();
			}

			state = 0;
			hasDrawn = true;
		}

		if (hasPID)
		{
			// Call buttons or page method
			//streamPIDS();
		}
		else // Error message buttons
		{
			if (Touch_getXY())
			{
				if ((x >= 365) && (x <= 401))
				{
					if ((y >= 100) && (y <= 130))
					{
						nextPage = VEHTOOL_MAIN;
						break; // Added break to create a small delay preventing double tap
					}
				}
			}
			if (millis() - g_var32[POS1] > 10000)
			{
				nextPage = VEHTOOL_MAIN;
			}
		}

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			//can1.setFilterMask0(0x0, 0x0);
			unlockVar8(POS0);
			unlockVar16(POS0);
			unlockVar16(POS1);
			unlockVar32(POS0);
			pageTransition();
		}
		break;

	case 12: // PID Guages
		// Draw page and lock variables
		if (!hasDrawn)
		{
			//drawPIDGauges();
			hasDrawn = true;
			//can1.startPID();
		}

		// Call buttons or page method
		//PIDGauges();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 13: // VIN
		// Draw page and lock variables
		if (!hasDrawn)
		{
			/*
			if (state < 4 && !can1.VINReady())
			{
				isSerialOut = false;
				state = can1.requestVIN(state, false);
			}
			else
			{
				hasDrawn = true;
				drawVIN();
			}
			*/
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			//can1.setFilterMask0(0x0, 0x0);
			pageTransition();
		}
		break;

	case 14: // 
		// Draw page and lock variables
		if (!hasDrawn)
		{

			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 15: // DTC
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawDTC())
			{
				break;
			}
			// Lock global variables
			error_t e = false;
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8); // Clear DTC state
			(lockVar16(POS1)) ? g_var16[POS1] = 0 : e = lockError(POS1, 16); // Loadbar divider
			(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32); // Send clear message timer
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = VEHTOOL_MAIN;
			}

			// Initialize state machine variables to 0
			hasDrawn = true;
			state = 0;
		}

		// Call buttons or page method
		//TC();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS0);
			unlockVar16(POS1);
			unlockVar32(POS0);
			pageTransition();
		}
		break;

	case 16:
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 17:
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}
		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case UTVTOOL_MAIN: /*========== UTV TOOLS ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawRZRTOOL())
			{
				break;
			}

			hasDrawn = true;
		}

		// Call buttons or page method
		//RZRToolButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 19: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 20: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 21: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 22: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 23: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 24: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 25: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 26: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case TESTING_MAIN: /*========== EXTRAFN ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawExtraFN())
			{
				break;
			}

			hasDrawn = true;
		}

		// Call buttons or page method
		//extraFNButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 28: // OBD CAN Simulator
		// Draw page and lock variables
		if (!hasDrawn)
		{
			//drawOBDSimulator();
			hasDrawn = true;
		}

		// Call buttons or page method
		//entry();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 29: // Message Spam
		// Draw page and lock variables
		// TODO: Clean up this mess / Move to testing
		if (!hasDrawn)
		{
			if (graphicLoaderState == 1)
			{
				// Lock global variables
				error_t e = false;
				(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8);        // Keypad index
				(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16);     // ID
				(lockVar16(POS1)) ? g_var16[POS1] = 0x7FF : e = lockError(POS1, 16); // Max
				(lockVar16(POS2)) ? g_var16[POS2] = 0x000 : e = lockError(POS2, 16); // Min
				(lockVar16(POS3)) ? g_var16[POS3] = 30 : e = lockError(POS3, 16);    // Interval (ms)
				(lockVar16(POS4)) ? g_var16[POS4] = 30 : e = lockError(POS4, 16);    // Keypad total
				(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32);     // Timer
				if (e)
				{
					DEBUG_ERROR(F("Error: Variable locked"));
					delay(5000);
					graphicLoaderState = 0;
					nextPage = TESTING_MAIN;
				}
			}

			switch (graphicLoaderState)
			{
			case 0:
				break;
			case 1:
				// Above ^^
				break;
			case 2:
				//drawMSGSpam();
				break;
			case 3:
				//drawMSGSpamMin();
				break;
			case 4:
				//drawMSGSpamMax();
				break;
			case 5:
				//drawMSGInterval();
				break;
			case 6:
				hasDrawn = true;
				break;
			}
			graphicLoaderState++;
			return;
		}

		// Call buttons or page method
		if (state == 0)
		{
			//MSGSpam();
			//sendMSGButtons();
			if (state == 1)
			{
				//drawKeypadDec();
				//g_var8[POS1] = 0;
				//g_var16[POS4] = 0;
				//keypadInput[0] = 0;
				//keypadInput[1] = 0;
				//keypadInput[2] = 0;
			}
			if (state == 2 || state == 3)
			{
				//drawKeypad();
				//g_var8[POS1] = 0;
				//g_var16[POS4] = 0;
				//keypadInput[0] = 0;
				//keypadInput[1] = 0;
				//keypadInput[2] = 0;
			}
		}
		if (state == 1)
		{
			//MSGSpam();
			uint8_t input = keypadControllerDec(g_var8[POS1], g_var16[POS4]);
			if (input == 0xF1)
			{
				const uint8_t RUN_TIME_OFFSET_MS = 1; // Approximate time it takes send out a single messagein ms
				g_var16[POS3] = g_var16[POS4] - RUN_TIME_OFFSET_MS;
				state = 0;
				hasDrawn = false;
				graphicLoaderState = 2;
			}
			if (input == 0xF0)
			{
				state = 0;
				hasDrawn = false;
				graphicLoaderState = 2;
			}
		}
		if (state == 2)
		{
			//MSGSpam();
			uint8_t input = keypadController(g_var8[POS1], g_var16[POS4]);
			if (input == 0xF1)
			{
				g_var16[POS2] = g_var16[POS4];
				state = 0;
				hasDrawn = false;
				graphicLoaderState = 2;
			}
			if (input == 0xF0)
			{
				state = 0;
				hasDrawn = false;
				graphicLoaderState = 2;
			}
		}
		if (state == 3)
		{
			//MSGSpam();
			uint8_t input = keypadController(g_var8[POS1], g_var16[POS4]);
			if (input == 0xF1)
			{
				g_var16[POS1] = g_var16[POS4];
				state = 0;
				hasDrawn = false;
				graphicLoaderState = 2;
			}
			if (input == 0xF0)
			{
				state = 0;
				hasDrawn = false;
				graphicLoaderState = 2;
			}
		}

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS1);
			unlockVar16(POS0);
			unlockVar16(POS1);
			unlockVar16(POS2);
			unlockVar16(POS3);
			unlockVar16(POS4);
			unlockVar32(POS0);
			pageTransition();
		}
		break;

	case 30: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 31: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 32: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 33: //
		// Draw page and lock variables
		if (!hasDrawn)
		{
			isSerialOut = true;
			selectedChannelOut = 6;
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 34: // Ford Dongle
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawDongleSimFord())
			{
				break;
			}

			error_t e = false;
			(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32);
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = TESTING_MAIN;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		//dongleSimButtonsFord();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar32(POS0);
			pageTransition();
		}
		break;

	case 35: // GM Dongle
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawDongleSim())
			{
				break;
			}

			error_t e = false;
			(lockVar32(POS0)) ? g_var32[POS0] = 0 : e = lockError(POS0, 32);
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = TESTING_MAIN;
			}

			hasDrawn = true;
		}

		// Call buttons or page method
		//dongleSimButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar32(POS0);
			pageTransition();
		}
		break;

	case SETTING_MAIN: /*========== SETTINGS ==========*/
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawSettings())
			{
				break;
			}

			hasDrawn = true;
		}

		// Call buttons or page method
		//settingsButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 37: // Memory
		// Draw page and lock variables
		if (!hasDrawn)
		{
			//memoryUse();
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 38: // About
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawAbout())
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

	case 39: // WiFi MAC Address
		// Draw Page
		// TODO: make a non-blocking state machine
		// TODO: Organize Serial3 commands
		if (!hasDrawn)
		{
			if (isSerialOut && selectedChannelOut == 6)
			{
				// Cant get mac while streaming CAN Bus data over WiFi
				// Can be fixed by printing all at once instead
				hasDrawn = true;
				break;
			}

			// Send command
			Serial3.write(0xAC);
			delay(1);
			Serial3.write(0xAD);
			delay(1);
			if (1)//drawMACAddress())
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

	case 40: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 41: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 42: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			hasDrawn = true;
		}

		// Call buttons or page method

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;

	case 43: // Connect Dongle
		// Draw page and lock variables
		if (!hasDrawn)
		{
			error_t e = false;
			(lockVar8(POS0)) ? g_var8[POS0] = 0 : e = lockError(POS0, 8);    // Selected index
			(lockVar8(POS1)) ? g_var8[POS1] = 0 : e = lockError(POS1, 8);    // User input
			(lockVar8(POS2)) ? g_var8[POS2] = 0 : e = lockError(POS2, 8);    // Keypad index
			(lockVar8(POS3)) ? g_var8[POS3] = 0 : e = lockError(POS3, 8);    // Scroll index
			(lockVar8(POS4)) ? g_var8[POS4] = 0 : e = lockError(POS4, 8);    // Node position
			(lockVar8(POS5)) ? g_var8[POS5] = 0 : e = lockError(POS5, 8);    // Keyboard index
			(lockVar16(POS0)) ? g_var16[POS0] = 0 : e = lockError(POS0, 16); // Total value
			if (e)
			{
				DEBUG_ERROR(F("Error: Variable locked"));
				delay(5000);
				graphicLoaderState = 0;
				nextPage = SETTING_MAIN;
			}
			state = 0;
			hasDrawn = true;
		}
		// Call buttons or page method
		//connectDongle();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			unlockVar8(POS0);
			unlockVar8(POS1);
			unlockVar8(POS2);
			unlockVar8(POS3);
			unlockVar8(POS4);
			unlockVar8(POS5);
			unlockVar16(POS0);
			pageTransition();
		}
		break;

	case 44: // Unused
		// Draw page and lock variables
		if (!hasDrawn)
		{
			if (1)//drawClockSpeed())
			{
				break;
			}
			hasDrawn = true;
		}

		// Call buttons or page method
		//clockSpeedButtons();

		// Release any variable locks if page changed
		if (nextPage != page)
		{
			pageTransition();
		}
		break;
	}
}

void drawCANBus() 
{
    drawSquareBtn(131, 55, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
    drawRoundBtn(140, 80, 305, 130, F("Capture"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(310, 80, 475, 130, F("Send"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(140, 135, 305, 185, F("Cap Files"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(310, 135, 475, 185, F("Baud"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(140, 190, 305, 240, F("FilterMask"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(310, 190, 475, 240, F("Auto Baud"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(140, 245, 305, 295, F(""), menuBackground, menuBtnBorder, menuBtnText, ALIGN_CENTER);
    drawRoundBtn(310, 245, 475, 295, F(""), menuBackground, menuBtnBorder, menuBtnText, ALIGN_CENTER);
}

void buttonMonitor()
{
	if (Touch_getXY())
	{
		for (uint8_t i = 0; i < buttonOnPage; i++)
		{
			if ((x >= userInterfaceButtons[i].getXStart()) && (x <= userInterfaceButtons[i].getXStop()))
			{
				if ((y >= userInterfaceButtons[i].getYStart()) && (y <= userInterfaceButtons[i].getXStop()))
				{
					// CANBUS
					waitForIt(userInterfaceButtons[i].getXStart(), userInterfaceButtons[i].getYStart(), userInterfaceButtons[i].getXStop(), userInterfaceButtons[i].getYStop());
					if (userInterfaceButtons[i].getUsingPage())
					{
						nextPage = userInterfaceButtons[i].getPage();
					}
					else
					{
						userInterfaceButtons[i].callFunction();
					}
					graphicLoaderState = 0;
				}
			}
		}
	}
}

void drawMenu() 
{
    // Draw Layout
    drawSquareBtn(0, 0, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
    drawSquareBtn(0, 0, 130, 319, "", menuBackground, menuBackground, menuBackground, ALIGN_CENTER);

    // Draw Menu Buttons
    drawRoundBtn(5, 32, 125, 83, F("CANBUS"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	userInterfaceButtons[0].setButton(5, 32, 125, 83, true, CANBUS_MAIN, NULL);
    drawRoundBtn(5, 88, 125, 140, F("VEHTOOL"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	userInterfaceButtons[1].setButton(5, 88, 125, 140, true, VEHTOOL_MAIN, NULL);
    drawRoundBtn(5, 145, 125, 197, F("UTVTOOL"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	userInterfaceButtons[2].setButton(5, 145, 125, 197, true, UTVTOOL_MAIN, NULL);
    drawRoundBtn(5, 202, 125, 254, F("TESTING"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	userInterfaceButtons[3].setButton(5, 202, 125, 254, true, TESTING_MAIN, NULL);
    drawRoundBtn(5, 259, 125, 312, F("SETTING"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	userInterfaceButtons[4].setButton(5, 259, 125, 312, true, SETTING_MAIN, NULL);
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

    drawMenu();
    drawCANBus();
}

// Resets variables for page change
void pageTransition()
{
    hasDrawn = false;
    graphicLoaderState = 0;
    page = nextPage;
}

// Button functions for the main menu
void menuButtons()
{
    // Touch screen controls
    if (Touch_getXY())
    {
        if ((x >= 5) && (x <= 125))
        {
            if ((y >= 32) && (y <= 83))
            {
                // CANBUS
                waitForIt(5, 32, 125, 83);
                nextPage = 0;
                graphicLoaderState = 0;
            }
            if ((y >= 88) && (y <= 140))
            {
                // VEHTOOL
                waitForIt(5, 88, 125, 140);
                nextPage = 9;
                graphicLoaderState = 0;
            }
            if ((y >= 145) && (y <= 197))
            {
                // RZRTOOL
                waitForIt(5, 145, 125, 197);
                nextPage = 18;
                graphicLoaderState = 0;
            }
            if ((y >= 202) && (y <= 254))
            {
                // EXTRAFN
                waitForIt(5, 202, 125, 254);
                nextPage = 27;
                graphicLoaderState = 0;
            }
            if ((y >= 259) && (y <= 312))
            {
                // SETTING
                waitForIt(5, 259, 125, 312);
                nextPage = 36;
                graphicLoaderState = 0;
            }
        }
    }
}

// All background process should be called from here
void backgroundProcess()
{
    // TODO
	buttonMonitor();
    //menuButtons();
    //updateTime();
    //serialOut();
    //SDCardOut();
    //timedTXSend();
}

// Main loop
void loop(void)
{
    // GUI
    //pageControl();

    // Background Processes
    backgroundProcess();
}  
