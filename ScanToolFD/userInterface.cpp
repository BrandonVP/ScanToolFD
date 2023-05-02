/*
 ===========================================================================
 Name        : userInterface.cpp
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description :
 ===========================================================================
 */

#include "userInterface.h"
#include "test.h"

extern void testf();

void UserInterfaceClass::init()
{

}

void UserInterfaceClass::setButton(uint16_t xStart,  uint16_t yStart, uint16_t xStop, uint16_t yStop, bool usingPage, uint8_t page, void* func, String btnName, uint16_t btnColor, uint16_t btnBorder, uint16_t btnText, uint8_t align)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	usePage = usingPage;
	if (usingPage)
	{
		this->page = page;
	}
	else
	{
		assignedFunction = (void(*)())func;
	}
	this->btnName = btnName;
	this->btnColor = btnColor;
	this->btnBorder = btnBorder;
	this->btnText = btnText;
	this->align = align;
}

uint16_t UserInterfaceClass::getXStart()
{
	return this->xStart;
}

uint16_t UserInterfaceClass::getXStop()
{
	return this->xStop;
}

uint16_t UserInterfaceClass::getYStart()
{
	return this->yStart;
}

uint16_t UserInterfaceClass::getYStop()
{
	return this->yStop;
}

uint8_t UserInterfaceClass::getPage()
{
	return this->page;
}

bool UserInterfaceClass::getUsingPage()
{
	return this->usePage;
}

void UserInterfaceClass::callFunction()
{
	assignedFunction();
}

bool UserInterfaceClass::getBtnName()
{
	return this->btnName;
}

bool UserInterfaceClass::getBtnColor()
{
	return this->btnColor;
}

bool UserInterfaceClass::getBtnBorder()
{
	return this->btnBorder;
}

bool UserInterfaceClass::getBtnText()
{
	return this->btnText;
}

bool UserInterfaceClass::getAlign()
{
	return this->align;
}
UserInterfaceClass UserInterface;

