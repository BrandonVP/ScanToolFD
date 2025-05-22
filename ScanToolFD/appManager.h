/*
===========================================================================
Name        : appManager.h
Author      : Brandon Van Pelt
Created	    : 9/26/2023
Description : Holds App objects for loading and unloading Apps
===========================================================================
*/

#ifndef APPMANAGER_H
#define APPMANAGER_H

#include "common.h"
#include <functional>

class appManager
{
public:
    appManager()
    {
        callBackPtr = NULL;
        printBtnPtr = NULL;
        runAnimationsPtr = NULL;
        runAppPtr = NULL;
    }

    appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void), void (*animationFunc)(void), bool (*CBFunc)(int));
    appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void), void (*animationFunc)(void));
    appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), uint8_t(*printFunc)(void));
    appManager(menus menuLocation, String descriptiveTxt, APP_labels label, void (*appFunc)(int), std::function<uint8_t(void)> CBFunc);

    uint8_t printButtons();
    bool isPrintButtonsNULL();
    void runApp(int userInput);
    bool isAppNULL();
    void runAnimations();
    bool isAnimationsNULL();
    bool executeCB(int userInput);
    bool isExecuteCBNULL();
    menus getAssignedMenu();
    String getName();
    APP_labels getAppLabel();

private:
    menus assignedMenu;
    String descriptiveName;
    APP_labels appLabel;
    void (*runAppPtr)(int);
    uint8_t(*printBtnPtr)(void);
    void (*runAnimationsPtr)(void);
    bool (*callBackPtr)(int);
    std::function<void(void)> CBFunc;

};
#endif // APPMANAGER_H

