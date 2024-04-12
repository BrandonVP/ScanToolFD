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
 Baud
 filter mask
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

// -------------------------------------------------------------
void setup(void)
{
	Serial.begin(115200); // USB is always 12 or 480 Mbit/sec
	//while (!Serial);
	Serial2.begin(115200); // ESP8266
	
	// Update time
	setSyncProvider(getTeensy3Time);

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
	setCANBusFD(CLK_24MHz, CAPTURE_CANBusFDBaudRate); //CLK_24MHz;// CLK_60MHz;
	Can3.setRegions(64);
	CAPTURE_createCaptureBtns();

	MTP.begin();
	SD.begin(SD_CARD_CS_PIN);
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

	LED_strobe((RGB)LED_OFF);

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
	//if (Serial2.available())
	//{
		//Serial.println(Serial2.read(), 16);
	//}

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
}
/*=========================================================
	Main loop
===========================================================*/
// Main loop runs the user interface and calls for background processes
void loop(void)
{
	app.run();
	backgroundProcess();
	//debugLoop();
}
