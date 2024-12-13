/*
 ===========================================================================
 Name        : appManager.h
 Author      : Brandon Van Pelt
 Created	 : 9/26/2023
 Description : Holds App objects for loading and unloading Apps
 ===========================================================================
 */

enum menus {
	MENU_canBus = 0,
	MENU_tools,
	MENU_settings,
	MENU_sub
};

#ifndef APPMANAGER_H
#define APPMANAGER_H

#pragma once
#include "common.h"
#include "config.h"
#include <functional>

class appManager
{
private:
	menus assignedMenu;
	String descriptiveName;
	APP_labels appLabel;
	void (*runAppPtr)(int);
	uint8_t (*printBtnPtr)(void);
	void (*runAnimationsPtr)(void);
	void (*callBackPtr)(void);

	std::function<void(void)> CBFunc;
public:
	appManager()
	{

	}

	appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void), void (*animationFunc)(void), void (*CBFunc)(void));
	appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void), void (*animationFunc)(void));
	appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void));
	appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), std::function<uint8_t(void)> CBFunc);

	uint8_t printButtons();
	bool isPrintButtonsNULL();
	void runApp(int userInput);
	bool isAppNULL();
	void runAnimations();
	bool isAnimationsNULL();
	void executeCB();
	bool isExecuteCBNULL();
	menus getAssignedMenu();
	String getName();
	APP_labels getAppLabel();
};
#endif

