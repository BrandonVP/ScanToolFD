/*
 Name:    CANBusCapture.h
 Created: 11/15/2020 8:54:18 AM
 Author:  Brandon Van Pelt
*/

#include "gui.h"
#define APP_CANBUS_TOOLS 0
#define APP_CAPTURE 1

#ifndef _CANBUSCAPTURE_h
#define _CANBUSCAPTURE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef _CANBUSCAPTURE_C
#include "config.h"
#include "common.h"
#include "userInterface.h"

extern UserInterfaceClass userInterfaceButtons[APP_BUTTON_SIZE];
extern void clearAppSpace();
void CAPTURE_createMenuBtns();
void CAPTURE_createCaptureBtns();
void CAPTURE_captureConfig();
#else

void CAPTURE_createMenuBtns();
void CAPTURE_createCaptureBtns();
void CAPTURE_captureConfig();

#endif
#endif

