/*
 Name:    CANBusCapture.h
 Created: 11/15/2020 8:54:18 AM
 Author:  Brandon Van Pelt
*/

#include "gui.h"

#define APP_CANBUS_TOOLS 0
#define APP_CAPTURE_CONFIG 1
#define APP_CAPTURE_LCD 2
#define APP_CAPTURE_FILES 3
#define APP_CAPTURE_BAUD 4
#define APP_CAPTURE_SEND 5
#define APP_CAPTURE_FILTERMASK 6
#define APP_CAPTURE_AUTOBAUD 7

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

extern uint8_t CAPTURE_input_config;
extern uint8_t CAPTURE_output_config;

void CAPTURE_createMenuBtns();
void CAPTURE_createCaptureBtns();
void CAPTURE_captureConfig();
void CAPTURE_createLCDBtns();
void CAPTURE_createBaudBtns();

#endif
#endif

