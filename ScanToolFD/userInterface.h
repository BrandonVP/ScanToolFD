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
#include <vector>

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h

class UserInterfaceClass
{
 private:
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
	 uint8_t textSize = 11;

	 String btnText;
	 uint8_t roundBtnRadius = 20;
	 uint8_t align = 1;
	 uint16_t clickReturn = 0;
	 uint8_t btnSettings = 0;

	 enum assignedBtnSettings {
		 Settings_isClickable = 0,
		 Settings_isRound,
		 Settings_unused1,
		 Settings_unused2,
		 Settings_unused3,
		 Settings_unused4,
		 Settings_unused5,
		 Settings_unused6
	 };


 public:
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, String btnText);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, String btnText, uint8_t alignText);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t textC);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t clickBorderC, uint16_t textC);
	void setShape(uint8_t isRound);
	void setClickable(uint8_t isClickable);
	void setBgColor(uint16_t color);
	void setBorderColor(uint16_t color);
	void setClickBorderColor(uint16_t color);
	void setTextColor(uint16_t color);
	void setTextSize(uint8_t size);

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
	uint8_t getTextSize();
};
#endif

