/*
 ===========================================================================
 Name        : CANBusCapture.h
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus functions
 ===========================================================================
 */

#define APP_CANBUS_TOOLS 0
#define APP_CAPTURE_CONFIG 1
#define APP_CAPTURE_LCD 2
#define APP_CAPTURE_FILES 3
#define APP_CAPTURE_BAUD 4
#define APP_CAPTURE_SEND 5
#define APP_CAPTURE_FILTERMASK 6
#define APP_CAPTURE_AUTOBAUD 7

#define CAPTURE_START_POS 13
#define CAPTURE_STOP_POS 14

#ifndef _CANBUSCAPTURE_h
#define _CANBUSCAPTURE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef _CANBUSCAPTURE_C

#include "common.h"



uint8_t CAPTURE_input_config = 0;
uint8_t CAPTURE_output_config = 0;

uint8_t CAPTURE_createMenuBtns();
uint8_t CAPTURE_createCaptureBtns();
void CAPTURE_captureConfig(int userInput);

#else

extern uint8_t CAPTURE_input_config;
extern uint8_t CAPTURE_output_config;

uint8_t CAPTURE_createMenuBtns();
uint8_t CAPTURE_createCaptureBtns();
void CAPTURE_captureConfig(int userInput);
void CAPTURE_createLCDBtns();
void CAPTURE_createBaudBtns();

#endif // _CANBUSCAPTURE_C
#endif // _CANBUSCAPTURE_h

