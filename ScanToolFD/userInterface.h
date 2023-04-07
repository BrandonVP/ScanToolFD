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
	 bool usePage = true;
	 uint8_t page = 0;
	 void(*assignedFunction)(void);

 public:
	void init();
	void setButton(uint16_t, uint16_t, uint16_t, uint16_t, bool, uint8_t, void*);
	uint16_t getXStart();
	uint16_t getXStop();
	uint16_t getYStart();
	uint16_t getYStop();
	uint8_t getPage();
	bool getUsingPage();
	void callFunction();
};


#endif

