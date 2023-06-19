/*
 ===========================================================================
 Name        : KeyInput.cpp
 Author      : Brandon Van Pelt
 Created	 : 11/08/2022 9:57:18 AM
 Description : User key inputs
 ===========================================================================
 */

#define _KEYINPUT_C
#include "KeyInput.h"

/*============== Hex/Dec Numpad ==============*/
/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Value contained in total
*
* index: Number place
* total: Current total added value selected
*/
/*
uint8_t keypadController(uint8_t& index, uint16_t& total)
{
	uint8_t input = keypadButtons();

	if (input >= 0x00 && input < 0x10 && index < 3)
	{
		keypadInput[2] = keypadInput[1];
		keypadInput[1] = keypadInput[0];
		keypadInput[0] = input;
		total = 0;// = keypadInput[0] * hexTable[0] + keypadInput[1] * hexTable[1] + keypadInput[2] * hexTable[2];
		//drawRoundBtn(255, 220, 470, 260, String(total, 16), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
		++index;
		return KEY_NO_CHANGE;
	}
	else if (input == 0x10)
	{
		switch (index)
		{
		case 1:
			keypadInput[0] = 0;
			break;
		case 2:
			keypadInput[0] = keypadInput[1];
			keypadInput[1] = 0;
			break;
		case 3:
			keypadInput[0] = keypadInput[1];
			keypadInput[1] = keypadInput[2];
			keypadInput[2] = 0;
			break;
		}
		total = 0;// keypadInput[0] * hexTable[0] + keypadInput[1] * hexTable[1] + keypadInput[2] * hexTable[2];
		//drawRoundBtn(255, 220, 470, 260, String(total, 16), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
		(index > 0) ? --index : 0;
		return KEY_NO_CHANGE;
	}
	
	return input;
}
*/


/*============== Keyboard ==============*/
// User input keypad

void createKeyboardButtons()
{
	uint8_t index = 0;
	userKeyButtons[index].setButton(0, 165, 479, 319, 1, true, 6, F(""), ALIGN_CENTER, themeBackground, themeBackground, themeBackground);
	userKeyButtons[index++].setClickable(false);
	userKeyButtons[index].setButton(0, 160, 480, 165, 1, true, 6, F(""), ALIGN_CENTER, menuBorder, menuBorder, menuBorder);
	userKeyButtons[index++].setClickable(false);

	userKeyButtons[index++].setButton(4, 167, 45, 203, 1, true, 6, F("1#"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(47, 167, 81, 203, 1, true, 6, F("q"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(83, 167, 117, 203, 1, true, 6, F("w"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(119, 167, 153, 203, 1, true, 6, F("e"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(155, 167, 189, 203, 1, true, 6, F("r"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(191, 167, 225, 203, 1, true, 6, F("t"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(227, 167, 261, 203, 1, true, 6, F("y"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(263, 167, 297, 203, 1, true, 6, F("u"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(299, 167, 333, 203, 1, true, 6, F("i"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(335, 167, 369, 203, 1, true, 6, F("o"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(371, 167, 405, 203, 1, true, 6, F("p"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(407, 167, 476, 203, 1, true, 6, F("<x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 205, 70, 241, 1, true, 6, F("ABC"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(72, 205, 106, 241, 1, true, 6, F("a"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(108, 205, 142, 241, 1, true, 6, F("s"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(144, 205, 178, 241, 1, true, 6, F("d"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(180, 205, 214, 241, 1, true, 6, F("f"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(216, 205, 250, 241, 1, true, 6, F("g"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(252, 205, 286, 241, 1, true, 6, F("h"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(288, 205, 322, 241, 1, true, 6, F("j"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(324, 205, 358, 241, 1, true, 6, F("k"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(360, 205, 394, 241, 1, true, 6, F("l"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(396, 205, 476, 241, 1, true, 6, F("<--"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 243, 41, 279, 1, true, 6, F("_"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(43, 243, 80, 279, 1, true, 6, F("-"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(82, 243, 120, 279, 1, true, 6, F("z"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(122, 243, 160, 279, 1, true, 6, F("x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(162, 243, 200, 279, 1, true, 6, F("c"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(202, 243, 240, 279, 1, true, 6, F("v"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(242, 243, 280, 279, 1, true, 6, F("b"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(282, 243, 320, 279, 1, true, 6, F("n"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(322, 243, 360, 279, 1, true, 6, F("m"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(362, 243, 399, 279, 1, true, 6, F("."), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(401, 243, 438, 279, 1, true, 6, F(","), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(440, 243, 476, 279, 1, true, 6, F(":"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 281, 64, 317, 1, true, 6, F("x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(66, 281, 126, 317, 1, true, 6, F("<"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(128, 281, 352, 317, 1, true, 6, F(" "), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(354, 281, 414, 317, 1, true, 6, F(">"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(416, 281, 476, 317, 1, true, 6, F("_/"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
}

/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Value contained in global keyboardInput
*
* index: Letter place (0-7)
*/
/*
uint8_t keyboardController(uint8_t& index)
{
	uint8_t input = keyboardButtons();

	if (input > 0x29 && input < 0x7B && index < 8) // 8 is max size of a filename
	{
		keyboardInput[index] = input;
		//drawRoundBtn(245, 230, 475, 270, String(keyboardInput), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
		++index;
		return KEY_NO_CHANGE;
	}
	else if (input == 0xF2 && index > 0)
	{
		keyboardInput[index - 1] = 0x20;
		//drawRoundBtn(245, 230, 475, 270, String(keyboardInput), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
		--index;
		return KEY_NO_CHANGE;
	}

	return input;
}
*/