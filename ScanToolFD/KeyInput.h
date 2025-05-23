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
	KEY_CHANGE = 0xF1,
	KEY_DEL = 0xF2,
	KEY_ACCEPT = 0xF3,
	KEY_CANCEL = 0xF4,
	KEY_NO_CHANGE = 0xF5
}Key_Returns;

#ifdef _KEYINPUT_C

uint8_t KEYINPUT_createKeyboardButtons(void);
uint8_t KEYINPUT_createNumpadButtons(int offset);

#else

extern uint8_t keyPadButtons;
extern UserInterfaceClass userKeyButtons[43];

void KEYINPUT_clearInput();
uint8_t KEYINPUT_createKeyboardButtons(void);
uint8_t KEYINPUT_createNumpadButtons(int offset);
uint8_t KEYINPUT_createHexpadButtons(int offset);
uint8_t KEYINPUT_keypadController(uint16_t& total, uint8_t& numIndex, uint8_t maxSize);
uint8_t KEYINPUT_keypadHexController(uint32_t& total, uint8_t& numIndex, uint8_t maxSize);
uint8_t KEYINPUT_keyboardController(uint8_t& index, char* keyboardInput);

#endif // _KEYINPUT_C
#endif // _KEYINPUT_H
