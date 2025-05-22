/*
===========================================================================
Name        : gui.h
Author      : Brandon Van Pelt
Created	    : 4/6/2023
Description : GUI framework
===========================================================================
*/

#ifndef GUI_H
#define GUI_H

#include "userInterface.h"
#include "common.h"
#include "App.h"

// Forward declaration of App
typedef struct App App;

enum textAlignment
{
	GUI_Align_Text_Left = 1,
	GUI_Align_Text_Center = 2,
	GUI_Align_Text_Right = 3
};

#ifdef GUI_C // Private

App* GUI_appPtr = NULL;

int x, y;
bool GUI_isTouchedMenu;
bool GUI_isTouchedBody;
uint8_t GUI_graphicLoaderState;
static uint16_t GUI_loadBarIndex = 471; // ends at 470
static uint32_t GUI_loadBarTimer = 0;
static uint16_t GUI_loarBarRunTimeMS = 8000;
static char GUI_printString[14][64];

void GUI_setAppPtr(App* app);
bool GUI_Touch_getXY();
void GUI_setTouchedMenu(bool isTouched);
void GUI_setTouchedBody(bool isTouched);
bool GUI_getTouchedMenu();
bool GUI_getTouchedBody();
void GUI_updateButton(int index);
void GUI_updateButtonText(int index, String newText);
void GUI_drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, int radius);
void GUI_drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void GUI_waitForIt(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC);
void GUI_waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC);
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint16_t size);
int GUI_subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size);
void GUI_setTextSize(int textSize);
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint);
void GUI_clearAppSpace();
void GUI_setLoadBarRuntimeMS(uint16_t time);
void GUI_startLoadBar(void);
void GUI_stopLoadBarTimed(void);
void GUI_sideLoadBarTimed(void);
void GUI_loadBarPrintFrame(void);
void GUI_LoadBarPrint(String buttonText, uint8_t fontSize, uint16_t textColor);
void GUI_LoadBarPrint(String buttonText);
void GUI_loadBarPrintFailed(void);
void GUI_loadBarPrintPass(void);
void GUI_drawSettingsPopUp(void);
void GUI_clearSettingsPopUp(void);
void GUI_LCD_Terminal(String buttonText);
void GUI_clear_LCD_Terminal();

#else // Public

extern int x, y;
extern uint8_t GUI_graphicLoaderState;

void GUI_setAppPtr(App* app);
void GUI_setTouchedMenu(bool isTouched);
void GUI_setTouchedBody(bool isTouched);
bool GUI_getTouchedMenu();
bool GUI_getTouchedBody();
void GUI_updateButton(int index);
void GUI_updateButtonText(int index, String newText);
void GUI_drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, int radius);
void GUI_drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint16_t size);
int GUI_subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size);
void GUI_setTextSize(int textSize);
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint);
void GUI_clearAppSpace(void);
void GUI_isButtonsEnabled(UserInterfaceClass* buttons, uint8_t start, uint8_t end, bool isDisabled);
void GUI_setLoadBarRuntimeMS(uint16_t time);
void GUI_startLoadBar(void);
void GUI_stopLoadBarTimed(void);
void GUI_sideLoadBarTimed(void);
void GUI_loadBarPrintFrame(void);
void GUI_LoadBarPrint(String buttonText, uint8_t fontSize, uint16_t textColor);
void GUI_LoadBarPrint(String buttonText);
void GUI_loadBarPrintFailed(void);
void GUI_loadBarPrintPass(void);
void GUI_drawSettingsPopUp(void);
void GUI_clearSettingsPopUp(void);
void GUI_LCD_Terminal(String buttonText);
void GUI_clear_LCD_Terminal();

#endif // GUI_C
#endif // GUI_H