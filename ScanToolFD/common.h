#ifndef COMMON_H
#define COMMON_H

#include "arduino.h"
#include <ILI9488_t3.h>      // Display
#include <Adafruit_FT6206.h> // Touch
#include "font_Michroma.h"
#include "userInterface.h"
#include "config.h"
#include <vector>
#include <FlexCAN_T4.h>
#include "ili9488_t3_font_Arial.h"
//#include "OBD_CAN_SIMULATOR.h"

/*
#include "ili9488_t3_font_ComicSansMS.h"
#include "ili9488_t3_font_ArialBold.h"
#include "font_AwesomeF180.h"
#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_ComicSansMS.h"
#include "font_OpenSans.h"
#include "font_DroidSans.h"
#include "font_Michroma.h"
#include "font_Crystal.h"
#include "font_ChanceryItalic.h"
*/

extern ILI9488_t3 display;
extern Adafruit_FT6206 ts;
extern UserInterfaceClass userInterfaceButton[APP_BUTTON_SIZE];
extern UserInterfaceClass userInterfaceCaptureButton[CAPTURE_BUTTON_SIZE];
extern UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];
extern void setCANBusFD(FLEXCAN_CLOCK clock, int baud);
extern FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
//extern FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can3;
extern FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> Can3;
extern int x, y;
extern uint8_t graphicLoaderState;

#endif // EXTERN_H