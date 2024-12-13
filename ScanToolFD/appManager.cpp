/*
 ===========================================================================
 Name        : appManager.cpp
 Author      : Brandon Van Pelt
 Created	 : 9/26/2023
 Description : Holds App objects for loading and unloading Apps
 =========================================================================== 
 */

#include "appManager.h"

appManager::appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void), void (*animationFunc)(void), void (*CBFunc)(void))
{
	assignedMenu = menuLocation;
	descriptiveName = descriptiveTxt;
	appLabel = label;
	runAppPtr = (void(*)(int))appFunc;
	printBtnPtr = (uint8_t(*)(void))printFunc;
	runAnimationsPtr = (void(*)(void))animationFunc;
	callBackPtr = (void(*)(void))CBFunc;
}

appManager::appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void), void (*animationFunc)(void))
{
	assignedMenu = menuLocation;
	descriptiveName = descriptiveTxt;
	appLabel = label;
	runAppPtr = (void(*)(int))appFunc;
	printBtnPtr = (uint8_t(*)(void))printFunc;
	runAnimationsPtr = (void(*)(void))animationFunc;
	callBackPtr = NULL;
}

appManager::appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void))
{
	assignedMenu = menuLocation;
	descriptiveName = descriptiveTxt;
	appLabel = label;
	runAppPtr = (void(*)(int))appFunc;
	printBtnPtr = (uint8_t(*)(void))printFunc;
	runAnimationsPtr = NULL;
	callBackPtr = NULL;
}

appManager::appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), std::function<uint8_t(void)> printFunc)
{
	assignedMenu = menuLocation;
	descriptiveName = descriptiveTxt;
	appLabel = label;
	runAppPtr = (void(*)(int))appFunc;
	CBFunc = CBFunc;
	runAnimationsPtr = NULL;
	callBackPtr = NULL;
}

uint8_t appManager::printButtons()
{
	return printBtnPtr();
}

bool appManager::isPrintButtonsNULL()
{
	return (printBtnPtr == NULL);
}

void appManager::runApp(int userInput)
{
	return runAppPtr(userInput);
}

bool appManager::isAppNULL()
{
	return (runAppPtr == NULL);
}

void appManager::runAnimations()
{
	return runAnimationsPtr();
}

bool appManager::isAnimationsNULL()
{
	return (runAnimationsPtr == NULL);
}

void appManager::executeCB()
{
	return callBackPtr();
}

bool appManager::isExecuteCBNULL()
{
	return (callBackPtr == NULL);
}

menus appManager::getAssignedMenu()
{
	return assignedMenu;
}

String appManager::getName()
{
	return descriptiveName;
}

APP_labels appManager::getAppLabel()
{
	return appLabel;
}