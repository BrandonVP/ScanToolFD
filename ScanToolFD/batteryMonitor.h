/*
===========================================================================
Name        : batteryMonitor.h
Author      : Brandon Van Pelt
Created     : 2023
Description : Battery monitoring functions
===========================================================================
*/

/*
Samples Taken 7/29/2023 using PSU
8.2V 1023
8.1V 1016
8.0v 1008
7.9v 1001
7.8v 993
7.7v 986
7.6v 978
7.5v 971
7.4v 964
7.3v 956
7.2v 948
7.1v 941
7.0v 932
*/

#ifndef _BATTERYMONITOR_h
#define _BATTERYMONITOR_h

#include "common.h"

#ifdef _BATTERYMONITOR_c

void BATTERY_printLevel(void);
void BATTERY_batteryLevelIcon(uint16_t analogValue);
String BATTERY_batteryLevelText(uint16_t analogValue);

#else

void BATTERY_printLevel(void);
String BATTERY_batteryLevelText(uint16_t analogValue);

#endif // _BATTERYMONITOR_c
#endif // _BATTERYMONITOR_h



