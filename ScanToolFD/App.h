/*
===========================================================================
Name        : App.h
Author      : Brandon Van Pelt
Created	    : 6/2/2024
Description : Class to load and run Apps
===========================================================================
*/

#ifndef APP_H
#define APP_H

#include "common.h"
#include "config.h"
#include "appManager.h"

//extern class appManager a; 
typedef struct appManager appManager;

class App
{

public:
	// Used for page control
	bool hasDrawn = false;

	APP_labels nextApp = APP_CANBUS;
	APP_labels activeApp = APP_CANBUS;
	std::vector<appManager> myApps;
	uint8_t buttonsOnPage = 0;

	App()
	{
	}

	void init();
	void run();
	uint8_t printMenu(menus menu, APP_labels label, uint8_t size);
	void appTransition();
	void newApp(APP_labels);
	int getAppSize(void);
	int getMenu(int index);
	APP_labels getLabel(int index);
	String getName(int index);
	APP_labels getActiveApp();
	void backgroundProcess();
	uint8_t getButtonsOnPage();
	void setButtonsOnPage(uint8_t buttons);
};
#endif // APP_H
