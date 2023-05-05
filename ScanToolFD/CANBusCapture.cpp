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
	userInterfaceButtons[btnPos++].setButton(140, 80, 305, 130, true, 1, NULL, F("CANBUS"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(310, 80, 475, 130, true, 2, NULL, F("Send"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(140, 135, 305, 185, true, 0, NULL, F("Cap Files"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(310, 135, 475, 185, true, 0, NULL, F("Baud"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(140, 190, 305, 240, true, 0, NULL, F("FilterMask"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(310, 190, 475, 240, true, 0, NULL, F("Auto Baud"), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(140, 245, 305, 295, true, 0, NULL, F(""), ALIGN_CENTER);
	userInterfaceButtons[btnPos++].setButton(310, 245, 475, 295, true, 0, NULL, F(""), ALIGN_CENTER);
}
