/*
 ===========================================================================
 Name        : RGB_LED.cpp
 Author      : Brandon Van Pelt
 Created	 : 6/23/2023
 Description : Control the on board LED
 ===========================================================================
 */

#define _RGB_LED_CPP
#include "RGB_LED.h"

/*=========================================================
            RGB LED
===========================================================*/
void LED_initialize(void)
{
    pinMode(LED1_R, OUTPUT);
    pinMode(LED2_R, OUTPUT);
    pinMode(LED1_G, OUTPUT);
    pinMode(LED2_G, OUTPUT);
    pinMode(LED1_B, OUTPUT);
    pinMode(LED2_B, OUTPUT);
    digitalWrite(LED2_B, OFF);
    digitalWrite(LED1_B, OFF);
    digitalWrite(LED2_G, OFF);
    digitalWrite(LED1_G, OFF);
    digitalWrite(LED2_R, OFF);
    digitalWrite(LED1_R, OFF);
}

// Set color and on/off status of RGB LED
void LED_RGB(RGB color)
{
    switch (color)
    {
        case LED_RED:
            digitalWrite(LED1_R, ON);
            digitalWrite(LED2_R, ON);
            break;
        case LED_GREEN:
            digitalWrite(LED1_G, ON);
            digitalWrite(LED2_G, ON);
            break;
        case LED_BLUE:
            digitalWrite(LED1_B, ON);
            digitalWrite(LED2_B, ON);
            break;
        case LED_YELLOW:
            digitalWrite(LED1_G, ON);
            digitalWrite(LED2_G, ON);
            digitalWrite(LED1_R, ON);
            digitalWrite(LED2_R, ON);
            break;
        case LED_PURPLE:
            digitalWrite(LED1_B, ON);
            digitalWrite(LED2_B, ON);
            digitalWrite(LED1_R, ON);
            digitalWrite(LED2_R, ON);
            break;
        case LED_TEAL:
            digitalWrite(LED1_B, ON);
            digitalWrite(LED2_B, ON);
            digitalWrite(LED1_G, ON);
            digitalWrite(LED2_G, ON);
            break;
        case LED_OFF:
            digitalWrite(LED1_B, OFF);
            digitalWrite(LED2_B, OFF);
            digitalWrite(LED1_G, OFF);
            digitalWrite(LED2_G, OFF);
            digitalWrite(LED1_R, OFF);
            digitalWrite(LED2_R, OFF);
            break;
    }
}

// Strobe Red Blue Green (RBG) colors
void LED_strobe(void)
{
    // Cycles values 1-3 for the RGB colors
    static uint8_t color = LED_RED;

    if (millis() - LEDTimer > LED_STROBE_INTERVAL)
    {
        LED_RGB((RGB)LED_OFF);
        LED_RGB((RGB)color);
        (color > LED_BLUE) ? color = LED_RED : color++;
        LEDTimer = millis();
    }
}

// Strobe Red Blue Green (RBG) colors
void LED_strobe(RGB color)
{
    if (millis() - LEDTimer > LED_STROBE_INTERVAL)
    {
        if (cycleLED)
        {
            LED_RGB((RGB)color);
            cycleLED = false;
        }
        else
        {
            LED_RGB((RGB)LED_OFF);
            cycleLED = true;
        }
        LEDTimer = millis();
    }
}

// Pulse a color and reset timer
void LED_pulse(RGB color)
{
    LED_RGB((RGB)LED_OFF);
    LED_RGB((RGB)color);
    LEDTimer = millis();
}

// Strobe Red Blue Green (RBG) colors
void LED_fast_strobe(RGB color)
{
    if (millis() - LEDTimer > (LED_STROBE_INTERVAL / 2))
    {
        LED_RGB(color);
        cycleLED = !cycleLED;
        LEDTimer = millis();
    }
}
