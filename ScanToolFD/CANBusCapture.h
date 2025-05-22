/*
 ===========================================================================
 Name        : CANBusCapture.h
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus functions
 ===========================================================================
 */

#ifndef _CANBUSCAPTURE_h
#define _CANBUSCAPTURE_h

#include "common.h"
#include "cbBuffer.h"
#include "cbBufferFD.h"
#include "BaudRate.h"
#include "icons.h"

typedef enum capture_lcd_btn {
	BTN_capture_noInput = 1,
	BTN_capture_start,
	BTN_capture_stop,
	BTN_capture_clear,
	BTN_capture_config,
	BTN_capture_back
}Capture_LCD_BTN;

typedef enum capture_config_btn {
	BTN_config_input_C1 = 4,
	BTN_config_input_C2,
	BTN_config_input_C3,
	BTN_config_input_Wireless,
	BTN_config_input_C2tx,
	BTN_config_output_LCD,
	BTN_config_output_Serial,
	BTN_config_output_SDCard,
	BTN_config_output_Wireless,
	BTN_config_state_Start,
	BTN_config_state_Stop,
	BTN_config_state_Filename,
	BTN_config_state_Filename_Accept,
	BTN_config_button_count,
	BTN_config_state_keyInput
	
}Capture_Config_BTN;




typedef enum capture_filterStates {
	BTN_filterState_print,
	BTN_filterState_min,
	BTN_filterState_max,
}Capture_FilterStates;
#ifdef _CANBUSCAPTURE_C

extern cbBuffer can1Buffer;
extern cbBuffer can2Buffer;
extern cbBufferFD can3Buffer;
extern bool enableCB1;
extern bool enableCB2;
extern bool enableCB3;

uint8_t CAPTURE_input_config = 0;
uint8_t CAPTURE_output_config = 0;

Icons* CAPTURE_iconPtr = NULL;

void CAPTURE_setPtrs(Icons* icons);
void CAPTURE_clearLocalVar();
uint8_t CAPTURE_createCaptureBtns();
uint8_t CAPTURE_createLCDBtns();
void CAPTURE_enableDisableConfigBtn(bool isEnabled);
void CAPTURE_processSerialCapture();
void CAPTURE_processSDCapture(int userInput);
void CAPTURE_processWirelessCapture();
void CAPTURE_printFilenameBtns();
void CAPTURE_removeFilenameBtn();
void CAPTURE_activateStartBtn();
void CAPTURE_deactivateStartBtn();
void CAPTURE_activateStopBtn();
void CAPTURE_deactivateStopBtn();
void CAPTURE_captureConfig(int userInput);
void CAPTURE_LCD_clear();
void CAPTURE_LCD_Print(uint32_t id, uint8_t length, uint8_t* data);
void CAPTURE_LCD_scan(int userInput);

#else

extern uint8_t CAPTURE_input_config;
extern uint8_t CAPTURE_output_config;
extern bool CAPTURE_isRunning;

void CAPTURE_setPtrs(Icons* icons);
void CAPTURE_clearLocalVar();
uint8_t CAPTURE_createCaptureBtns();
uint8_t CAPTURE_createLCDBtns();
void CAPTURE_processSerialCapture();
void CAPTURE_processSDCapture(int userInput);
void CAPTURE_processWirelessCapture();
void CAPTURE_captureConfig(int userInput);
void CAPTURE_LCD_scan(int userInput);
uint8_t CAPTURE_drawCANLogScroll();

#endif // _CANBUSCAPTURE_C
#endif // _CANBUSCAPTURE_h

