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

#include "userInterface.h"

#ifdef _GUI_C

#include "config.h"
#include <ILI9488_t3.h>      // Display
#include <Adafruit_FT6206.h> // Touch

uint8_t nextPage = 0;
uint8_t page = 0;

extern UserInterfaceClass userInterfaceButtons[40];
extern uint16_t buttonsOnPage;
extern Adafruit_FT6206 ts;
extern ILI9488_t3 display;
extern void backgroundProcess();
extern bool Touch_getXY();
extern int x, y;
extern uint8_t nextPage;
extern uint8_t graphicLoaderState;

void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, int radius);
void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, bool usingPage, uint8_t page, void* function);
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, bool usingPage, uint8_t page, void* function);
void waitForIt(int x_start, int y_start, int x_stop, int y_stop);
void waitForItRect(int x_start, int y_start, int x_stop, int y_stop);
void buttonMonitor(UserInterfaceClass* buttons, uint8_t size);

#else
// TODO extern?
void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, int radius);
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void waitForIt(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC);
void waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC);
void buttonMonitor(UserInterfaceClass* buttons, uint8_t size);
int subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size);
bool drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint);
void drawButton(UserInterfaceClass* button, uint16_t buttonColor, uint16_t buttonBorderColor, uint16_t buttonTextColor);
extern uint8_t nextPage;
extern uint8_t page;
#endif // _GUI_C
#endif // _GUI_H