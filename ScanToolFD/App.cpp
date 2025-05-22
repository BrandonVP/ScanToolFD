/*
===========================================================================
Name        : App.cpp
Author      : Brandon Van Pelt
Created	    : 6/2/2024
Description : Class to load and run Apps
===========================================================================
*/

#include "App.h"
#include "CANBusCapture.h"
#include "CANBusSend.h"
#include "baudRate.h"
#include "filterMask.h"

extern App app;

//
void someFn(int a)
{
	// Temporary holder 
}

int someDraw()
{
	return 0;
}

//
void menuInput(int userInput)
{
	if (userInput >= 0)
	{
		app.newApp((APP_labels)userInput);
	}
}

//
uint8_t createMenu()
{
	uint8_t appCount = 0;

	for (int i = 0; i < app.getAppSize(); i++)
	{
		if ((app.getMenu(i) == app.getMenu(app.activeApp)) && (app.getLabel(i) != app.getLabel(app.activeApp)))
		{
			appCount++;
		}
	}

	return app.printMenu((menus)app.getMenu(app.activeApp), app.getLabel(app.activeApp), appCount);
}

//
void App::init()
{
	
	this->myApps.reserve(APP_COUNT);

	/*---------------------Menu----------Title----------------App Label---------------Main Function--------------Create Button Function----------Animation-----------------App Exit Function-----*/
	
    appManager appObj0(MENU_canBus,     "CAN Bus",           APP_CANBUS,            menuInput,                  createMenu);
	appManager appObj1(MENU_tools,      "Tools",             APP_TOOLS,             menuInput,                  createMenu);
	appManager appObj2(MENU_settings,   "Settings",          APP_SETTINGS,          menuInput,                  createMenu);
	appManager appObj3(MENU_canBus,     "Capture",           APP_CAPTURE,           CAPTURE_captureConfig,      CAPTURE_createCaptureBtns);
	appManager appObj4(MENU_sub,        "CaptureLCD",        APP_CAPTURE_LCD,       CAPTURE_LCD_scan,           CAPTURE_createLCDBtns);
	appManager appObj5(MENU_canBus,     "Files",             APP_FILES,             someFn,                     CAPTURE_drawCANLogScroll);
	appManager appObj6(MENU_canBus,     "FilterMask",        APP_FILTER_MASK,       FILTERMASK_filterMask,      FILTERMASK_createFilterMaskBtns);
	appManager appObj7(MENU_canBus,     "Send",              APP_SEND,              someFn,                     SEND_createSendBtns);
	appManager appObj8(MENU_canBus,     "BaudRate",          APP_BAUD_RATE,         BAUD_Baud,                  BAUD_createBaudBtns);
	appManager appObj9(MENU_settings,   "Get Vin1",          APP_GET_VIN,           BAUD_Baud,                  BAUD_createBaudBtns);
	appManager appObj10(MENU_settings,  "Get Vin2",          APP_GET_VIN,           BAUD_Baud,                  BAUD_createBaudBtns);
	appManager appObj11(MENU_tools,     "Get Vin3",          APP_GET_VIN,           BAUD_Baud,                  BAUD_createBaudBtns);
	appManager appObj12(MENU_tools,     "Get Vin4",          APP_GET_VIN,           BAUD_Baud,                  BAUD_createBaudBtns);

	this->myApps.push_back(appObj0);
	this->myApps.push_back(appObj1);
	this->myApps.push_back(appObj2);
	this->myApps.push_back(appObj3);
	this->myApps.push_back(appObj4);
	this->myApps.push_back(appObj5);
	this->myApps.push_back(appObj6);
	this->myApps.push_back(appObj7);
	this->myApps.push_back(appObj8);
	this->myApps.push_back(appObj9);
	this->myApps.push_back(appObj10);
	this->myApps.push_back(appObj11);
	this->myApps.push_back(appObj12);

}

// Main framework process
void App::run()
{
    // Check if menu was pressed
    GUI_buttonMonitor(userInterfaceMenuButton, MENU_BUTTON_SIZE);

    // Draw page
    if (!hasDrawn)
    {
        if (GUI_graphicLoaderState == 0)
        {
            GUI_clearAppSpace();

            // Animation
            if (!myApps[(int)activeApp].isAnimationsNULL())
            {
                myApps[(int)activeApp].runAnimations();
            }

            buttonsOnPage = myApps[(int)activeApp].printButtons();
            GUI_graphicLoaderState++;
            return;
        }
        if (GUI_drawPage(userInterfaceButton, GUI_graphicLoaderState, buttonsOnPage))
        {
            return;
        }
        hasDrawn = true;
    }

    // Run any background processes
    this->backgroundProcess();

    // Get user input
    int userInput = GUI_subMenuButtonMonitor(userInterfaceButton, buttonsOnPage);

    // Call buttons or page method
    myApps[activeApp].runApp(userInput);

    // Load next selected app
    if (nextApp != activeApp)
    {
        if (!myApps[activeApp].isExecuteCBNULL())
        {
            if (myApps[activeApp].executeCB(userInput))
            {
                return; // Prevent appTransition from loading next app
            }
        }
        this->appTransition();
    }
}

// Print the menu buttons by loading a coordinate matrix matching the quantity of buttons assigned to the currently loaded object's assigned menu
uint8_t App::printMenu(menus menu, APP_labels label, uint8_t numOfButtons)
{
    uint8_t btnPos = 0;
    uint8_t menuCoordIndex = 0;

    // Adjust layout to match buttons on page
    if (numOfButtons <= 4)
    {
        for (int i = 0; i < app.getAppSize(); i++)
        {
            if ((app.getMenu(i) == menu) && (app.getLabel(i) != label))
            {
                userInterfaceButton[btnPos++].setButton(MENU_4Center[menuCoordIndex][0], MENU_4Center[menuCoordIndex][1], MENU_4Center[menuCoordIndex][2], MENU_4Center[menuCoordIndex][3], app.getLabel(i), true, 10, app.getName(i), GUI_Align_Text_Center, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
                menuCoordIndex++;
            }
        }
    }
    else if (numOfButtons <= 6)
    {
        for (int i = 0; i < app.getAppSize(); i++)
        {
            if ((app.getMenu(i) == menu) && (app.getLabel(i) != label))
            {
                userInterfaceButton[btnPos++].setButton(MENU_6Grid[menuCoordIndex][0], MENU_6Grid[menuCoordIndex][1], MENU_6Grid[menuCoordIndex][2], MENU_6Grid[menuCoordIndex][3], app.getLabel(i), true, 10, app.getName(i), GUI_Align_Text_Center, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
                menuCoordIndex++;
            }
        }
    }
    else // if (numOfButtons <= 8) // Everything else loads with a maximum of 8 buttons. This can be expanded if another coordinate matrix is added for increasing the button layouts.
    {
        for (int i = 0; i < app.getAppSize(); i++)
        {
            if ((app.getMenu(i) == menu) && (app.getLabel(i) != label))
            {
                userInterfaceButton[btnPos++].setButton(MENU_8Grid[menuCoordIndex][0], MENU_8Grid[menuCoordIndex][1], MENU_8Grid[menuCoordIndex][2], MENU_8Grid[menuCoordIndex][3], app.getLabel(i), true, 10, app.getName(i), GUI_Align_Text_Center, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
                menuCoordIndex++;
            }
        }
    }

    return btnPos;
}

// Resets variables for page change
void App::appTransition()
{
    hasDrawn = false;
    GUI_graphicLoaderState = 0;
    activeApp = nextApp;
    GUI_stopLoadBarTimed();
}

//
void App::newApp(APP_labels loadNewApp)
{
    nextApp = loadNewApp;
}

//
int App::getAppSize(void)
{
    return myApps.size();
}

//
int App::getMenu(int index)
{
    return myApps[index].getAssignedMenu();
}

//
APP_labels App::getLabel(int index)
{
    return myApps[index].getAppLabel();
}

//
String App::getName(int index)
{
    return myApps[index].getName();
}

//
APP_labels App::getActiveApp()
{
    return activeApp;
}

//
void App::backgroundProcess()
{
    GUI_sideLoadBarTimed();
}

//
uint8_t App::getButtonsOnPage()
{
    return buttonsOnPage;
}

//
void App::setButtonsOnPage(uint8_t buttons)
{
    buttonsOnPage = buttons;
}
