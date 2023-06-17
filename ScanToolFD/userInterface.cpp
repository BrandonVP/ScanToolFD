/*
 ===========================================================================
 Name        : userInterface.cpp
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description :
 ===========================================================================
 */

#include "userInterface.h"
#include "config.h"

void UserInterfaceClass::init()
{

}

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, String btnText)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->page = page;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = 20;
	this->btnText = btnText;
	this->align = ALIGN_CENTER;
	this->bodyColor = menuBtnColor;
	this->borderColor = menuBtnBorder;
	this->textColor = menuBtnText;
}

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, String btnText, uint8_t alignText)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->page = page;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = 20;
	this->btnText = btnText;
	this->align = alignText;
	this->bodyColor = menuBtnColor;
	this->borderColor = menuBtnBorder;
	this->textColor = menuBtnText;
}

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint8_t page, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t textC)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->page = page;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = radius;
	this->btnText = btnText;
	this->align = alignText;
	this->bodyColor = btnC;
	this->borderColor = borderC;
	this->textColor = textC;
}

void UserInterfaceClass::setShape(uint8_t isRound)
{
	if (isRound)
	{
		btnSettings |= (1 << IS_ROUND_POSITION);
	}
	else
	{
		btnSettings &= ~(1 << IS_ROUND_POSITION);
	}
}

void UserInterfaceClass::setClickable(uint8_t isClickable)
{
	if (isClickable)
	{
		btnSettings |= (1 << IS_CLICKABLE_POSITION);
	}
	else
	{
		btnSettings &= ~(1 << IS_CLICKABLE_POSITION);
	}
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

bool UserInterfaceClass::getIsRound()
{
	bool isRound;
	(CHECK_BIT(this->btnSettings, IS_ROUND_POSITION)) ? isRound = true : isRound = false;
	return isRound;
}

bool UserInterfaceClass::getIsClickable()
{
	bool isClickable;
	(CHECK_BIT(this->btnSettings, IS_CLICKABLE_POSITION)) ? isClickable = true : isClickable = false;
	return isClickable;
}

uint8_t UserInterfaceClass::getRadius()
{
	return this->roundBtnRadius;
}

String UserInterfaceClass::getBtnText()
{
	return this->btnText;
}

uint8_t UserInterfaceClass::getAlign()
{
	return this->align;
}

uint16_t UserInterfaceClass::getBtnColor()
{
	return this->bodyColor;
}

uint16_t UserInterfaceClass::getBorderColor()
{
	return this->borderColor;
}

uint16_t UserInterfaceClass::getTextColor()
{
	return this->textColor;
}
UserInterfaceClass UserInterface;

