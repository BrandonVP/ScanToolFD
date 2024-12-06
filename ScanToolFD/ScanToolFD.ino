/*
 ===========================================================================
 Name        : ScanToolFD.ino
 Author      : Brandon Van Pelt
 Created	 : 3/24/2023
 Description : Main
 ===========================================================================
 */

 /*=========================================================
	 Todo List
 ===========================================================
 Wires config
 C2 out only config
 SD FD format for 64 byte data
 read sd logs on card
 -delete
 -replay
 -view

 Tools
 -MSG spam
 -OBD Sim?
 -VIN

 Bug
 Moving off button after selecting will prevent deselect
 ===========================================================
	 End Todo List
 =========================================================*/

 /*=========================================================
	 README
 ===========================================================
	To create a new page:
	1.) Assign page in page control
	2.) Create buttons
	3.) Call function to draw buttons in assigned page switch statement hasDrawn section
	4.) Call function to monitor button press in assigned page switch statement loop section

	Swap SD SPI port in SdSpiTeensy3.cpp
	Change: m_spi = &SPI;
	To: m_spi = &SPI2;
 ===========================================================
	 End README
 =========================================================*/

#include "filterMask.h"
#include "baudRate.h"
#include <MTP_Teensy.h>
#include <TimeLib.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> // Touch
#include <ILI9488_t3.h>      // Display
#include "icons.h"
#include "SDCard.h"
#include "batteryMonitor.h"
#include "CANBusCapture.h"
#include "RGB_LED.h"
#include "cbBuffer.h"
#include "cbBufferFD.h"

#include "CANBusSend.h"


File myFile;
ILI9488_t3 display = ILI9488_t3(&SPI, TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

App app;
cbBuffer can1Buffer;
cbBuffer can2Buffer;
cbBufferFD can3Buffer;
UserInterfaceClass userInterfaceButton[APP_BUTTON_SIZE];
UserInterfaceClass userInterfaceCaptureButton[CAPTURE_BUTTON_SIZE];
UserInterfaceClass userInterfaceMenuButton[MENU_BUTTON_SIZE];

// Use to load pages in pieces to prevent blocking while loading entire page
uint8_t graphicLoaderState = 0;

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
//FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can3;
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> Can3; // FD

bool enableCB1 = true;
bool enableCB2 = true;
bool enableCB3 = true;

// Arduino library time update
time_t getTeensy3Time()
{
	return Teensy3Clock.get();
}

//
void dateTime(uint16_t* date, uint16_t* time)
{
	*date = FAT_DATE(year(), month(), day());
	*time = FAT_TIME(hour(), minute(), second());
}

//
void drawMenu()
{
	GUI_drawSquareBtn(0, 0, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
	GUI_drawSquareBtn(0, 0, 480, 45, "", menuBackground, menuBackground, menuBackground, ALIGN_CENTER);
	GUI_drawSquareBtn(0, 45, 480, 50, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
	GUI_drawSquareBtn(160, 45, 260, 50, "", menuBtnColor, menuBtnColor, menuBtnColor, ALIGN_CENTER);

	// Create button objects
	createMenuBtns();

	// Draw Menu
	while (GUI_drawPage(userInterfaceMenuButton, graphicLoaderState, 3));
	graphicLoaderState = 0;

	ICONS_printIcon(5, 5, battery_bits, 32, 4, menuBtnTextColor, 1);
	display.updateScreen();
}

//
void setCANBusFD(FLEXCAN_CLOCK clock, int baud)
{
	CANFD_timings_t config;
	config.clock = clock; // CLK_24MHz;// CLK_60MHz;
	config.baudrate = baud;
	config.baudrateFD = baud;
	config.propdelay = 190;
	config.bus_length = 1;
	config.sample = 70;
	Can3.setBaudRate(config);
}

#define ESP_BLE_TX 29
#define ESP_BLE_RX 28

#define ESP_WIFI_TX 35
#define ESP_WIFI_RX 34

// -------------------------------------------------------------
void setup(void)
{
	Serial.begin(115200); // USB is always 12 or 480 Mbit/sec
	//while (!Serial);
	Serial1.begin(115200); 
	Serial4.begin(115200); 
	Serial7.setTX(ESP_BLE_TX);
	Serial7.setRX(ESP_BLE_RX);
	Serial7.begin(115200);
	Serial8.setTX(ESP_WIFI_TX);
	Serial8.setRX(ESP_WIFI_RX);
	Serial8.begin(115200);

	// Update time
	setSyncProvider(getTeensy3Time);

	pinMode(BATTERY_ENABLE_READ, OUTPUT);
	pinMode(BATTERY_READ, INPUT);

	//pinMode(2, OUTPUT);
	//pinMode(3, OUTPUT);
	//pinMode(4, OUTPUT);
	//pinMode(5, OUTPUT);
	//pinMode(6, OUTPUT);
	//pinMode(37, OUTPUT);

	//digitalWrite(2, LOW);
	//digitalWrite(3, LOW);
	//digitalWrite(4, LOW);
	//digitalWrite(5, LOW);
	//digitalWrite(6, LOW);
	//digitalWrite(37, LOW);

	/*
	LED_initialize();

	LED_RGB((RGB)LED_RED);
	delay(200);
	LED_RGB((RGB)LED_OFF);

	LED_RGB((RGB)LED_GREEN);
	delay(200);
	LED_RGB((RGB)LED_OFF);

	LED_RGB((RGB)LED_BLUE);
	delay(200);
	LED_RGB((RGB)LED_OFF);
	*/

	pinMode(LCD_BL, OUTPUT);
	digitalWrite(LCD_BL, HIGH);

	ts.begin(40);
	display.begin();
	display.useFrameBuffer(true);
	display.fillScreen(ILI9488_BLACK);
	display.setRotation(1);

	//display.setFont(Arial_14);
	//display.setFont(AwesomeF180_12);
	display.setFont(Michroma_11);
	//display.setFont(ComicSansMS_12);
	//display.setFont(OpenSans12);
	//display.setFont(Crystal_18_Italic);
	//display.setFont(Michroma_12);

	Can1.begin();
	Can1.setBaudRate(500000);
	Can1.setMaxMB(8);
	Can1.setMB((FLEXCAN_MAILBOX)0, RX, STD);
	Can1.setMB((FLEXCAN_MAILBOX)1, RX, EXT);
	Can1.setMB((FLEXCAN_MAILBOX)2, TX, STD);
	Can1.setMB((FLEXCAN_MAILBOX)3, TX, EXT);
	Can1.setMBFilter(REJECT_ALL);
	Can1.enableMBInterrupts();
	Can1.onReceive(MB0, CANBus1_IRQHandler);
	Can1.onReceive(MB1, CANBus1_IRQHandler);
	Can1.setMBFilter(MB0, 0x0, 0x7FF);
	Can1.setMBFilter(MB1, 0x0, 0x1FFFFFF);
	Can1.mailboxStatus();

	Can2.begin();
	Can2.setBaudRate(500000); 
	Can2.setMaxMB(8);
	Can2.setMB((FLEXCAN_MAILBOX)0, RX, STD);
	Can2.setMB((FLEXCAN_MAILBOX)1, RX, EXT);
	Can2.setMB((FLEXCAN_MAILBOX)2, TX, STD);
	Can2.setMB((FLEXCAN_MAILBOX)3, TX, EXT);
	Can2.setMBFilter(REJECT_ALL);
	Can2.enableMBInterrupts();
	Can2.onReceive(MB0, CANBus2_IRQHandler);
	Can2.onReceive(MB1, CANBus2_IRQHandler);
	Can2.setMBFilter(MB0, 0x0, 0x7FF);
	Can2.setMBFilter(MB1, 0x0, 0x1FFFFFF);
	Can2.mailboxStatus();


	CANFD_timings_t config;
	config.clock = CLK_24MHz;
	config.baudrate = 500000;
	config.baudrateFD = 500000;
	config.propdelay = 190;
	config.bus_length = 1;
	config.sample = 70;
	Can3.setBaudRate(config);


	Can3.begin();
	//Can3.setMB((FLEXCAN_MAILBOX)0, RX, STD);
	//Can3.setMB((FLEXCAN_MAILBOX)1, RX, EXT);
	//Can3.setMB((FLEXCAN_MAILBOX)2, TX, STD);
	//Can3.setMB((FLEXCAN_MAILBOX)3, TX, EXT);
	//Can3.setMBFilter(REJECT_ALL);
	//Can3.enableMBInterrupts();
	//Can3.setMBFilter(MB0, 0x0, 0x7FF);
	//Can3.setMBFilter(MB1, 0x0, 0x1FFFFFF);
	//Can3.mailboxStatus();
	//setCANBusFD(CLK_24MHz, 500000); //CLK_24MHz;// CLK_60MHz; // CAPTURE_CANBusFDBaudRate
	Can3.setRegions(64);
	CAPTURE_createCaptureBtns();

	MTP.begin();
	SD.begin(BUILTIN_SDCARD); // SD_CARD_CS_PIN // LCD sd reader in prototype
	SdFile::dateTimeCallback(dateTime);
	MTP.addFilesystem(SD, "SD Card");

	pinMode(6, OUTPUT);
	digitalWrite(6, LOW);

	app.init();
	drawMenu();
}

//
void createMenuBtns()
{
	// Create Menu Buttons
	uint8_t menuPosition = 0;

	userInterfaceMenuButton[menuPosition++].setButton(155, 0, 260, 45, APP_CANBUS, true, 0, F("CAN Bus"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
	userInterfaceMenuButton[menuPosition++].setButton(265, 0, 365, 45, APP_TOOLS, true, 0, F("Tools"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
	userInterfaceMenuButton[menuPosition++].setButton(370, 0, 475, 45, APP_SETTINGS, true, 0, F("Settings"), ALIGN_CENTER, menuBackground, menuBackground, menuBtnTextColor);
}

//
void CANBus1_IRQHandler(const CAN_message_t& msg)
{
	LED_pulse((RGB)LED_RED);
	//Serial.printf("1: %03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	//Serial.printf("%d\n", msg.mb);
	can1Buffer.push_cb(msg.id, msg.len, msg.buf);
}

//
void CANBus2_IRQHandler(const CAN_message_t& msg)
{
	LED_pulse((RGB)LED_BLUE);
	//Serial.printf("2: %03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
	can2Buffer.push_cb(msg.id, msg.len, msg.buf);
}

//
void CANBus3_IRQHandler()
{
	CANFD_message_t msg;
	if (Can3.read(msg))
	{
		LED_pulse((RGB)LED_GREEN);
		//Serial.print(msg.id, HEX);
		//Serial.print("   ");
		//for (int i = 0; i < msg.len; i++)
		//{
		//	Serial.print(msg.buf[i], HEX);
		//	Serial.print("  ");
		//}
		//Serial.println();
		can3Buffer.push_cb(msg.id, msg.len, msg.buf);
	}
	//Serial.printf("%03X  %d  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", msg.id, msg.len, msg.buf[0], msg.buf[1], msg.buf[2], msg.buf[3], msg.buf[4], msg.buf[5], msg.buf[6], msg.buf[7]);
}

//
void CANBus3_IRQHandler2()
//void CANBus3_IRQHandler(const CANFD_message_t& msg)
{
	CANFD_message_t msg;
	if (Can3.read(msg))
	{
		LED_pulse((RGB)LED_GREEN);
		Serial.print(msg.id, HEX);
		Serial.print("   ");
		for (int i = 0; i < msg.len; i++)
		{
			Serial.print(msg.buf[i], HEX);
			Serial.print("  ");
		}
		Serial.println();
	}
}


/*=========================================================
	Background Processes
===========================================================*/
// All background process should be called from here
void backgroundProcess()
{
	Can1.events();
	Can2.events();
	//Can3.events();
	GUI_buttonMonitor(userInterfaceMenuButton, MENU_BUTTON_SIZE);
	BATTERY_printLevel();
	updateTime();
	MTP.loop();
	//LED_strobe((RGB)LED_OFF);
	CANBus3_IRQHandler();
	CAPTURE_processSerialCapture();
	CAPTURE_processWirelessCapture();
	//timedTXSend();
}

// Displays time in menu
void updateTime()
{
	static uint32_t updateClock = 0;
	if (millis() - updateClock > 999)
	{
		display.setTextColor(menuBtnTextColor);
		char printStringT[64];
		char printStringB[64];

		display.setFont(Michroma_8);

		// menuBackground

		sprintf(printStringT, "%2d:%2d:%2d", hour(), minute(), second());
		sprintf(printStringB, "%2d/%2d/%2d", month(), day(), year());

		// Print to screen
		display.useFrameBuffer(false);
		display.fillRect(64, 8, 88, 27, menuBackground);
		display.drawString(printStringT, 65, 10);
		display.drawString(printStringB, 65, 25);
		display.useFrameBuffer(true);

		// Update buffer but DO NOT print buffer
		display.fillRect(64, 8, 88, 27, menuBackground);
		display.drawString(printStringT, 65, 10);
		display.drawString(printStringB, 65, 25);

		display.setFont(Michroma_11);
		updateClock = millis();
	}
}

void debugLoop()
{
	static uint32_t test123 = 0;
	static bool swapMe = false;
#if 0
	static uint32_t timeout123 = millis();
	static uint16_t rotatingID = 0;
	if (millis() - timeout123 > 2000)
	{
		if (CAPTURE_input_config == BTN_config_input_C3)
		{
			CANFD_message_t msg;
			msg.id = 0x4CA;
			msg.len = 64;
			for (uint8_t i = 0; i < 64; i++) msg.buf[i] = i + 0x25;
			Can3.write(msg);
		}
		else if (CAPTURE_input_config == BTN_config_input_C2)
		{
			CAN_message_t msg;
			msg.id = rotatingID++;
			msg.len = 8;
			for (uint8_t i = 0; i < 8; i++) msg.buf[i] = (uint8_t)millis() + (i * 0x3C);
			Can2.write(msg);
		}
		else if (CAPTURE_input_config == BTN_config_input_C1)
		{
			CAN_message_t msg;
			msg.id = rotatingID++;
			msg.len = 8;
			for (uint8_t i = 0; i < 8; i++) msg.buf[i] = (uint8_t)millis() + (i * 0x3C);
			Can1.write(msg);
		}
		timeout123 = millis();
	}
#endif
#if 0
	if (millis() - test123 > 500)
	{
		CAN_message_t msg_vehicle;
		CAN_Frame msgRX;

		msg_vehicle.id = 123;
		msg_vehicle.buf[0] = 1;
		msg_vehicle.buf[1] = 2;
		msg_vehicle.buf[2] = 3;
		msg_vehicle.buf[3] = 4;
		msg_vehicle.buf[4] = 5;
		msg_vehicle.buf[5] = 6;
		msg_vehicle.buf[6] = 7;
		msg_vehicle.buf[7] = 8;

		Can1.write(msg_vehicle);
		Can2.write(msg_vehicle);

		Can3.write(msg_vehicle);

		test123 = millis();

	}
#endif
#if 0
	if (millis() - test123 > 500)
	{
		if (swapMe)
		{
			Serial.println("if");
			swapMe = false;
			digitalWrite(3, HIGH);
			digitalWrite(4, HIGH);
			digitalWrite(5, HIGH);
			digitalWrite(6, HIGH);
			digitalWrite(7, HIGH);
			digitalWrite(8, HIGH);
		}
		else
		{
			Serial.println("else");
			swapMe = true;
			digitalWrite(3, LOW);
			digitalWrite(4, LOW);
			digitalWrite(5, LOW);
			digitalWrite(6, LOW);
			digitalWrite(7, LOW);
			digitalWrite(8, LOW);
		}
		test123 = millis();
		Serial.println(millis());
	}
#endif
}
/*=========================================================
	Main loop
===========================================================*/
// Main loop runs the user interface and calls for background processes
void loop(void)
{
	app.run();
	backgroundProcess();
	debugLoop();
}
