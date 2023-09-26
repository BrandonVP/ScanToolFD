#include "appManager.h"

appManager::appManager(void* printFunc, void* animationFunc, void* appFunc, void* cbFunc)
{

}

appManager::appManager(void* printFunc, void* appFunc, void* cbFunc)
{

}

appManager::appManager(void* printFunc, void* appFunc)
{
	printBtnPtr = (void(*)())printFunc;
	runAppPtr = (void(*)())appFunc;
}

void appManager::printButtons()
{
	return printBtnPtr();
}

void appManager::runApp()
{
	return runAppPtr();
}