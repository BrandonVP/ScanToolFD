/*
 ===========================================================================
 Name        : KeyInput.h
 Author      : Brandon Van Pelt
 Company     : HP Tuners
 Created	 : 11/08/2022
 Description : User key inputs
 ===========================================================================
 */

#ifndef _KEYINPUT_H
#define _KEYINPUT_H

#include "common.h"

typedef enum key_returns {
	KEY_CHANGE = 0xF3,
	KEY_ACCEPT = 0xF1,
	KEY_CANCEL = 0xF0,
	KEY_NO_CHANGE = 0xFF
}Key_Returns;

#ifdef _KEYINPUT_C

uint8_t KEYINPUT_createKeyboardButtons(void);
uint8_t KEYINPUT_createNumpadButtons(int offset);

#else

extern uint8_t keyPadButtons;
extern UserInterfaceClass userKeyButtons[43];

uint8_t KEYINPUT_createKeyboardButtons(void);
uint8_t KEYINPUT_createNumpadButtons(int offset);
uint8_t KEYINPUT_createHexpadButtons(int offset);
uint8_t KEYINPUT_keypadController(uint16_t& total, uint8_t& numIndex, uint8_t maxSize);
uint8_t KEYINPUT_keypadHexController(uint32_t& total, uint8_t& numIndex, uint8_t maxSize);
void KEYINPUT_clearInput();

#endif // _KEYINPUT_C
#endif // _KEYINPUT_H
