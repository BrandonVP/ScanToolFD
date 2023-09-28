/*
 ===========================================================================
 Name        : userInterface.cpp
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description : Button objects
 ===========================================================================
 */

#include "userInterface.h"
#include "config.h"

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, String btnText)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->clickReturn = clickReturn;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = 20;
	this->btnText = btnText;
	this->align = ALIGN_CENTER;
	this->backgroundColor = menuBtnColor;
	this->borderColor = menuBtnBorder;
	this->clickBorderColor = menuBtnColor;
	this->textColor = menuBtnText;
}

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, String btnText, uint8_t alignText)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->clickReturn = clickReturn;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = 20;
	this->btnText = btnText;
	this->align = alignText;
	this->backgroundColor = menuBtnColor;
	this->borderColor = menuBtnBorder;
	this->clickBorderColor = menuBtnColor;
	this->textColor = menuBtnText;
}

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t textC)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->clickReturn = clickReturn;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = radius;
	this->btnText = btnText;
	this->align = alignText;
	this->backgroundColor = btnC;
	this->borderColor = borderC;
	this->clickBorderColor = borderC;
	this->textColor = textC;
}

void UserInterfaceClass::setButton(uint16_t xStart, uint16_t yStart, uint16_t xStop, uint16_t yStop, uint16_t clickReturn, bool isRound, uint8_t radius, String btnText, uint8_t alignText, uint16_t btnC, uint16_t borderC, uint16_t clickBorderC, uint16_t textC)
{
	this->xStart = xStart;
	this->xStop = xStop;
	this->yStart = yStart;
	this->yStop = yStop;
	this->clickReturn = clickReturn;
	this->setClickable(true);
	this->setShape(isRound);
	this->roundBtnRadius = radius;
	this->btnText = btnText;
	this->align = alignText;
	this->backgroundColor = btnC;
	this->borderColor = borderC;
	this->clickBorderColor = clickBorderC;
	this->textColor = textC;
}

void UserInterfaceClass::setShape(uint8_t isRound)
{
	if (isRound)
	{
		btnSettings |= (1 << Settings_isRound);
	}
	else
	{
		btnSettings &= ~(1 << Settings_isRound);
	}
}

void UserInterfaceClass::setClickable(uint8_t isClickable)
{
	if (isClickable)
	{
		btnSettings |= (1 << Settings_isClickable);
	}
	else
	{
		btnSettings &= ~(1 << Settings_isClickable);
	}
}

void UserInterfaceClass::setBgColor(uint16_t color)
{
	this->backgroundColor = color;

}

void UserInterfaceClass::setBorderColor(uint16_t color)
{
	this->borderColor = color;

}

void UserInterfaceClass::setClickBorderColor(uint16_t color)
{
	this->clickBorderColor = color;

}

void UserInterfaceClass::setTextColor(uint16_t color)
{
	this->textColor = color;

}

void UserInterfaceClass::setTextSize(uint8_t size)
{
	this->textSize = size;

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

uint8_t UserInterfaceClass::getClickReturn()
{
	return this->clickReturn;
}

bool UserInterfaceClass::getIsRound()
{
	bool isRound;
	(CHECK_BIT(this->btnSettings, Settings_isRound)) ? isRound = true : isRound = false;
	return isRound;
}

bool UserInterfaceClass::getIsClickable()
{
	bool isClickable;
	(CHECK_BIT(this->btnSettings, Settings_isClickable)) ? isClickable = true : isClickable = false;
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
	return this->backgroundColor;
}

uint16_t UserInterfaceClass::getClickBorderColor()
{
	return this->clickBorderColor;
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

uint8_t UserInterfaceClass::getTextSize()
{
	return this->textSize;

}