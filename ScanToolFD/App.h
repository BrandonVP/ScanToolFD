#pragma once
#ifndef APP_H
#define APP_H

#include "common.h"
#include "config.h"
#include "appManager.h"

using namespace std;

extern class appManager a; 

class App
{

public:
	// Used for page control
	bool hasDrawn = false;

	APP_labels nextApp = APP_CANBUS;
	APP_labels activeApp = APP_CANBUS;
	vector<appManager> myApps;
	uint8_t buttonsOnPage = 0;

	App()
	{
	}

	void init();
	uint8_t printMenu(menus menu, APP_labels label);
	void appTransition();
	void newApp(APP_labels);
	int getAppSize(void);
	int getMenu(int index);
	APP_labels getLabel(int index);
	String getName(int index);
	APP_labels getActiveApp();
	void run();

private:

};
#endif
