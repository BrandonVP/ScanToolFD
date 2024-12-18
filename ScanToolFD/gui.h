/*
 ===========================================================================
 Name        : gui.h
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description : GUI framework
 ===========================================================================
 */


#ifndef _GUI_H
#define _GUI_H

#include "common.h"
#include "userInterface.h"

#define FINISH_SIDE_LOADBAR 270
#define LOADBAR_TURN_OFF 471
#define LOADBAR_DEFAULT_RUN_TIME 8000
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

enum textAlignment
{
	GUI_Align_Text_Left = 1,
	GUI_Align_Text_Center = 2,
	GUI_Align_Text_Right = 3
};

#ifdef _GUI_C

// For touch controls
int x, y;


extern UserInterfaceClass userInterfaceButtons[40];
extern uint8_t buttonsOnPage;
extern Adafruit_FT6206 ts;
extern ILI9488_t3 display;
extern void backgroundProcess();
extern uint8_t graphicLoaderState;

bool GUI_Touch_getXY();
void GUI_drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, int radius);
void GUI_drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void GUI_waitForIt(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC);
void GUI_waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC);
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint16_t size);
int GUI_subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size);
void GUI_setTextSize(int textSize);
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint);
void GUI_clearAppSpace();
void GUI_setLoadBarRuntime(uint16_t time);
void GUI_startLoadBar(void);
void GUI_stopLoadBarTimed(void);
void GUI_sideLoadBarTimed(void);

#else

extern int x, y;


bool GUI_Touch_getXY();
void GUI_drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align, int radius);
void GUI_drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align);
void GUI_waitForIt(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC);
void GUI_waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC);
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint16_t size);
int GUI_subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size);
void GUI_setTextSize(int textSize);
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint);
void GUI_clearAppSpace(void);
void GUI_isButtonsEnabled(UserInterfaceClass* buttons, uint8_t start, uint8_t end, bool isDisabled);
void GUI_setLoadBarRuntime(uint16_t time);
void GUI_startLoadBar(void);
void GUI_stopLoadBarTimed(void);
void GUI_sideLoadBarTimed(void);
void GUI_loadBarPrintFrame(void);
void GUI_LoadBarPrintStart(void);
void GUI_loadBarPrintFailed(void);
void GUI_loadBarPrintProgPass(void);
void GUI_loadBarPrintReadPass(void);

#endif // _GUI_C
#endif // _GUI_H