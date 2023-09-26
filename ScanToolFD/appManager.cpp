#include "appManager.h"

appManager::appManager(void* printFunc, void* animationFunc, void* appFunc, void* cbFunc)
{

}

appManager::appManager(void* printFunc, void* appFunc, void* cbFunc)
{

}

appManager::appManager(void (*appFunc)(int), uint8_t(*printFunc)(void))
{
	runAppPtr = (void(*)(int))appFunc;
	printBtnPtr = (uint8_t(*)())printFunc;
}

uint8_t appManager::printButtons()
{
	return printBtnPtr();
}

void appManager::runApp(int userInput)
{
	return runAppPtr(userInput);
}
/*
// This function takes a std::function object as a parameter and returns an int
int apply(std::function<int(int&)> f, int& x) {
  // The function can call the std::function object with x as an argument
  return f(x);
}
*/