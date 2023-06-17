/*
 ===========================================================================
 Name        : gui.cpp
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description :
 ===========================================================================
 */

#define _GUI_C
#include "gui.h"
#include "userInterface.h"
#include "config.h"

 //
void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int alignText, int radius)
{
    const uint8_t LETTER_WIDTH = 11;
    const uint8_t SIDE_OFFSET = 2;
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, btnBgColor);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, btnBorderColor);
    display.drawRoundRect(x_start+1, y_start+1, (x_stop - x_start)-2, (y_stop - y_start)-2, radius, btnBorderColor);
    display.setTextColor(btnTxtColor);

    // Print String with desired alignment
    switch (alignText)
    {
    case 1: // Left
        display.drawString(buttonText, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    case 2: // Center
        // Calculate center
        stringLength = buttonText.length() * LETTER_WIDTH;
        buttonWidth = (x_stop) - (x_start);
        //buttonWidth = (x_stop - SIDE_OFFSET) - (x_start + SIDE_OFFSET);
        offset = (x_start - SIDE_OFFSET) + (buttonWidth / 2) - (stringLength / 2);
        //offset = (x_start + SIDE_OFFSET) + (buttonWidth / 2) - (stringLength / 2);
        //display.setCursor(offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset, false);
        //display.println(button);
        //display.drawString(buttonText, x_start + (x_stop - x_start), y_start + (y_stop - y_start));
        display.drawString(buttonText, offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset);
        break;
    case 3: // Right
        display.drawString(buttonText, x_start + 55, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    default:
        break;
    }
}

//
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align)
{
    const uint8_t LETTER_WIDTH = 11;
    const uint8_t SIDE_OFFSET = 2;
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBgColor);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBorderColor);

    // Print String with desired alignment
    switch (align)
    {
    case 1: // Left
        display.drawString(buttonText, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    case 2: // Center
        // Calculate center
        stringLength = buttonText.length() * LETTER_WIDTH;
        buttonWidth = (x_stop - SIDE_OFFSET) - (x_start + SIDE_OFFSET);
        offset = (x_start + SIDE_OFFSET) + (buttonWidth / 2) - (stringLength / 2);

        display.setTextColor(btnTxtColor);
        //display.setCursor(offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset, false);
        //display.println(button);
        display.drawString(buttonText, offset, y_start + ((y_stop - y_start) / 2) - yMagicOffset);
        break;
    case 3: // Right
        display.drawString(buttonText, x_start + 55, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    default:
        break;
    }
}


// Holds round button down while pressed
void waitForIt(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC)
{
    if (y_stop > 50)
    {
        display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, themeBackground);
    }
    else
    {
        drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
    }
    
    while (ts.touched())
    {
        // This is a blocking function so call backgroundProcess while looping
        backgroundProcess();
    }
    if (y_stop > 50)
    {
        display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, borderC);
    }
    else
    {
        drawSquareBtn(x_start, 45, x_stop, 50, "", menuBtnColor, menuBtnColor, borderC, ALIGN_CENTER);
    }
}

// Holds rectangle button down while pressed
void waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC)
{
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), themeBackground);
    while (ts.touched())
    {
        // This is a blocking function so call backgroundProcess while looping
        backgroundProcess();
    }
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), borderC);
}

//
void buttonMonitor(UserInterfaceClass* buttons, uint8_t size)
{
    if (Touch_getXY())
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if ((x >= buttons[i].getXStart()) && (x <= buttons[i].getXStop()) && (y >= buttons[i].getYStart()) && (y <= buttons[i].getYStop()))
            {
                if (buttons[i].getIsClickable())
                {
                    if (buttons[i].getIsRound())
                    {
                        waitForIt(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor());
                    }
                    else
                    {
                        waitForItRect(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor());
                    }
                    nextPage = buttons[i].getPage();
                }
            }
        }
    }
}

//
int subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size)
{
    if (Touch_getXY())
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if ((x >= buttons[i].getXStart()) && (x <= buttons[i].getXStop()) && (y >= buttons[i].getYStart()) && (y <= buttons[i].getYStop()))
            {
                if (buttons[i].getIsClickable())
                {
                    if (buttons[i].getIsRound())
                    {
                        waitForIt(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor());
                    }
                    else
                    {
                        waitForItRect(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor());
                    }
                    return buttons[i].getPage();
                }
            }
        }
    }
    return -1;
}

//
bool drawPage(UserInterfaceClass* buttons, uint8_t &pos, uint8_t buttonsToPrint)
{
    uint8_t btn = pos - 1;
    if (buttonsToPrint != 0)
    {
        Serial.print("isRound: ");
        Serial.println(buttons[btn].getIsRound());
        if (buttons[btn].getIsRound())
        {
            drawRoundBtn(buttons[btn].getXStart(), buttons[btn].getYStart(), buttons[btn].getXStop(), buttons[btn].getYStop(), buttons[btn].getBtnText(), buttons[btn].getBtnColor(), buttons[btn].getBorderColor(), buttons[btn].getTextColor(), buttons[btn].getAlign(), buttons[btn].getRadius());
        }
        else
        {
            drawSquareBtn(buttons[btn].getXStart(), buttons[btn].getYStart(), buttons[btn].getXStop(), buttons[btn].getYStop(), buttons[btn].getBtnText(), buttons[btn].getBtnColor(), buttons[btn].getBorderColor(), buttons[btn].getTextColor(), buttons[btn].getAlign());
        }
        pos++;
    }
    if (pos > buttonsToPrint)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//
void drawButton(UserInterfaceClass* button, uint16_t buttonColor, uint16_t buttonBorderColor, uint16_t buttonTextColor)
{
    drawRoundBtn(button->getXStart(), button->getYStart(), button->getXStop(), button->getYStop(), button->getBtnText(), button->getBtnColor(), button->getBorderColor(), button->getTextColor(), button->getAlign(), button->getRadius());
}