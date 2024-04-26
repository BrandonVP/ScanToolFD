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

typedef enum capture_baud_btn {
	BTN_baud_label1,
	BTN_baud_can1,
	BTN_baud_label2,
	BTN_baud_can2,
	BTN_baud_label3,
	BTN_baud_can3,
	BTN_baud_canSet,
	BTN_baud_table,
	BTN_baud_5m,
	BTN_baud_2m,
	BTN_baud_1m,
	BTN_baud_800k,
	BTN_baud_500k,
	BTN_baud_250k,
	BTN_baud_125k,
	BTN_baud_100k,
	BTN_baud_button_count
}Capture_Baud_BTN;

typedef enum capture_filterMask_btn {
	BTN_filterMask_CANLabel,    // Turned OFF
	BTN_filterMask_CAN1Label,   // Turned OFF
	BTN_filterMask_CAN2Label,   // Turned OFF
	BTN_filterMask_CAN3Label,   // Turned OFF
	BTN_filterMask_WiFiLabel,   // Turned OFF

	BTN_filterMask_filterLabel, // Turned OFF
	BTN_filterMask_filter1,
	BTN_filterMask_filter2,
	BTN_filterMask_filter3,
	BTN_filterMask_filterWiFi,

	BTN_filterMask_maskLabel,   // Turned OFF
	BTN_filterMask_mask1,
	BTN_filterMask_mask2,
	BTN_filterMask_mask3,
	BTN_filterMask_maskWiFi,

	BTN_filterMask_openLabel,   // Turned OFF
	BTN_filterMask_open1,
	BTN_filterMask_open2,
	BTN_filterMask_open3,
	BTN_filterMask_openWifi,    // Turned OFF

	BTN_filterMask_button_count // Turned OFF
}Capture_FilterMask_BTN;


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

void CAPTURE_clearLocalVar();
uint8_t CAPTURE_createMenuBtns();
uint8_t CAPTURE_createCaptureBtns();
uint8_t CAPTURE_createLCDBtns();
uint8_t CAPTURE_createBaudBtns();
uint8_t CAPTURE_createFilterMaskBtns();
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
void CAPTURE_Baud(int userInput);
void CAPTURE_CANBus(int userInput);

#else

extern uint8_t CAPTURE_input_config;
extern uint8_t CAPTURE_output_config;
extern bool CAPTURE_isRunning;
extern uint32_t CAPTURE_CANBusFDBaudRate;

void CAPTURE_clearLocalVar();
uint8_t CAPTURE_createMenuBtns();
uint8_t CAPTURE_createCaptureBtns();
uint8_t CAPTURE_createLCDBtns();
uint8_t CAPTURE_createBaudBtns();
uint8_t CAPTURE_createFilterMaskBtns();
void CAPTURE_processSerialCapture();
void CAPTURE_processSDCapture(int userInput);
void CAPTURE_processWirelessCapture();
void CAPTURE_captureConfig(int userInput);
void CAPTURE_LCD_scan(int userInput);
void CAPTURE_Baud(int userInput);
uint8_t CAPTURE_drawCANLogScroll();
void CAPTURE_filterMask(int userInput);
void CAPTURE_CANBus(int userInput);

#endif // _CANBUSCAPTURE_C
#endif // _CANBUSCAPTURE_h

