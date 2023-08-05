/*
 ===========================================================================
 Name        : gui.cpp
 Author      : Brandon Van Pelt
 Created	 : 4/6/2023
 Description : GUI framework
 ===========================================================================
 */

#define _GUI_C
#include "gui.h"

 // Monitor screen touch
bool GUI_Touch_getXY()
{
    if (ts.touched())
    {
        TS_Point p = ts.getPoint();
        x = p.y;
        y = SCREEN_HEIGHT - p.x;
        return true;
    }
    return false;
}

//
void GUI_drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int alignText, int radius)
{
    const uint8_t LETTER_WIDTH = 10;
    //const uint8_t SIDE_OFFSET = 2;
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, btnBgColor);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, btnBorderColor);
    display.drawRoundRect(x_start + 1, y_start + 1, (x_stop - x_start) - 2, (y_stop - y_start) - 2, radius, btnBorderColor);
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
        //Serial.print("stringLength: ");
        //Serial.println(buttonText.length());
        buttonWidth = (x_stop)-(x_start);
        //buttonWidth = (x_stop - SIDE_OFFSET) - (x_start + SIDE_OFFSET);
        offset = x_start + (buttonWidth / 2) - ((stringLength / 2));
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
void GUI_drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int align)
{
    const uint8_t LETTER_WIDTH = 11;
    const uint8_t SIDE_OFFSET = 2;
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBgColor);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBorderColor);
    display.setTextColor(btnTxtColor);

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
void GUI_waitForIt(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC)
{
    if (y_stop > 50)
    {
        display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, clickBorderC);
    }
    else
    {
        GUI_drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
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
        GUI_drawSquareBtn(x_start, 45, x_stop, 50, "", menuBtnColor, menuBtnColor, borderC, ALIGN_CENTER);
    }
}

// Holds rectangle button down while pressed
void GUI_waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC)
{
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), clickBorderC);
    while (ts.touched())
    {
        // This is a blocking function so call backgroundProcess while looping
        backgroundProcess();
    }
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), borderC);
}

//
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint8_t size)
{
    if (GUI_Touch_getXY())
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if ((x >= buttons[i].getXStart()) && (x <= buttons[i].getXStop()) && (y >= buttons[i].getYStart()) && (y <= buttons[i].getYStop()))
            {
                if (buttons[i].getIsClickable())
                {
                    if (buttons[i].getIsRound())
                    {
                        GUI_waitForIt(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor(), buttons[i].getClickBorderColor());
                    }
                    else
                    {
                        GUI_waitForItRect(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor(), buttons[i].getClickBorderColor());
                    }
                    nextApp = buttons[i].getClickReturn();
                }
            }
        }
    }
}

// Returns button clickReturn value - No press returns -1
int GUI_subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size)
{
    if (GUI_Touch_getXY())
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if ((x >= buttons[i].getXStart()) && (x <= buttons[i].getXStop()) && (y >= buttons[i].getYStart()) && (y <= buttons[i].getYStop()))
            {
                if (buttons[i].getIsClickable())
                {
                    if (buttons[i].getIsRound())
                    {
                        GUI_waitForIt(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor(), buttons[i].getClickBorderColor());
                    }
                    else
                    {
                        GUI_waitForItRect(buttons[i].getXStart(), buttons[i].getYStart(), buttons[i].getXStop(), buttons[i].getYStop(), buttons[i].getRadius(), buttons[i].getBorderColor(), buttons[i].getClickBorderColor());
                    }
                    return buttons[i].getClickReturn();
                }
            }
        }
    }
    return -1;
}

//
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint)
{
    uint8_t btn = pos - 1;
    if (buttonsToPrint != 0)
    {
        if (buttons[btn].getIsRound())
        {
            GUI_drawRoundBtn(buttons[btn].getXStart(), buttons[btn].getYStart(), buttons[btn].getXStop(), buttons[btn].getYStop(), buttons[btn].getBtnText(), buttons[btn].getBtnColor(), buttons[btn].getBorderColor(), buttons[btn].getTextColor(), buttons[btn].getAlign(), buttons[btn].getRadius());
        }
        else
        {
            GUI_drawSquareBtn(buttons[btn].getXStart(), buttons[btn].getYStart(), buttons[btn].getXStop(), buttons[btn].getYStop(), buttons[btn].getBtnText(), buttons[btn].getBtnColor(), buttons[btn].getBorderColor(), buttons[btn].getTextColor(), buttons[btn].getAlign());
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
void GUI_clearAppSpace()
{
    display.fillRect(0, 51, 480, 269, themeBackground);
}