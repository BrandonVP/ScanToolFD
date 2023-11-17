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



 // Local load bar variables
static uint16_t loadBarIndex = 471; // ends at 470
static uint32_t loadBarTimer = 0;
static uint16_t loarBarRunTime = 8000;

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
    display.useFrameBuffer(false);
    if (y_stop > 50)
    {
        display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, clickBorderC);
    }
    else
    {
        // < 50 indicates this is a menu button press
        GUI_drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
    }

    while (ts.touched())
    {
        // This is a blocking function, call backgroundProcess while looping
        backgroundProcess();
    }

    if (y_stop > 50)
    {
        display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, borderC);
    }
    else
    {
        GUI_drawSquareBtn(x_start, 45, x_stop, 50, "", menuBtnColor, menuBtnColor, borderC, ALIGN_CENTER);
        display.useFrameBuffer(true);
        GUI_drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
        GUI_drawSquareBtn(x_start, 45, x_stop, 50, "", menuBtnColor, menuBtnColor, borderC, ALIGN_CENTER);
    }
}

// Holds rectangle button down while pressed
void GUI_waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC)
{
    display.useFrameBuffer(false);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), clickBorderC);
    while (ts.touched())
    {
        // This is a blocking function, call backgroundProcess while looping
        backgroundProcess();
    }
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), borderC);
    display.useFrameBuffer(true);
}

//
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint16_t size)
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
void GUI_setTextSize(int textSize)
{
    if (textSize < 10)
    {
        display.setFont(Michroma_9);
    }
    else if (textSize < 11)
    {
        display.setFont(Michroma_10);
    }
    else if (textSize < 12)
    {
        display.setFont(Michroma_11);
    }
    else if (textSize < 13)
    {
        display.setFont(Michroma_12);
    }
    else if (textSize < 14)
    {
        display.setFont(Michroma_13);
    }
    else if (textSize < 16)
    {
        display.setFont(Michroma_14);
    }
    else if (textSize < 18)
    {
        display.setFont(Michroma_16);
    }
    else if (textSize < 20)
    {
        display.setFont(Michroma_18);
    }
    else if (textSize < 24)
    {
        display.setFont(Michroma_20);
    }
    else if (textSize < 28)
    {
        display.setFont(Michroma_24);
    }
    else if (textSize < 32)
    {
        display.setFont(Michroma_28);
    }
    else if (textSize < 40)
    {
        display.setFont(Michroma_32);
    }
    else if (textSize < 48)
    {
        display.setFont(Michroma_40);
    }
    else if (textSize < 60)
    {
        display.setFont(Michroma_48);
    }
    else if (textSize < 72)
    {
        display.setFont(Michroma_60);
    }
    else if (textSize < 96)
    {
        display.setFont(Michroma_72);
    }
    else if (textSize > 95)
    {
        display.setFont(Michroma_96);
    }
}

//
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint)
{
    uint8_t btn = pos - 1;
    if (buttonsToPrint != 0)
    {
        GUI_setTextSize(buttons[btn].getTextSize());
        
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

//
void GUI_setLoadBarRuntime(uint16_t time)
{
    loarBarRunTime = time;
}

//
void GUI_startLoadBar(void)
{
    loadBarIndex = 250;
    loadBarTimer = millis();
}

//
void GUI_stopLoadBarTimed(void)
{
    loadBarIndex = 471;
}

// Load bar
void GUI_sideLoadBarTimed(void)
{
    if (loadBarIndex > 470)
    {
        return;
    }

    uint32_t temp = ceil(loarBarRunTime / 270);

    if (millis() - loadBarTimer > temp)
    {
        display.fillRect(loadBarIndex++, 151, 1, 38, OrangeBtnColor);
        loadBarTimer = millis();
    }
}

//
void GUI_loadBarPrintFrame(void)
{
    GUI_drawSquareBtn(244, 146, 476, 195, "", menuBackground, BlackBtnColor, menuBtnColor, ALIGN_CENTER);
    GUI_drawSquareBtn(248, 150, 472, 191, F(""), themeBackground, BlackBtnColor, themeBackground, ALIGN_CENTER);
}

//
void GUI_LoadBarPrintStart(void)
{
    display.fillRect(241, 51, 480, 269, themeBackground);
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(245, 127, 480, 142, F("Place tool next to TPMS"), themeBackground, themeBackground, menuBtnTextColor, ALIGN_LEFT);
    display.setFont(Michroma_11);
}

//
void GUI_loadBarPrintFailed(void)
{
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(242, 200, 480, 220, F("TPMS not detected"), themeBackground, themeBackground, menuBtnTextColor, ALIGN_CENTER);
    display.setFont(Michroma_11);
}

//
void GUI_loadBarPrintProgPass(void)
{
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(242, 200, 480, 220, F("Programming successful"), themeBackground, themeBackground, menuBtnTextColor, ALIGN_LEFT);
    display.setFont(Michroma_11);
}

//
void GUI_loadBarPrintReadPass(void)
{
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(280, 200, 480, 220, F("Read successful"), themeBackground, themeBackground, menuBtnTextColor, ALIGN_LEFT);
    display.setFont(Michroma_11);
}