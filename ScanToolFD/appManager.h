#pragma once
class appManager
{
private:
	void (*printBtnPtr)(void);
	//void* (*f)(void*)
	void (*runAnimationsPtr)(void);
	void (*runAppPtr)(void);
	void (*callBackPtr)(void);
public:
	appManager(void* printFunc, void* animationFunc, void* appFunc, void* cbFunc);
	appManager(void* printFunc, void* appFunc, void* cbFunc);
	appManager(void* printFunc, void* appFunc);
	void printButtons();
	void runApp();
};

