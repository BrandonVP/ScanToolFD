/*
 Name:    CANBusCapture.cpp
 Created: 11/15/2020 8:54:18 AM
 Author:  Brandon Van Pelt
*/

#include "CANBusCapture.h"
#include "config.h"
#include "userInterface.h"

#define APP_BUTTON_SIZE 30
extern UserInterfaceClass userInterfaceButtons[APP_BUTTON_SIZE];

void createCANBusBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButtons[btnPos++].setButton(55, 80, 220, 120, 1, true, F("Capture"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 80, 425, 120, 2, true, F("Send"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 140, 220, 180, 3, true, F("Files"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 140, 425, 180, 0, true, F("Baud"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 200, 220, 240, 0, true, F("Filter Mask"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Auto Baud"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(55, 260, 220, 300, 0, true, F(""), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(260, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
}
