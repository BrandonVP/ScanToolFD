#pragma once
#include <cstdint>

#define FINISH_SIDE_LOADBAR 270
#define LOADBAR_TURN_OFF 471
#define LOADBAR_DEFAULT_RUN_TIME 8000
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#define DISPLAY_CS 28U
#define DISPLAY_DC 22U
#define DISPLAY_RST 00U

enum APP_labels {
	APP_CANBUS = 0,
	APP_TOOLS,
	APP_SETTINGS,
	APP_CAPTURE,
	APP_CAPTURE_LCD,
	APP_FILES,
	APP_FILTER_MASK,
	APP_SEND,
	APP_BAUD_RATE,
	APP_GET_VIN,
	APP_OBD_SIM,
	APP_TX_SPAM,
	APP_PID_SCAN,
	APP_PID_STREAM,
	APP_DTC,
	APP_GAUGES,
	APP_COUNT
};

extern APP_labels nextApp;
extern APP_labels activeApp;

enum menus {
	MENU_canBus = 0,
	MENU_tools,
	MENU_settings,
	MENU_sub
};

// Coordinates for different menu button layouts
const uint16_t MENU_4Center[4][4] = { {145,  75, 335, 125},
{145, 135, 335, 185},
{145, 195, 335, 245},
{145, 255, 335, 305} };

const uint16_t MENU_6Grid[6][4] = { {45, 95, 230, 145},  {250, 95, 435, 145},
{45, 155, 230, 205}, {250, 155, 435, 205},
{45, 215, 230, 265}, {250, 215, 435, 265} };

const uint16_t MENU_8Grid[8][4] = { {45,  75, 230, 125}, {250,  75, 435, 125},
{45, 135, 230, 185}, {250, 135, 435, 185},
{45, 195, 230, 245}, {250, 195, 435, 245},
{45, 255, 230, 305}, {250, 255, 435, 305} };

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define SD_CARD_CS_PIN 2
#define LCD_BL 14
#define LCD_RST 15
#define TFT_CS 10
#define TFT_DC 9

// Global LCD theme color variables

#define themeBackground 0x424B // White
#define menuBtnText 0xFFFF // White
#define menuBtnTextColor 0xBE18 // White-ish
#define menuBtnBorder 0x869B // 
#define menuBtnColor 0x0516// Button Blue
#define menuBackground 0x5B0E // Dark top menu background
#define menuBorder 0x39E8 // 
#define frameBorder 0x8452 // 
#define OrangeBtnColor 0xFC00 // Orange
#define BlackBtnColor 0x0000 // Black
/*
#define themeBackground 0xFFFF // White
#define menuBtnText 0x0000 // White
#define menuBtnBorder 0x0000 // Black
#define menuBtnColor 0xFC00 // Orange
#define menuBackground 0xC618 //Silver
*/

// CAN Bus message configuration
#define USE_CAN1			1 // Messages on CAN0
#define USE_CAN2			2 // Messages on CAN1
#define USE_CAN3			3 // Messages on CAN1
#define USE_BOTH			4 // Messages on CAN0 and CAN1
#define USE_BRIDGE_CAN1_RX  5 // Bridge CAN0 and CAN1 but only show CAN1 RX messages (One way capture)
#define USE_BRIDGE_BOTH		6 // Bridge CAN0 and CAN1 and show both messages
#define USE_WIFI			7 // Messages over WIFI

//#define DEBUG(x)
//#define DEBUG(x)  Serial.println(x);
#define DEBUG_HEX(x, z)  SerialUSB.print(x, z);
#define DEBUG(x)  SerialUSB.print(x);

//#define DEBUG_ERROR(x)
//#define DEBUG_ERROR(x)  Serial.println(x);
#define DEBUG_ERROR(x)  SerialUSB.println(x);

#define ECU_RX 0x7E8
#define CAN_PID_ID 0x7DF

// This defines the char length of a formatted message
// Inpacts reading and writing from SD card class and message streaming from the canbus class
#define MSG_STRING_LENGTH       (67)
#define SD_CAPTURE_NUM_MSG      (10)
#define SD_CAPTURE_BLOCK_SIZE   (SD_CAPTURE_NUM_MSG * MSG_STRING_LENGTH) // SD_CAPTURE_NUM_MSG * MSG_STRING_LENGTH

#define PIDSAMPLES 5

// For the draw shape functions
#define ALIGN_LEFT 1
#define ALIGN_CENTER 2
#define ALIGN_RIGHT 3

#define CANBUS_MAIN 0
#define VEHTOOL_MAIN 9
#define UTVTOOL_MAIN 18
#define TESTING_MAIN 27
#define SETTING_MAIN 36

// Error checking
typedef bool error_t;

// Define items to show for scroll function
#define MAXSCROLL 6

// Bitmap
#define BUFFPIXEL 20

// Prevents physical button doubletap
#define BUTTON_DELAY 200

// Checks single bit of binary number
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

// Loadbar
#define DONE 8

// Buttons
#define MENU_BUTTON_SIZE 5
#define CAPTURE_BUTTON_SIZE 15
#define APP_BUTTON_SIZE 30


//
#define PIDLIST "PIDScan.txt"

#define VERSION "Version 4.8.1"

// Used for converting keypad input to appropriate hex place
const uint32_t hexTable[8] = { 1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456 };

// List of baud rates for Baud page
const uint32_t baudRates[6] = { 1000000, 800000, 500000, 250000, 125000, 100000 };

/*
uint8_t PIDS[193] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
					  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
					  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
					  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
					  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
					  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
					  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
					  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
					  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
					  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
					  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
					  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
					  0xC0
					 };
*/

#define CAN_BPS_250K 250000
#define CAN_BPS_500K 500000

constexpr auto BATTERY_ENABLE_READ = A10;
constexpr auto BATTERY_READ = A11;

/*
Version Log
4.3.1 - Brandon Van Pelt


*/

// Unused pins on back
// 27 28 29 32 33
// 
// if removed LED
// 24, 25, 26

// SPI0 
// SCK0  13 // Orange
// MISO0 12 // Blue
// MOSI0 11 // White/Pink
// CS0   10 // Brown

// SPI1
// SCK1  27
// MISO1 1
// MOSI1 26
// CS1   0

// SPI2  
// SCK2  37
// MISO2 34
// MOSI2 35
// CS2   36

// Blue   - MISO
// White  - MOSI
// Orange - SCK
// Brown  - CS

/*
45 (E)
53 (S)
50 (P)
52 (82)
42 (66)

98
F4
AB
B4
40
2B
0

0
FF
0
26
2C

FD // ENDING_BYTE

45
53
50
52
42

98
F4
AB
B4
40
2B

C8
FF
FF
FF
FF
FF
FD

*/

//#define SEND_MAC                (0xAC)
//#define SEND_MAC_CONFIRM        (0xAD)
//#define CONNECT_NEW_DONGLE      (0xCA)
//#define CONNECT_DONGLE_CONFIRM  (0xCC)
//#define RESET_DEVICE            (0xBA)
//#define RESET_DEVICE_CONFIRM    (0xBF)
//	delay(100);
//	Serial2.write(CONNECT_NEW_DONGLE);
//	delay(1);
//	Serial2.write(CONNECT_DONGLE_CONFIRM);
//	delay(1);
//	Serial2.write(0xC8);
//	delay(1);
//	Serial2.write(0xC9);
//	delay(1);
//	Serial2.write(0xA3);
//	delay(1);
//	Serial2.write(0xF9);
//	delay(1);
//	Serial2.write(0xFD);
//	delay(1);
//	Serial2.write(0x04);
//	delay(500);
//	Serial2.write(RESET_DEVICE);
//	delay(1);
//	Serial2.write(RESET_DEVICE_CONFIRM);
//	delay(2500);
//	Serial2.write(SEND_MAC);
//	delay(1);
//	Serial2.write(SEND_MAC_CONFIRM);