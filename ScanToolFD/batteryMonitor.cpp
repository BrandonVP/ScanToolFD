// 
// 
// 

#include "batteryMonitor.h"


String batteryLevel(uint16_t analogValue)
{
	if (analogValue >= 1023)
	{
		return "AUX";
	}
	else if ((analogValue >= 1013) && (analogValue < 1023))
	{
		return "100";
	}
	else if ((analogValue >= 1003) && (analogValue < 1013))
	{
		return "90";
	}
	else if ((analogValue >= 993) && (analogValue < 1003))
	{
		return "80";
	}
	else if ((analogValue >= 983) && (analogValue < 993))
	{
		return "70";
	}
	else if ((analogValue >= 973) && (analogValue < 983))
	{
		return "60";
	}
	else if ((analogValue >= 963) && (analogValue < 973))
	{
		return "50";
	}
	else if ((analogValue >= 953) && (analogValue < 963))
	{
		return "40";
	}
	else if ((analogValue >= 943) && (analogValue < 953))
	{
		return "30";
	}
	else if ((analogValue >= 933) && (analogValue < 943))
	{
		return "20";
	}
	else if ((analogValue >= 933) && (analogValue < 943))
	{
		return "10";
	}
	else if ((analogValue >= 800) && (analogValue < 943))
	{
		return "0";
	}
	else return "OFF";
}