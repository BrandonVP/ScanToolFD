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

#define TIMED_TX_MAX_SIZE 20

struct MessageRX
{
	char name[9];
	uint8_t channel = 0;
	uint16_t interval = 0;
	uint32_t id = 0;
	uint32_t timer = 0;
	uint8_t data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bool isOn = false;
	bool isDel = true;
};

struct SchedulerRX
{
	uint8_t nodeLength = 0;
	MessageRX node[TIMED_TX_MAX_SIZE];
};

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
	BTN_config_button_count
}Capture_Config_BTN;

#ifdef _CANBUSCAPTURE_C

extern cbBuffer can1Buffer;
extern cbBuffer can2Buffer;
extern cbBuffer can3Buffer;
extern bool enableCB1;
extern bool enableCB2;
extern bool enableCB3;

uint8_t CAPTURE_input_config = 0;
uint8_t CAPTURE_output_config = 0;

uint8_t CAPTURE_createMenuBtns();
uint8_t CAPTURE_createCaptureBtns();
void CAPTURE_captureConfig(int userInput);

#else

extern uint8_t CAPTURE_input_config;
extern uint8_t CAPTURE_output_config;

void CAPTURE_clearLocalVar();
uint8_t CAPTURE_createMenuBtns();
uint8_t CAPTURE_createCaptureBtns();
uint8_t CAPTURE_createLCDBtns();
uint8_t CAPTURE_createBaudBtns();
void CAPTURE_captureConfig(int userInput);
void CAPTURE_LCD_scan(int userInput);

#endif // _CANBUSCAPTURE_C
#endif // _CANBUSCAPTURE_h

