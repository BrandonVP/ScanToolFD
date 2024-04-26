#include "App.h"
#include "CANBusCapture.h"
#include "CANBusSend.h"

//
void someFn(int a)
{
	// Temporary holder 
}

// TODO: Make auto populated
uint8_t createToolBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(45, 75, 230, 125, APP_TOOLS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 135, 230, 185, APP_TOOLS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 195, 230, 245, APP_TOOLS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	//userInterfaceButton[btnPos++].setButton(45, 260, 220, 300, APP_TOOLS, true, F(""), ALIGN_CENTER);

	userInterfaceButton[btnPos++].setButton(250, 75, 435, 125, APP_TOOLS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(250, 135, 435, 185, APP_TOOLS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(250, 195, 435, 245, APP_TOOLS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	//userInterfaceButton[btnPos++].setButton(270, 260, 425, 300, APP_TOOLS, true, F(""), ALIGN_CENTER);

	/*
	userInterfaceButton[btnPos++].setButton(55, 80, 220, 120, 1, true, F("PID Scan"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 80, 425, 120, 2, true, F("PID Stream"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 140, 220, 180, 0, true, F("Get VIN"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 140, 425, 180, 0, true, F("DTC"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 200, 220, 240, 0, true, F("OBD Simulator"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Gauges"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 260, 220, 300, 0, true, F("TX Spam"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 260, 425, 300, 0, true, F(""), ALIGN_CENTER);
	*/
	return btnPos;
}

// TODO: Make auto populated
uint8_t createSettingsBtns()
{
	uint8_t btnPos = 0;
	userInterfaceButton[btnPos++].setButton(45, 75, 230, 125, APP_SETTINGS, true, 10, F("Connect Dongle"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 135, 230, 185, APP_SETTINGS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 195, 230, 245, APP_SETTINGS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(45, 260, 220, 300, APP_SETTINGS, true, F(""), ALIGN_CENTER);

	userInterfaceButton[btnPos++].setButton(250, 75, 435, 125, APP_SETTINGS, true, 10, F("About"), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(250, 135, 435, 185, APP_SETTINGS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(250, 195, 435, 245, APP_SETTINGS, true, 10, F(""), ALIGN_CENTER, menuBtnColor, menuBtnBorder, BlackBtnColor, menuBtnText);
	userInterfaceButton[btnPos++].setButton(270, 260, 425, 300, APP_SETTINGS, true, F(""), ALIGN_CENTER);

	/*
	userInterfaceButton[btnPos++].setButton(55, 80, 220, 120, 1, true, F("Memory"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 80, 425, 120, 2, true, F("About"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 140, 220, 180, 0, true, F("WiFi MAC"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 140, 425, 180, 0, true, F("Overclock"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 200, 220, 240, 0, true, F("Reset WiFi"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 200, 425, 240, 0, true, F("Set Time"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(55, 260, 220, 300, 0, true, F("Dongle"), ALIGN_CENTER);
	userInterfaceButton[btnPos++].setButton(260, 260, 425, 300, 0, true, F("Reset"), ALIGN_CENTER);
	*/
	return btnPos;
}

//
void App::init()
{
	
	this->myApps.reserve(APP_COUNT);

	/*---------------------Menu----------Title----------------App Label---------------Main Function--------------Create Button Function----------Animation-----------------App Exit Function-----*/
	
	appManager appObj0(MENU_canBus,    "CAN Bus",         APP_CANBUS, CAPTURE_CANBus, CAPTURE_createMenuBtns);
	appManager appObj1(MENU_tools,     "Tools",           APP_TOOLS, someFn, createToolBtns);
	appManager appObj2(MENU_settings,  "Settings",        APP_SETTINGS, someFn, createSettingsBtns);
	appManager appObj3(MENU_canBus,    "Capture",         APP_CAPTURE, CAPTURE_captureConfig, CAPTURE_createCaptureBtns);
	appManager appObj4(MENU_sub,       "CaptureLCD",      APP_CAPTURE_LCD, CAPTURE_LCD_scan, CAPTURE_createLCDBtns);
	appManager appObj5(MENU_canBus,    "Files",           APP_FILES, someFn, CAPTURE_drawCANLogScroll);
	appManager appObj6(MENU_canBus,    "FilterMask",      APP_FILTER_MASK, CAPTURE_filterMask, CAPTURE_createFilterMaskBtns);
	appManager appObj7(MENU_canBus,    "Send",            APP_SEND, someFn, SEND_createSendBtns);
	appManager appObj8(MENU_canBus,    "BaudRate",        APP_BAUD_RATE, CAPTURE_Baud, CAPTURE_createBaudBtns);
	
	this->myApps.push_back(appObj0);
	this->myApps.push_back(appObj1);
	this->myApps.push_back(appObj2);
	this->myApps.push_back(appObj3);
	this->myApps.push_back(appObj4);
	this->myApps.push_back(appObj5);
	this->myApps.push_back(appObj6);
	this->myApps.push_back(appObj7);
	this->myApps.push_back(appObj8);

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