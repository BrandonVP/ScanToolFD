/*
 ===========================================================================
 Name        : userInterface.h
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description :
 ===========================================================================
 */

#include "gui.h"

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class UserInterfaceClass
{
 protected:
	 uint16_t xStart = 0;
	 uint16_t xStop = 0;
	 uint16_t yStart = 0;
	 uint16_t yStop = 0;
	 uint16_t bodyColor = 0;
	 uint16_t borderColor = 0;
	 uint16_t textColor = 0;
	 uint8_t page = 0;
	 bool isRound = true;
	 String btnText;
	 uint8_t align = 1;
	 uint8_t roundBtnRadius = 20;

 public:
	void init();
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, String btnText);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, String btnText, uint8_t alignText);
	void setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t textC);
	uint16_t getXStart();
	uint16_t getXStop();
	uint16_t getYStart();
	uint16_t getYStop();
	uint8_t getPage();
	bool getIsRound();
	uint8_t getRadius();
	String getBtnText();
	uint8_t getAlign();
	uint16_t getBtnColor();
	uint16_t getBorderColor();
	uint16_t getTextColor();
};


#endif

