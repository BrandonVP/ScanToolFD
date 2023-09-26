#pragma once
#include "common.h"
class appManager
{
private:
	void (*runAppPtr)(int);
	uint8_t(*printBtnPtr)(void);
	void (*runAnimationsPtr)(void);
	
	void (*callBackPtr)(void);
public:
	appManager(void* printFunc, void* animationFunc, void* appFunc, void* cbFunc);
	appManager(void* printFunc, void* appFunc, void* cbFunc);
	appManager(void (*appFunc)(int), uint8_t(*printFunc)(void));
	uint8_t printButtons();
	void runApp(int userInput);
};

