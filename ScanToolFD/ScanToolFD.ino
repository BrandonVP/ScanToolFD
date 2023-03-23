/*
 * Teensy 4.0 with LittlevGL Graphic library on a 480 x 320 ILI9488 LCD
 *
 * May 2020
 * skpang.co.uk
 *
  MIT License
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 */

#include <lvgl.h>
#include <SPI.h>
#include "Adafruit_FT6206.h"
#include <Wire.h>
#include <ILI9488_t3.h>
#include "ili9488_t3_font_Arial.h"
#include "ili9488_t3_font_ComicSansMS.h"

#define LV_CONF_INCLUDE_SIMPLE


 // Global LCD theme color variables
#define themeBackground 0xFFFF // White
#define menuBtnText 0xFFFF // White
#define menuBtnBorder 0x0000 // Black
#define menuBtnColor 0xFC00 // Orange
#define menuBackground 0xC618 //Silver

#define LEFT 1
#define CENTER 2
#define RIGHT 3

#define LED_R 24
#define LED_B 26
#define LED_G 25
#define LCD_BL 14
#define LCD_RST 15
#define ON  LOW
#define OFF HIGH
#define TFT_CS 10
#define TFT_DC  9
ILI9488_t3 display = ILI9488_t3(&SPI, TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

#define LVGL_TICK_PERIOD 60
int screenWidth = 480;
int screenHeight = 320;

int oldTouchX = 0;
int oldTouchY = 0;

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 40];

int led = 13;

void drawRoundBtn(int x_start, int y_start, int x_stop, int y_stop, String button, int backgroundColor, int btnBorderColor, int btnTxtColor, int align) {
    int size, temp, offset;

    //display.setColor(backgroundColor);
    display.fillRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, backgroundColor); // H_Start, V_Start, H_Stop, V_Stop
    //display.setColor(btnBorderColor);
    display.drawRoundRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), 1, btnBorderColor);
    //display.setColor(btnTxtColor); // text color
    //display.setBackColor(backgroundColor); // text background


    switch (align)
    {
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
    display.fillRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), backgroundColor); // H_Start, V_Start, H_Stop, V_Stop
    //display.setColor(btnBorderColor);
    display.drawRect(x_start, y_start, (x_stop - x_start), (y_stop - y_start), btnBorderColor);
    //display.setColor(btnTxtColor); // text color
    //display.setBackColor(backgroundColor); // text background
    switch (align)
    {
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

void drawCANBus()
{
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

void drawMenu()
{
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


IntervalTimer tick;
static void lv_tick_handler(void)
{

    lv_tick_inc(LVGL_TICK_PERIOD);
}

bool my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
{
    uint16_t touchX, touchY;

    if (ts.touched())
    {
        // Retrieve a point  
        TS_Point p = ts.getPoint();

        touchX = p.y;
        touchY = p.x;
        touchY = 320 - touchY;
        Serial.print("X= ");
        Serial.print(touchX);
        Serial.print(" Y= ");
        Serial.println(touchY);

        if ((touchX != oldTouchX) || (touchY != oldTouchY))
        {

            oldTouchY = touchY;
            oldTouchX = touchX;
            data->state = LV_INDEV_STATE_PR;
            //  data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; 
            data->point.x = touchX;
            data->point.y = touchY;

        }
    }
    else
    {

        data->point.x = oldTouchX;
        data->point.y = oldTouchY;
        data->state = LV_INDEV_STATE_REL;
    }
    return 0;
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{

    uint16_t width = (area->x2 - area->x1 + 1);
    uint16_t height = (area->y2 - area->y1 + 1);

    display.writeRect(area->x1, area->y1, width, height, (uint16_t*)color_p);

    lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */

}


static void event_handler(lv_obj_t* obj, lv_event_t event)
{

    if (event == LV_EVENT_CLICKED) {
        Serial.printf("Clicked\n");
    }
    else if (event == LV_EVENT_VALUE_CHANGED) {
        Serial.printf("Toggled\n");
    }
}

// -------------------------------------------------------------
void setup(void)
{
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
    //display.setFont(Arial_12);
    display.println("Arial_12");
    delay(2000);
    lv_init();
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 40);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
    indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
    lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/


    //demo_create(); // Create the demo in /lv_example/lv_apps/demo/demo.c

    /*
    lv_ex_btn_1();
    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Hello Teensy 4.0 LittlevGL on ILI9488 skpang.co.uk");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, -85);
    */

    tick.begin(lv_tick_handler, LVGL_TICK_PERIOD * 1000);  // Start tick timer
    Serial.println("tick.begin");
    drawMenu();
    drawCANBus();
    delay(10000);
}

void printEvent(String Event, lv_event_t event)
{

    Serial.print(Event);
    printf(" ");

    switch (event) {
    case LV_EVENT_PRESSED:
        printf("Pressed\n");
        break;

    case LV_EVENT_SHORT_CLICKED:
        printf("Short clicked\n");
        break;

    case LV_EVENT_CLICKED:
        printf("Clicked\n");
        break;

    case LV_EVENT_LONG_PRESSED:
        printf("Long press\n");
        break;

    case LV_EVENT_LONG_PRESSED_REPEAT:
        printf("Long press repeat\n");
        break;

    case LV_EVENT_RELEASED:
        printf("Released\n");
        break;
    }
}

void lv_ex_btn_1(void)
{
    lv_obj_t* label;

    lv_obj_t* btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_handler);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button");

    lv_obj_t* btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn2, event_handler);
    lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 40);
    //lv_btn_set_toggle(btn2, true);
    lv_btn_toggle(btn2);
    lv_btn_set_fit2(btn2, LV_FIT_NONE, LV_FIT_TIGHT);

    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, "Toggled");
}
// -------------------------------------------------------------
void loop(void)
{

    //lv_task_handler(); /* let the GUI do its work */
    //delay(5);

} // Main loop
