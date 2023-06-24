/*
 ===========================================================================
 Name        : RGB_LED.h
 Author      : Brandon Van Pelt
 Created	 : 6/23/2023
 Description : Control the on board LED
 ===========================================================================
 */

#ifndef _RGB_LED_H
#define _RGB_LED_H

enum RGB
{
	LED_RED = 1,
	LED_BLUE = 2,
	LED_GREEN = 3,
	LED_PURPLE = 4,
	LED_YELLOW = 5,
	LED_TEAL = 6,
	LED_OFF = 7,
};

#define LED_R 24
#define LED_B 26
#define LED_G 25
#define ON LOW
#define OFF HIGH

#ifdef _RGB_LED_CPP

#include "common.h"
#define LED_STROBE_INTERVAL        (20)
#define COLOR_BUFFER_SIZE		   (0x1F)
bool cycleLED = false;
uint32_t LEDTimer = 0;
void LED_RGB(RGB color);

#else 

void LED_initialize(void);
void LED_RGB(RGB color);
void LED_strobe(void);
void LED_strobe(RGB color);
void LED_pulse(RGB color);
void LED_fast_strobe(RGB color);

#endif // _RGB_LED_CPP
#endif // _RGB_LED_H
