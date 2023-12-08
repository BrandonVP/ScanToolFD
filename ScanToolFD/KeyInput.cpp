/*
 ===========================================================================
 Name        : KeyInput.cpp
 Author      : Brandon Van Pelt
 Company     : HP Tuners
 Created	 : 11/08/2022
 Description : User key inputs
 ===========================================================================
 */

#define _KEYINPUT_C
#include "KeyInput.h"

 // Local variables
UserInterfaceClass userKeyButtons[45];
uint8_t keyPadButtons = 0;
uint8_t keypadInput[5] = { 0, 0, 0, 0, 0 };
uint8_t hexInput[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// Print the lower case keyboard
uint8_t KEYINPUT_createKeyboardButtons(void)
{
	uint8_t index = 0;
	userKeyButtons[index].setButton(0, 165, 479, 319, 1, true, 6, F(""), ALIGN_CENTER, themeBackground, themeBackground, themeBackground);
	userKeyButtons[index++].setClickable(false);
	userKeyButtons[index].setButton(0, 163, 480, 165, 1, true, 6, F(""), ALIGN_CENTER, menuBorder, menuBorder, menuBorder);
	userKeyButtons[index++].setClickable(false);

	userKeyButtons[index++].setButton(4, 167, 45, 203, 1, true, 6, F("1#"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(47, 167, 81, 203, 'q', true, 6, F("q"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(83, 167, 117, 203, 'w', true, 6, F("w"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(119, 167, 153, 203, 'e', true, 6, F("e"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(155, 167, 189, 203, 'r', true, 6, F("r"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(191, 167, 225, 203, 't', true, 6, F("t"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(227, 167, 261, 203, 'y', true, 6, F("y"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(263, 167, 297, 203, 'u', true, 6, F("u"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(299, 167, 333, 203, 'i', true, 6, F("i"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(335, 167, 369, 203, 'o', true, 6, F("o"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(371, 167, 405, 203, 'p', true, 6, F("p"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(407, 167, 476, 203, 0xF2, true, 6, F("<x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 205, 70, 241, 0xc1, true, 6, F("ABC"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(72, 205, 106, 241, 'a', true, 6, F("a"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(108, 205, 142, 241, 's', true, 6, F("s"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(144, 205, 178, 241, 'd', true, 6, F("d"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(180, 205, 214, 241, 'f', true, 6, F("f"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(216, 205, 250, 241, 'g', true, 6, F("g"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(252, 205, 286, 241, 'h', true, 6, F("h"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(288, 205, 322, 241, 'j', true, 6, F("j"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(324, 205, 358, 241, 'k', true, 6, F("k"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(360, 205, 394, 241, 'l', true, 6, F("l"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(396, 205, 476, 241, 1, true, 6, F("<--"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 243, 41, 279, '_', true, 6, F("_"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(43, 243, 80, 279, '-', true, 6, F("-"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(82, 243, 120, 279, 'z', true, 6, F("z"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(122, 243, 160, 279, 'x', true, 6, F("x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(162, 243, 200, 279, 'c', true, 6, F("c"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(202, 243, 240, 279, 'v', true, 6, F("v"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(242, 243, 280, 279, 'b', true, 6, F("b"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(282, 243, 320, 279, 'n', true, 6, F("n"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(322, 243, 360, 279, 'm', true, 6, F("m"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(362, 243, 399, 279, '.', true, 6, F("."), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(401, 243, 438, 279, ',', true, 6, F(","), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(440, 243, 476, 279, ':', true, 6, F(":"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 281, 64, 317, 1, true, 6, F("x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(66, 281, 126, 317, 1, true, 6, F("<"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(128, 281, 352, 317, 1, true, 6, F(" "), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(354, 281, 414, 317, 1, true, 6, F(">"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(416, 281, 476, 317, 1, true, 6, F("_/"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	return index;
}

// Print the upper case keyboard
uint8_t KEYINPUT_createUpperCaseButtons()
{
	uint8_t index = 0;
	userKeyButtons[index].setButton(0, 165, 479, 319, 1, true, 6, F(""), ALIGN_CENTER, themeBackground, themeBackground, themeBackground);
	userKeyButtons[index++].setClickable(false);
	userKeyButtons[index].setButton(0, 163, 480, 165, 1, true, 6, F(""), ALIGN_CENTER, menuBorder, menuBorder, menuBorder);
	userKeyButtons[index++].setClickable(false);

	userKeyButtons[index++].setButton(47, 167, 81, 203, 'Q', true, 6, F("Q"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(83, 167, 117, 203, 'W', true, 6, F("W"), ALIGN_LEFT, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(119, 167, 153, 203, 'E', true, 6, F("E"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(155, 167, 189, 203, 'R', true, 6, F("R"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(191, 167, 225, 203, 'T', true, 6, F("T"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(227, 167, 261, 203, 'Y', true, 6, F("Y"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(263, 167, 297, 203, 'U', true, 6, F("U"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(299, 167, 333, 203, 'I', true, 6, F("I"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(335, 167, 369, 203, 'O', true, 6, F("O"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(371, 167, 405, 203, 'P', true, 6, F("P"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(407, 167, 476, 203, 0xF2, true, 6, F("<x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 205, 70, 241, 0xc2, true, 6, F("abc"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(72, 205, 106, 241, 'A', true, 6, F("A"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(108, 205, 142, 241, 'S', true, 6, F("S"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(144, 205, 178, 241, 'D', true, 6, F("D"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(180, 205, 214, 241, 'F', true, 6, F("F"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(216, 205, 250, 241, 'G', true, 6, F("G"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(252, 205, 286, 241, 'H', true, 6, F("H"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(288, 205, 322, 241, 'J', true, 6, F("J"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(324, 205, 358, 241, 'K', true, 6, F("K"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(360, 205, 394, 241, 'L', true, 6, F("L"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(396, 205, 476, 241, 1, true, 6, F("<--"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(4, 243, 41, 279, '_', true, 6, F("_"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(43, 243, 80, 279, '-', true, 6, F("-"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(82, 243, 120, 279, 'Z', true, 6, F("Z"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(122, 243, 160, 279, 'X', true, 6, F("X"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(162, 243, 200, 279, 'C', true, 6, F("C"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(202, 243, 240, 279, 'V', true, 6, F("V"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(242, 243, 280, 279, 'B', true, 6, F("B"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(282, 243, 320, 279, 'N', true, 6, F("N"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(322, 243, 360, 279, 'M', true, 6, F("M"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	return index;
}

// Print the hex keypad
uint8_t KEYINPUT_createHexpadButtons(int offset)
{
	uint8_t index = 0;
	userKeyButtons[index++].setButton(248, 55, 303, 105, 0x0, true, 6, F("0"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(306, 55, 361, 105, 0x1, true, 6, F("1"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(364, 55, 419, 105, 0x2, true, 6, F("2"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(422, 55, 477, 105, 0x3, true, 6, F("3"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(248, 108, 303, 158, 0x4, true, 6, F("4"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(306, 108, 361, 158, 0x5, true, 6, F("5"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(364, 108, 419, 158, 0x6, true, 6, F("6"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(422, 108, 477, 158, 0x7, true, 6, F("7"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(248, 161, 303, 211, 0x8, true, 6, F("8"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(306, 161, 361, 211, 0x9, true, 6, F("9"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(364, 161, 419, 211, 0xA, true, 6, F("A"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(422, 161, 477, 211, 0xB, true, 6, F("B"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(248, 214, 303, 264, 0xC, true, 6, F("C"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(306, 214, 361, 264, 0xD, true, 6, F("D"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(364, 214, 419, 264, 0xE, true, 6, F("E"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(422, 214, 477, 264, 0xF, true, 6, F("F"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index].setButton(248, 267, 419, 317, 0, true, 6, F(""), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setClickable(false);

	userKeyButtons[index++].setButton(422, 267, 477, 317, 0x10, true, 6, F("del"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	return index;
}

// Print the decimal keypad
uint8_t KEYINPUT_createNumpadButtons(int offset)
{
	uint8_t index = 0;
	userKeyButtons[index++].setButton(280, 74 + offset, 340, 124 + offset, 7, true, 6, F("7"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(345, 74 + offset, 405, 124 + offset, 8, true, 6, F("8"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(410, 74 + offset, 470, 124 + offset, 9, true, 6, F("9"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(280, 128 + offset, 340, 178 + offset, 4, true, 6, F("4"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(345, 128 + offset, 405, 178 + offset, 5, true, 6, F("5"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(410, 128 + offset, 470, 178 + offset, 6, true, 6, F("6"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(280, 182 + offset, 340, 232 + offset, 1, true, 6, F("1"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(345, 182 + offset, 405, 232 + offset, 2, true, 6, F("2"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(410, 182 + offset, 470, 232 + offset, 3, true, 6, F("3"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);

	userKeyButtons[index++].setButton(280, 236 + offset, 405, 286 + offset, 0, true, 6, F("0"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	userKeyButtons[index++].setButton(410, 236 + offset, 470, 286 + offset, 0x10, true, 6, F("del"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnColor, menuBtnTextColor);
	return index;
}

//
void  KEYINPUT_clearInput()
{
	for (uint8_t i = 0; i < 5; i++)
	{
		keypadInput[i] = 0;
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		hexInput[i] = 0;
	}
}

/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Back returns 0x10;
* Value contained in total
*
* index: Number place
* total: Current total added value selected
*/
// Controller for decimal keypad entry
uint8_t KEYINPUT_keypadController(uint16_t& total, uint8_t& numIndex, uint8_t maxSize)
{
	uint8_t input = GUI_subMenuButtonMonitor(userKeyButtons, keyPadButtons);

	if (input >= 0x00 && input < 10 && numIndex < maxSize)
	{
		keypadInput[4] = keypadInput[3];
		keypadInput[3] = keypadInput[2];
		keypadInput[2] = keypadInput[1];
		keypadInput[1] = keypadInput[0];
		keypadInput[0] = input;
		total = (keypadInput[0] * 1) + (keypadInput[1] * 10) + (keypadInput[2] * 100) + (keypadInput[3] * 1000) + (keypadInput[4] * 10000);

		++numIndex;
		return KEY_CHANGE;
	}
	else if (input == 0x10)
	{
		switch (numIndex)
		{
		case 1:
			keypadInput[0] = 0;
			keypadInput[1] = 0;
			keypadInput[2] = 0;
			keypadInput[3] = 0;
			break;
		case 2:
			keypadInput[0] = keypadInput[1];
			keypadInput[1] = 0;
			keypadInput[2] = 0;
			keypadInput[3] = 0;
			break;
		case 3:
			keypadInput[0] = keypadInput[1];
			keypadInput[1] = keypadInput[2];
			keypadInput[2] = 0;
			keypadInput[3] = 0;
			break;
		case 4:
			keypadInput[0] = keypadInput[1];
			keypadInput[1] = keypadInput[2];
			keypadInput[2] = keypadInput[3];
			keypadInput[3] = 0;
			break;
		case 5:
			keypadInput[0] = keypadInput[1];
			keypadInput[1] = keypadInput[2];
			keypadInput[2] = keypadInput[3];
			keypadInput[3] = keypadInput[4];
			keypadInput[4] = 0;
			break;
		}
		total = (keypadInput[0] * 1) + (keypadInput[1] * 10) + (keypadInput[2] * 100) + (keypadInput[3] * 1000) + (keypadInput[4] * 10000);
		(numIndex > 0) ? --numIndex : 0;
		return KEY_CHANGE;
	}
	return KEY_NO_CHANGE;
}

// Controller for hex keypad entry
uint8_t KEYINPUT_keypadHexController(uint32_t& total, uint8_t& numIndex, uint8_t maxSize)
{
	uint8_t input = GUI_subMenuButtonMonitor(userKeyButtons, keyPadButtons);
	uint8_t maxIndex = maxSize;
	(maxIndex > 8) ? maxIndex = 8 : maxIndex = maxIndex;

	if ((input >= 0x00) && (input < 0x10) && (numIndex < maxIndex))
	{
		for (uint8_t i = 7; i > 0; i--)
		{
			hexInput[i] = hexInput[i - 1];
		}
		hexInput[0] = input;

		total = (hexInput[0] << 0) + (hexInput[1] << 4) + (hexInput[2] << 8) + (hexInput[3] << 12)
			+ (hexInput[4] << 16) + (hexInput[5] << 20) + (hexInput[6] << 24) + (hexInput[7] << 28);

		GUI_drawRoundBtn(248, 267, 419, 317, String(total, 16), themeBackground, menuBackground, menuBtnTextColor, ALIGN_CENTER, 6);

		//Serial.printf("input: %X \n", input);
		//Serial.printf("total: %X \n", total);

		++numIndex;
		return KEY_CHANGE;
	}
	else if (input == 0x10)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			if (i < numIndex - 1)
			{
				hexInput[i] = hexInput[i + 1];
			}
			else
			{
				hexInput[i] = 0;
			}
		}

		total = (hexInput[0] << 0) + (hexInput[1] << 4) + (hexInput[2] << 8) + (hexInput[3] << 12)
			+ (hexInput[4] << 16) + (hexInput[5] << 20) + (hexInput[6] << 24) + (hexInput[7] << 28);

		GUI_drawRoundBtn(248, 267, 419, 317, String(total, 16), themeBackground, menuBackground, menuBtnTextColor, ALIGN_CENTER, 6);

		//Serial.printf("total: %X \n", total);
		(numIndex > 0) ? --numIndex : 0;
		return KEY_CHANGE;
	}
	return KEY_NO_CHANGE;
}

/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Value contained in global keyboardInput
*
* index: Letter place (0-7)
*/
uint8_t KEYINPUT_keyboardController(uint8_t& index, char *keyboardInput)
{
	uint8_t input = GUI_subMenuButtonMonitor(userKeyButtons, keyPadButtons);

	if (input > 0x29 && input < 0x7B && index < 8) // 8 is max size of a filename
	{
		keyboardInput[index] = input;
		
		++index;
		return KEY_CHANGE;
	}
	else if (input == 0xF2 && index > 0)
	{
		keyboardInput[index - 1] = 0x20;
		
		--index;
		return KEY_CHANGE;
	}
	else if (input == 0xc1)
	{
		KEYINPUT_createUpperCaseButtons();
		graphicLoaderState = 0;
		while (GUI_drawPage(userKeyButtons, graphicLoaderState, keyPadButtons));
	}
	else if (input == 0xc2)
	{
		KEYINPUT_createKeyboardButtons();
		graphicLoaderState = 0;
		while (GUI_drawPage(userKeyButtons, graphicLoaderState, keyPadButtons));
	}

	return KEY_NO_CHANGE;
}