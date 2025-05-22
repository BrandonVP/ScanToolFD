/*
===========================================================================
Name        : gui.cpp
Author      : Brandon Van Pelt
Created	    : 4/6/2023
Description : GUI framework
===========================================================================
*/

#define GUI_C
#include "gui.h"

// Assign pointer used for running background processes
void GUI_setAppPtr(App* app)
{
    GUI_appPtr = app;
}

// Monitor screen touch
bool GUI_Touch_getXY()
{
    if (ts.touched() > 0)
    {
        //DbgConsole_Printf("x: %d | y: %d\r\n", ts.touchX, ts.touchY);
        TS_Point p = ts.getPoint();
        x = p.y;
        y = SCREEN_HEIGHT - p.x;
        return true;
    }
    return false;
}

void GUI_setTouchedMenu(bool isTouched)
{
    GUI_isTouchedMenu = isTouched;
}

void GUI_setTouchedBody(bool isTouched)
{
    GUI_isTouchedBody = isTouched;
}

bool GUI_getTouchedMenu()
{
    return GUI_isTouchedMenu;
}

bool GUI_getTouchedBody()
{
    return GUI_isTouchedBody;
}

void GUI_updateButton(int index)
{
    GUI_graphicLoaderState = 0;
    while (GUI_drawPage(&userInterfaceButton[index], GUI_graphicLoaderState, 1));
}

void GUI_updateButtonText(int index, String newText)
{
    userInterfaceButton[index].setText(newText);
    GUI_graphicLoaderState = 0;
    while (GUI_drawPage(&userInterfaceButton[index], GUI_graphicLoaderState, 1));
}

// Draw a square button with round edges
void GUI_drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int alignText, int radius)
{
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, btnBgColor);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, btnBorderColor);
    display.drawRoundRect(x_start + 1, y_start + 1, (x_stop - x_start) - 2, (y_stop - y_start) - 2, radius, btnBorderColor);
    display.setTextColor(btnTxtColor);

    char c[0xFF];
    strcpy(c, buttonText.c_str());
    stringLength = display.strPixelLen(c, 2);

    size_t length = buttonText.length();
    int lengthAsInt = static_cast<int>(length);
    const char* cString = buttonText.c_str();

    // Print String with desired alignment
    switch (alignText)
    {
    case 1: // Left
        display.drawString(cString, lengthAsInt, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    case 2: // Center
        // Calculate center
        buttonWidth = (x_stop)-(x_start);
        offset = (buttonWidth - stringLength) / 2;
        display.drawString(cString, lengthAsInt, (x_start + offset), y_start + ((y_stop - y_start) / 2) - yMagicOffset);
        break;
    case 3: // Right
        display.drawString(cString, lengthAsInt, x_stop - (stringLength + 5), y_start + ((y_stop - y_start) / 2) - 8);
        break;
    default:
        break;
    }
}

// Draw a square button
void GUI_drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String buttonText, int btnBgColor, int btnBorderColor, int btnTxtColor, int alignText)
{
    const uint8_t yMagicOffset = 6;
    int stringLength, buttonWidth, offset;

    // Print button
    display.fillRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBgColor);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBorderColor);
    display.setTextColor(btnTxtColor);

    char c[0xFF];
    strcpy(c, buttonText.c_str());
    stringLength = display.strPixelLen(c, 2);

    size_t length = buttonText.length();
    int lengthAsInt = static_cast<int>(length);
    const char* cString = buttonText.c_str();

    // Print String with desired alignment
    switch (alignText)
    {
    case 1: // Left
        display.drawString(cString, lengthAsInt, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8);
        break;
    case 2: // Center
        // Calculate center
        buttonWidth = (x_stop)-(x_start);
        offset = (buttonWidth - stringLength) / 2;
        display.drawString(cString, lengthAsInt, (x_start + offset), y_start + ((y_stop - y_start) / 2) - yMagicOffset);
        break;
    case 3: // Right
        display.drawString(cString, lengthAsInt, x_stop - (stringLength + 5), y_start + ((y_stop - y_start) / 2) - 8);
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
        GUI_drawSquareBtn(62, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, GUI_Align_Text_Center);
    }

    while (true)
    {

        if (ts.touched() > 0)
        {
            TS_Point p = ts.getPoint();
            x = p.y;
            y = SCREEN_HEIGHT - p.x;
            //DbgConsole_Printf("x: %d | y: %d\r\n", x, y);
        }
        else
        {
            GUI_setTouchedMenu(false);
            GUI_setTouchedBody(false);
            break;
        }
        GUI_appPtr->backgroundProcess();
    }

    if (y_stop > 50)
    {
        display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), radius, borderC);
    }
    else
    {
        GUI_drawSquareBtn(x_start, 45, x_stop, 50, "", menuBtnColor, menuBtnColor, borderC, GUI_Align_Text_Center);
        GUI_drawSquareBtn(62, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, GUI_Align_Text_Center);
        GUI_drawSquareBtn(x_start, 45, x_stop, 50, "", menuBtnColor, menuBtnColor, borderC, GUI_Align_Text_Center);
    }
}

// Holds rectangle button down while pressed
void GUI_waitForItRect(int x_start, int y_start, int x_stop, int y_stop, int radius, int borderC, int clickBorderC)
{
    display.useFrameBuffer(false);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), clickBorderC);

    while (true)
    {
        if (ts.touched() > 0)
        {
            TS_Point p = ts.getPoint();
            x = p.y;
            y = SCREEN_HEIGHT - p.x;
            //DbgConsole_Printf("x: %d | y: %d\r\n", x, y);
        }
        else
        {
            GUI_setTouchedMenu(false);
            GUI_setTouchedBody(false);
            break;
        }
        
        GUI_appPtr->backgroundProcess();
    }

    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), borderC);
}

// Loop through active button objects looking for a screen touch - Button press clickReturn directly load Apps
void GUI_buttonMonitor(UserInterfaceClass* buttons, uint16_t size)
{
    if (GUI_getTouchedMenu())
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
                    GUI_appPtr->newApp((APP_labels)buttons[i].getClickReturn());
                }
            }
        }
    }
}

// Loop through active button objects looking for a screen touch - Button press returns button clickReturn value and No press returns -1
int GUI_subMenuButtonMonitor(UserInterfaceClass* buttons, uint8_t size)
{
    if (GUI_getTouchedBody())
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

// Select font based off provided text size
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

// Used to draw an App page
bool GUI_drawPage(UserInterfaceClass* buttons, uint8_t& pos, uint8_t buttonsToPrint)
{
    if (pos == 0)
    {
        pos++;
        return true;
    }

    uint8_t btn = pos - 1;
    if (!buttons[btn].getIsPrintable())
    {
        pos++;
    }
    else if (buttonsToPrint != 0)
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

/// Clear non top menu space
void GUI_clearAppSpace()
{
    display.fillRect(0, 50, 480, 270, themeBackground);
}

// Enable or disable buttons start -> end(including end)
void GUI_isButtonsEnabled(UserInterfaceClass* buttons, uint8_t start, uint8_t end, bool isDisabled)
{
    for (uint8_t i = start; i <= end; i++)
    {
        buttons[i].setClickable(isDisabled);
    }
}

// Set how long to run the load bar in ms
void GUI_setLoadBarRuntimeMS(uint16_t time)
{
    GUI_loarBarRunTimeMS = time;
}

// Always starts at px 250
void GUI_startLoadBar(void)
{
    GUI_loadBarIndex = 250;
    GUI_loadBarTimer = millis();
    display.useFrameBuffer(false);
}

// End the loadbar early
void GUI_stopLoadBarTimed(void)
{
    GUI_loadBarIndex = LOADBAR_TURN_OFF;
}

// Load bar
void GUI_sideLoadBarTimed(void)
{
    if (GUI_loadBarIndex > LOADBAR_TURN_OFF - 1)
    {
        return;
    }

    uint32_t temp = (int)ceil(GUI_loarBarRunTimeMS / 270);

    if (millis() - GUI_loadBarTimer > temp)
    {
        //DbgConsole_Printf("loadBarTimer > temp\r\n");
        display.fillRect(GUI_loadBarIndex++, 181, 1, 38, OrangeBtnColor);
        GUI_loadBarTimer = millis();
    }
}

//
void GUI_loadBarPrintFrame(void)
{
    GUI_drawSquareBtn(244, 176, 476, 225, "", menuBackground, BlackBtnColor, menuBtnColor, GUI_Align_Text_Center);
    GUI_drawSquareBtn(248, 180, 472, 221, "", themeBackground, BlackBtnColor, themeBackground, GUI_Align_Text_Center);
}

//
void GUI_LoadBarPrint(String buttonText, uint8_t fontSize, uint16_t textColor)
{
    if (buttonText == "")
    {
        GUI_drawSquareBtn(244, 109, 476, 175, buttonText, themeBackground, themeBackground, themeBackground, GUI_Align_Text_Center);
    }
    else
    {
        GUI_setTextSize(fontSize);
        GUI_drawSquareBtn(244, 110, 476, 225, "", menuBackground, BlackBtnColor, textColor, GUI_Align_Text_Center);
        GUI_drawSquareBtn(250, 112, 470, 160, buttonText, menuBackground, menuBackground, textColor, GUI_Align_Text_Center);
        display.setFont(Michroma_11);
    }
}

//
void GUI_LoadBarPrint(String buttonText)
{
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(244, 125, 476, 145, buttonText, themeBackground, themeBackground, menuBtnTextColor, GUI_Align_Text_Center);
    display.setFont(Michroma_11);
}

//
void GUI_loadBarPrintFailed(void)
{
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(244, 130, 476, 150, "Test Failed", themeBackground, themeBackground, menuBtnTextColor, GUI_Align_Text_Center);
    display.setFont(Michroma_11);
}

//
void GUI_loadBarPrintPass(void)
{
    display.setFont(Michroma_10);
    GUI_drawSquareBtn(244, 125, 476, 145, "Passed All Tests", themeBackground, themeBackground, menuBtnTextColor, GUI_Align_Text_Center);
    display.setFont(Michroma_11);
}

void GUI_drawSettingsPopUp(void)
{
    GUI_drawSquareBtn(244, 315, 476, 316, "", BlackBtnColor, BlackBtnColor, BlackBtnColor, GUI_Align_Text_Center); // Bottem border
    for (int i = 0; i < 220; i++)
    {
        GUI_drawSquareBtn(245, 314 - i, 475, 315 - i, "", menuBackground, menuBackground, menuBackground, GUI_Align_Text_Center); // Body
        GUI_drawSquareBtn(244, 314 - i, 245, 315 - i, "", BlackBtnColor, BlackBtnColor, BlackBtnColor, GUI_Align_Text_Center); // Left border
        GUI_drawSquareBtn(475, 314 - i, 476, 315 - i, "", BlackBtnColor, BlackBtnColor, BlackBtnColor, GUI_Align_Text_Center); // Right border
    }
    GUI_drawSquareBtn(244, 94, 476, 95, "", BlackBtnColor, BlackBtnColor, BlackBtnColor, GUI_Align_Text_Center); // Top border
}

//
void GUI_clearSettingsPopUp(void)
{
    for (int i = 0; i < 230; i++)
    {
        GUI_drawSquareBtn(244, 90 + i, 476, 91 + i, "", themeBackground, themeBackground, themeBackground, GUI_Align_Text_Center); // Body
    }
}

// Print RF packets to the LCD
void GUI_LCD_Terminal(String buttonText)
{
    static uint8_t GUI_index = 0;

    memcpy(GUI_printString[GUI_index], buttonText.c_str(), buttonText.length());

    // Ensure the string is null-terminated
    GUI_printString[GUI_index++][buttonText.length()] = '\0';

    display.setTextColor(menuBtnText);
    display.setFont(Michroma_8);
    uint8_t temp = 0;
    if (GUI_index == 0)
    {
        temp = 13;
    }
    else
    {
        temp = GUI_index - 1;
    }
    if (GUI_index > 13)
    {
        GUI_index = 0;
    }

    for (uint16_t i = 0, j = 295, k = temp; i < 13; i++, j = j - 19)
    {
        GUI_appPtr->backgroundProcess();

        // This might need adjusted. Letters have different widths. For example "A" will only fit 19 times. 
        // I choose 24 as a nice average number that should be conservative enought to not bleed go over.
        int length = strlen(GUI_printString[k]);
        (length > 24) ? length = 24 : length = length;

        if (j == 295)
        {
            display.fillRect(4, j + 4, 226, 14, menuBackground);
            display.drawString(GUI_printString[k], length, 5, j + 6);
        }
        else
        {
            display.fillRect(5, j, 225, 11, BlackBtnColor);
            display.drawString(GUI_printString[k], length, 5, j);
        }

        if (k == 0 || k > 13)
        {
            k = 13;
        }
        else
        {
            k--;
        }
    }
    display.setFont(Michroma_11);
}

// Print RF packets to the LCD
void GUI_clear_LCD_Terminal()
{
    memset(GUI_printString, 0, sizeof(GUI_printString));
}