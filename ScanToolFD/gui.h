/*
 ===========================================================================
 Name        : gui.h
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description : 
 ===========================================================================
 */

#ifndef _GUI_H
#define _GUI_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef _GUI_C

#include "userInterface.h"
#include "definitions.h"
#include <ILI9488_t3.h>      // Display
#include <Adafruit_FT6206.h> // Touch

extern UserInterfaceClass userInterfaceButtons[40];
extern uint16_t buttonsOnPage;
extern Adafruit_FT6206 ts;
extern ILI9488_t3 display;
extern void backgroundProcess();

void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, bool usingPage, uint8_t page, void* function);
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, bool usingPage, uint8_t page, void* function);
void waitForIt(int x_start, int y_start, int x_stop, int y_stop);
void waitForItRect(int x_start, int y_start, int x_stop, int y_stop);

#else

extern void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
extern void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, bool usingPage, uint8_t page, void* function);
extern void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
extern void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, bool usingPage, uint8_t page, void* function);
extern void waitForIt(int x_start, int y_start, int x_stop, int y_stop);
extern void waitForItRect(int x_start, int y_start, int x_stop, int y_stop);

#endif // _GUI_C
#endif // _GUI_H