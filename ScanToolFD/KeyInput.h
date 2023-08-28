/*
 ===========================================================================
 Name        : KeyInput.h
 Author      : Brandon Van Pelt
 Created	 : 11/08/2022 9:57:18 AM
 Description : User key inputs
 ===========================================================================
 */

#include "common.h"

#ifndef _KEYINPUT_H
#define _KEYINPUT_H

#define KEY_ACCEPT    0xF1
#define KEY_CANCEL    0xF0
#define KEY_NO_CHANGE 0xFF

#ifdef _KEYINPUT_C

UserInterfaceClass userKeyButtons[43];
//uint8_t keyboardController(uint8_t&);

#else

extern UserInterfaceClass userKeyButtons[43];
uint8_t KEYINPUT_createKeyboardButtons(void);
uint8_t KEYINPUT_createUpperCaseButtons(void);
uint8_t KEYINPUT_createHexpadButtons(void);
uint8_t KEYINPUT_createNumpadButtons(void);
uint8_t keypadController(uint8_t& total, uint8_t buttonsOnPage);

#endif // _KEYINPUT_C
#endif // _KEYINPUT_H
