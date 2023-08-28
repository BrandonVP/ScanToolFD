/*
===========================================================================
Name        : batteryMonitor.cpp
Author      : Brandon Van Pelt
Company     : HP Tuners
Created     : 2023
Description : Battery monitoring functions
===========================================================================
*/

#define _BATTERYMONITOR_c
#include "batteryMonitor.h"

// Local variables
static uint32_t BATTERY_timer = 0;
static uint16_t oldValue = 0;

// Read in A1 and print battery level on icon
void BATTERY_printLevel(void)
{
	if (millis() - BATTERY_timer > 1000)
	{
		int batteryVoltage = analogRead(A1);
		if (batteryVoltage != oldValue)
		{
			oldValue = batteryVoltage;
			BATTERY_batteryLevelIcon(batteryVoltage);
		}
		BATTERY_timer = millis();
	}
}


// Prints battery level
void BATTERY_batteryLevelIcon(uint16_t analogValue)
{
	if ((analogValue >= 983))
	{
		display.fillRect(10, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(16, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(22, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(27, 14, 3, 15, ILI9488_GREEN);
	}
	else if (analogValue >= 943) 
	{
		display.fillRect(10, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(16, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(22, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(27, 14, 3, 15, menuBackground);
	}
	else if (analogValue >= 900)
	{
		display.fillRect(10, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(16, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(22, 14, 3, 15, menuBackground);
		display.fillRect(27, 14, 3, 15, menuBackground);
	}
	else if (analogValue >= 863)
	{
		display.fillRect(10, 14, 3, 15, ILI9488_GREEN);
		display.fillRect(16, 14, 3, 15, menuBackground);
		display.fillRect(22, 14, 3, 15, menuBackground);
		display.fillRect(27, 14, 3, 15, menuBackground);
	}
	else 
	{
		display.fillRect(10, 14, 3, 14, ILI9488_RED);
		display.fillRect(16, 14, 3, 15, menuBackground);
		display.fillRect(22, 14, 3, 15, menuBackground);
		display.fillRect(27, 14, 3, 15, menuBackground);
	}
}

// Text version of battery level
String BATTERY_batteryLevelText(uint16_t analogValue)
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
		return "100";
	}
	else if ((analogValue >= 993) && (analogValue < 1003))
	{
		return "90";
	}
	else if ((analogValue >= 983) && (analogValue < 993))
	{
		return "80";
	}
	else if ((analogValue >= 973) && (analogValue < 983))
	{
		return "70";
	}
	else if ((analogValue >= 963) && (analogValue < 973))
	{
		return "60";
	}
	else if ((analogValue >= 953) && (analogValue < 963))
	{
		return "50";
	}
	else if ((analogValue >= 943) && (analogValue < 953))
	{
		return "40";
	}
	else if ((analogValue >= 933) && (analogValue < 943))
	{
		return "30";
	}
	else if ((analogValue >= 933) && (analogValue < 943))
	{
		return "20";
	}
	else if ((analogValue >= 800) && (analogValue < 943))
	{
		return "10";
	}
	else return "OFF";
}