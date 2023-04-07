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

void UserInterfaceClass::init()
{

}

void UserInterfaceClass::setButton(uint16_t xStart,  uint16_t yStart, uint16_t xStop, uint16_t yStop, bool usingPage, uint8_t page, void* function)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	if (usingPage)
	{
		this->page = page;
	}
	else
	{
		assignedFunction = (void(*)())function;
	}
}

uint16_t UserInterfaceClass::getXStart()
{
	return xStart;
}

uint16_t UserInterfaceClass::getXStop()
{
	return xStop;
}

uint16_t UserInterfaceClass::getYStart()
{
	return yStart;
}

uint16_t UserInterfaceClass::getYStop()
{
	return yStop;
}

uint8_t UserInterfaceClass::getPage()
{
	return page;
}

bool UserInterfaceClass::getUsingPage()
{
	return usePage;
}

void UserInterfaceClass::callFunction()
{
	assignedFunction();
}

UserInterfaceClass UserInterface;

