/*
 ===========================================================================
 Name        : userInterface.h
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description : Button objects
 ===========================================================================
 */

#include "gui.h"
#include "common.h"

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h

#define IS_ROUND_POSITION 7
#define IS_CLICKABLE_POSITION 6

class UserInterfaceClass
{
 protected:
	 // Coordinates
	 uint16_t xStart = 0;
	 uint16_t xStop = 0;
	 uint16_t yStart = 0;
	 uint16_t yStop = 0;

	 // Colors
	 uint16_t backgroundColor = 0;
	 uint16_t borderColor = 0;
	 uint16_t clickBorderColor = 0;
	 uint16_t textColor = 0;

	 String btnText;
	 uint8_t roundBtnRadius = 20;
	 uint8_t align = 1;
	 uint8_t clickReturn = 0;
	 // Boolean Button Settings
	 /* |      7      |      6      |   5   |   4   |   3   |   2   |   1   |   0   | */
	 /* |   isRound   | isClickable |       |       |       |       |       |       | */
	 uint8_t btnSettings = 0;

 public:
	void init();
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t clickReturn, bool isRound, String btnText);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t clickReturn, bool isRound, String btnText, uint8_t alignText);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t clickReturn, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t textC);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t clickReturn, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t clickBorderC, uint16_t textC);
	void setShape(uint8_t isRound);
	void setClickable(uint8_t isClickable);
	void setBgColor(uint16_t color);
	void setBorderColor(uint16_t color);
	void setClickBorderColor(uint16_t color);
	void setTextColor(uint16_t color);
	
	uint16_t getXStart();
	uint16_t getXStop();
	uint16_t getYStart();
	uint16_t getYStop();
	uint8_t getClickReturn();
	bool getIsRound();
	bool getIsClickable();
	uint8_t getRadius();
	String getBtnText();
	uint8_t getAlign();
	uint16_t getBtnColor();
	uint16_t getBorderColor();
	uint16_t getClickBorderColor();
	uint16_t getTextColor();
};
#endif

