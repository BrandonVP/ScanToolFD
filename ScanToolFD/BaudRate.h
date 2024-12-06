/*
 ===========================================================================
 Name        : baudRate.h
 Author      : Brandon Van Pelt
 Created	 : 12/6/2024 
 Description : Set CAN Bus baud rates
 ===========================================================================
 */

#ifndef _BAUDRATE_H
#define _BAUDRATE_H

#include "common.h"

typedef enum baud_btn {
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

#ifdef _BAUDRATE_C

uint8_t BAUD_createBaudBtns();
void BAUD_Baud(int userInput);
int BAUD_convertBaudRate(int baud);
uint32_t BAUD_getBaudRate1();
uint32_t BAUD_getBaudRate2();
uint32_t BAUD_getBaudRateFD();

#else

uint8_t BAUD_createBaudBtns();
void BAUD_Baud(int userInput);
int BAUD_convertBaudRate(int baud);
uint32_t BAUD_getBaudRate1();
uint32_t BAUD_getBaudRate2();
uint32_t BAUD_getBaudRateFD();

#endif // _BAUDRATE_C
#endif // _BAUDRATE_H
