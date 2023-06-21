#ifndef COMMON_H
#define COMMON_H

//#include "CANBus.h"
//#include "SDCard.h"
#include "variableLock.h"
//#include "OBD_CAN_SIMULATOR.h"
#include <ILI9488_t3.h>      // Display
#include <Adafruit_FT6206.h> // Touch
//#include "definitions.h"
#include "userInterface.h"
#include "gui.h"
//#include "KeyInput.h"
#include "config.h"


extern uint8_t CANBusOut;
extern uint8_t CANBusIn;
extern uint32_t messageNum;

extern ILI9488_t3 display;

extern UserInterfaceClass userInterfaceButton[APP_BUTTON_SIZE];
extern UserInterfaceClass userInterfaceCaptureButton[CAPTURE_BUTTON_SIZE];
extern UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

extern uint16_t buttonsOnPage;
extern Adafruit_FT6206 ts;


extern int x, y;

extern uint8_t graphicLoaderState;


#endif // EXTERN_H