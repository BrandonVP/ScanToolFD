#include <SPI.h>
#include <Wire.h>
#include <Adafruit_FT6206.h> // Touch
#include <ILI9488_t3.h>      // Display
#include "ili9488_t3_font_Arial.h"
#include "ili9488_t3_font_ComicSansMS.h"

// Global LCD theme color variables
#define themeBackground 0xFFFF  // White
#define menuBtnText 0xFFFF      // White
#define menuBtnBorder 0x0000    // Black
#define menuBtnColor 0xFC00     // Orange
#define menuBackground 0xC618   //Silver

#define LEFT 1
#define CENTER 2
#define RIGHT 3

#define LED_R 24
#define LED_B 26
#define LED_G 25
#define LCD_BL 14
#define LCD_RST 15
#define ON LOW
#define OFF HIGH
#define TFT_CS 10
#define TFT_DC 9

ILI9488_t3 display = ILI9488_t3(&SPI, TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

int oldTouchX = 0;
int oldTouchY = 0;


// For touch controls
int x, y;

// Used for page control
uint8_t nextPage = 0;
uint8_t page = 0;
bool hasDrawn = false;

// *Used by background process*
uint8_t selectedChannelOut = 0;
uint8_t selectedSourceOut = 0;
uint32_t updateClock = 0;
bool isSerialOut = false;
bool isSDOut = false;
bool isMSGSpam = false;

// General use variables
uint8_t state = 0;

// Filter range / Filter Mask
uint32_t CAN0Filter = 0x000;
uint32_t CAN0Mask = 0x000;
uint32_t CAN1Filter = 0x000;
uint32_t CAN1Mask = 0x000;
uint32_t CANWiFiFilter = 0x000;
uint32_t CANWiFiMask = 0x000;

// Use to load pages in pieces to prevent blocking while loading entire page
uint8_t graphicLoaderState = 0;

// Simplifies getting x and y coords
bool Touch_getXY()
{
    if (ts.touched())
    {
        TS_Point p = ts.getPoint();
        x = p.y;
        y = SCREEN_HEIGHT - p.x;

        //Serial.print("X= ");
        //Serial.print(x);
        //Serial.print(" Y= ");
        //Serial.println(y);
        return true;
    }
    return false;
}

// Holds button down while pressed
void waitForIt(int x_start, int y_start, int x_stop, int y_stop)
{
    //myGLCD.setColor(themeBackground);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, themeBackground);
    while (ts.touched())
    {
        backgroundProcess();
    }

    //myGLCD.setColor(menuBtnBorder);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, menuBtnBorder);
}
void waitForItRect(int x_start, int y_start, int x_stop, int y_stop)
{
    //myGLCD.setColor(themeBackground);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), themeBackground);
    while (ts.touched())
    {
        backgroundProcess();
    }
    //myGLCD.setColor(menuBtnBorder);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), menuBtnBorder);
}

void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String button, int backgroundColor, int btnBorderColor, int btnTxtColor, int align) {
    int size, temp, offset;

    //display.setColor(backgroundColor);
    display.fillRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, backgroundColor);  // H_Start, V_Start, H_Stop, V_Stop
    //display.setColor(btnBorderColor);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, btnBorderColor);
    //display.setColor(btnTxtColor); // text color
    //display.setBackColor(backgroundColor); // text background


    switch (align) {
    case 1:
        //display.print(button, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8); // hor, ver
        break;
    case 2:
        size = button.length();
        temp = ((x_stop - x_start) / 2);
        offset = x_start + (temp - (8 * size));
        display.setCursor(offset, y_start + ((y_stop - y_start) / 2) - 8, false);
        display.setTextColor(btnTxtColor);
        display.println(button);
        //display.print(button, offset, y_start + ((y_stop - y_start) / 2) - 8); // hor, ver
        break;
    case 3:
        // Currently hotwired for deg text only
        //display.print(button, x_start + 55, y_start + ((y_stop - y_start) / 2) - 8); // hor, ver
        break;
    default:
        break;
    }
}
void drawSquareBtn(int x_start, int y_start, int x_stop, int y_stop, String button, int backgroundColor, int btnBorderColor, int btnTxtColor, int align) {
    int size, temp, offset;
    //display.setColor(backgroundColor);
    display.fillRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), backgroundColor);  // H_Start, V_Start, H_Stop, V_Stop
    //display.setColor(btnBorderColor);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBorderColor);
    //display.setColor(btnTxtColor); // text color
    //display.setBackColor(backgroundColor); // text background
    switch (align) {
    case 1:
        //display.print(button, x_start + 5, y_start + ((y_stop - y_start) / 2) - 8); // hor, ver
        break;
    case 2:
        size = button.length();
        temp = ((x_stop - x_start) / 2);
        offset = x_start + (temp - (8 * size));
        //display.print(button, offset, y_start + ((y_stop - y_start) / 2) - 8); // hor, ver
        break;
    case 3:
        //align left
        break;
    default:
        break;
    }
}

void drawCANBus() {
    drawSquareBtn(131, 55, 479, 319, "", themeBackground, themeBackground, themeBackground, CENTER);
    drawRoundBtn(140, 80, 305, 130, F("Capture"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(310, 80, 475, 130, F("Send"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(140, 135, 305, 185, F("Cap Files"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(310, 135, 475, 185, F("Baud"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(140, 190, 305, 240, F("FilterMask"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(310, 190, 475, 240, F("Auto Baud"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(140, 245, 305, 295, F(""), menuBackground, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(310, 245, 475, 295, F(""), menuBackground, menuBtnBorder, menuBtnText, CENTER);
}

void drawMenu() {
    // Draw Layout
    drawSquareBtn(0, 0, 479, 319, "", themeBackground, themeBackground, themeBackground, CENTER);
    drawSquareBtn(0, 0, 130, 319, "", menuBackground, menuBackground, menuBackground, CENTER);

    // Draw Menu Buttons
    drawRoundBtn(5, 32, 125, 83, F("CANBUS"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(5, 88, 125, 140, F("VEHTOOL"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(5, 145, 125, 197, F("UTVTOOL"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(5, 202, 125, 254, F("TESTING"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
    drawRoundBtn(5, 259, 125, 312, F("SETTING"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
}

// -------------------------------------------------------------
void setup(void)
{
    Serial.begin(9600); // USB is always 12 or 480 Mbit/sec

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(LCD_BL, OUTPUT);

    digitalWrite(LCD_BL, HIGH);
    digitalWrite(LED_B, OFF);
    digitalWrite(LED_G, OFF);
    digitalWrite(LED_R, OFF);
    digitalWrite(LED_R, ON);
    delay(200);
    digitalWrite(LED_R, OFF);

    digitalWrite(LED_G, ON);
    delay(200);
    digitalWrite(LED_G, OFF);

    digitalWrite(LED_B, ON);
    delay(200);
    digitalWrite(LED_B, OFF);

    if (!ts.begin(40)) {
        Serial.println("Unable to start touchscreen.");
    }
    else {
        Serial.println("Touchscreen started.");
    }

    display.begin();
    display.fillScreen(ILI9488_BLUE);
    display.setRotation(1);
    delay(1000);
    display.fillScreen(ILI9488_WHITE);
    delay(1000);
    display.setTextColor(ILI9488_BLACK);
    display.setFont(ComicSansMS_12);
    display.println("Arial_12");
    delay(2000);


    drawMenu();
    drawCANBus();

    Serial.println("This may be sent before your PC is able to receive");
    while (!Serial) {
        // wait for Arduino Serial Monitor to be ready
    }
    Serial.println("This line will definitely appear in the serial monitor");
}

// Resets variables for page change
void pageTransition()
{
    hasDrawn = false;
    graphicLoaderState = 0;
    page = nextPage;
}

// Button functions for the main menu
void menuButtons()
{
    // Touch screen controls
    if (Touch_getXY())
    {
        if ((x >= 5) && (x <= 125))
        {
            if ((y >= 32) && (y <= 83))
            {
                // CANBUS
                waitForIt(5, 32, 125, 83);
                nextPage = 0;
                graphicLoaderState = 0;
            }
            if ((y >= 88) && (y <= 140))
            {
                // VEHTOOL
                waitForIt(5, 88, 125, 140);
                nextPage = 9;
                graphicLoaderState = 0;
            }
            if ((y >= 145) && (y <= 197))
            {
                // RZRTOOL
                waitForIt(5, 145, 125, 197);
                nextPage = 18;
                graphicLoaderState = 0;
            }
            if ((y >= 202) && (y <= 254))
            {
                // EXTRAFN
                waitForIt(5, 202, 125, 254);
                nextPage = 27;
                graphicLoaderState = 0;
            }
            if ((y >= 259) && (y <= 312))
            {
                // SETTING
                waitForIt(5, 259, 125, 312);
                nextPage = 36;
                graphicLoaderState = 0;
            }
        }
    }
}

// All background process should be called from here
void backgroundProcess()
{
    // TODO
    menuButtons();
    //updateTime();
    //serialOut();
    //SDCardOut();
    //timedTXSend();
}

// Main loop
void loop(void)
{
    // GUI
    //pageControl();

    // Background Processes
    backgroundProcess();
}  
