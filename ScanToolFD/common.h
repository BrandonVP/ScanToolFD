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
#include <FlexCAN_T4.h>
#include "config.h"


extern uint8_t CANBusOut;
extern uint8_t CANBusIn;
extern uint32_t messageNum;

extern ILI9488_t3 display;

extern UserInterfaceClass userInterfaceButton[APP_BUTTON_SIZE];
extern UserInterfaceClass userInterfaceCaptureButton[CAPTURE_BUTTON_SIZE];
extern UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

extern void clearAppSpace();

extern FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
extern FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> Can3;

extern uint16_t buttonsOnPage;
extern Adafruit_FT6206 ts;


extern int x, y;

extern uint8_t graphicLoaderState;


#endif // EXTERN_H