// batteryMonitor.h

#ifndef _BATTERYMONITOR_h
#define _BATTERYMONITOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
String batteryLevel(uint16_t analogValue);

#endif

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