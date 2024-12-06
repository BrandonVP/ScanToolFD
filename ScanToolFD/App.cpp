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

uint8_t createToolMenu()
{
	return app.printMenu(MENU_tools, APP_TOOLS);
}

uint8_t createSettingsMenu()
{
	return app.printMenu(MENU_settings, APP_SETTINGS);
}

uint8_t createCANBusMenu()
{
	return app.printMenu(MENU_canBus, APP_CANBUS);
}

//
void App::init()
{
	
	this->myApps.reserve(APP_COUNT);

	/*---------------------Menu----------Title----------------App Label---------------Main Function--------------Create Button Function----------Animation-----------------App Exit Function-----*/
	appManager appObj0(MENU_canBus,     "CAN Bus",           APP_CANBUS,            CAPTURE_CANBus,             createCANBusMenu);
	appManager appObj1(MENU_tools,      "Tools",             APP_TOOLS,             someFn,                     createToolMenu);
	appManager appObj2(MENU_settings,   "Settings",          APP_SETTINGS,          someFn,                     createSettingsMenu);
	appManager appObj3(MENU_canBus,     "Capture",           APP_CAPTURE,           CAPTURE_captureConfig,      CAPTURE_createCaptureBtns);
	appManager appObj4(MENU_sub,        "CaptureLCD",        APP_CAPTURE_LCD,       CAPTURE_LCD_scan,           CAPTURE_createLCDBtns);
	appManager appObj5(MENU_canBus,     "Files",             APP_FILES,             someFn,                     CAPTURE_drawCANLogScroll);
	appManager appObj6(MENU_canBus,     "FilterMask",        APP_FILTER_MASK,       FILTERMASK_filterMask,      FILTERMASK_createFilterMaskBtns);
	appManager appObj7(MENU_canBus,     "Send",              APP_SEND,              someFn,                     SEND_createSendBtns);
	appManager appObj8(MENU_canBus,     "BaudRate",          APP_BAUD_RATE,         BAUD_Baud,                  BAUD_createBaudBtns);
	appManager appObj9(MENU_tools,      "Get Vin",           APP_GET_VIN,           BAUD_Baud,                  BAUD_createBaudBtns);

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

}

uint8_t App::printMenu(menus menu, APP_labels label)
{
	uint8_t btnPos = 0;
	uint8_t menuCoordIndex = 0;

	Serial.printf("app.getAppSize(): %d \n", app.getAppSize());
	for (int i = 0; i < app.getAppSize(); i++)
	{
		Serial.printf("app.getMenu(i): %d \n", app.getMenu(i));
		Serial.printf("app.getLabel(i): %d \n", app.getLabel(i));
		if ((app.getMenu(i) == menu) && (app.getLabel(i) != label))
		{
			userInterfaceButton[btnPos++].setButton(MENU_COORD[menuCoordIndex][0], MENU_COORD[menuCoordIndex][1], MENU_COORD[menuCoordIndex][2], MENU_COORD[menuCoordIndex][3], app.getLabel(i), true, 10, app.getName(i), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
			menuCoordIndex++;
			Serial.printf("menuCoordIndex: %d \n", menuCoordIndex);
		}
	}
	Serial.printf("btnPos: %d \n", btnPos);
	return btnPos;
}

// Resets variables for page change
void App::appTransition()
{
	hasDrawn = false;
	graphicLoaderState = 0;
	activeApp = nextApp;
	GUI_stopLoadBarTimed();
	display.useFrameBuffer(true);
	CAPTURE_clearLocalVar();
}

//
void App::newApp(APP_labels loadNewApp )
{
	nextApp = loadNewApp;
}

//
int App::getAppSize(void)
{
	return myApps.size();
}

int App::getMenu(int index)
{
	return myApps[index].getAssignedMenu();
}

APP_labels App::getLabel(int index)
{
	return myApps[index].getAppLabel();
}

String App::getName(int index)
{
	return myApps[index].getName();
}

//
void App::run()
{
	// Draw page
	if (!hasDrawn)
	{
		if (graphicLoaderState == 0)
		{
			GUI_clearAppSpace();
			buttonsOnPage = myApps[(int)activeApp].printButtons();
			graphicLoaderState++;
			return;
		}
		if (GUI_drawPage(userInterfaceButton, graphicLoaderState, buttonsOnPage))
		{
			return;
		}
		Serial.printf("App: %d\n", (int)activeApp);
		Serial.println(myApps[(int)activeApp].getName());
		hasDrawn = true;
		display.updateScreen();
	}

	// Call buttons or page method
	myApps[activeApp].runApp(GUI_subMenuButtonMonitor(userInterfaceButton, buttonsOnPage));

	// Load next selected app
	if (nextApp != activeApp)
	{
		this->appTransition();
	}
}
