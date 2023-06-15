/*
 ===========================================================================
 Name        : KeyInput.cpp
 Author      : Brandon Van Pelt
 Created	 : 11/08/2022 9:57:18 AM
 Description : USer key inputs
 ===========================================================================
 */

#define _KEYINPUT_C

 //#define DEBUG_KEYBOARD(x)  SerialUSB.println(x);
#define DEBUG_KEYBOARD(x)

#include "KeyInput.h"


// Call before using keypad to clear out old values from array
void resetKeypad()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		keypadInput[i] = 0;
	}
}

/*============== Hex Keypad ==============*/
// User input keypad
void drawKeypad()
{



	//uint16_t posY = 80;
	//uint8_t numPad = 0x00;

	// Clear LCD
	//drawSquareBtn(131, 55, 479, 319, "", themeBackground, themeBackground, themeBackground, CENTER);

	/*
	for (uint8_t i = 0; i < 3; i++)
	{
		int posX = 145;
		for (uint8_t j = 0; j < 6; j++)
		{
			if (numPad < 0x10)
			{
				//drawRoundBtn(posX, posY, posX + 50, posY + 40, String(numPad, HEX), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
				posX += 55;
				numPad++;
			}
		}
		posY += 45;
	}
	*/

	//drawRoundBtn(365, 170, 470, 210, F("<---"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	//drawRoundBtn(145, 220, 250, 260, F("Input:"), menuBackground, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	//drawRoundBtn(255, 220, 470, 260, F(" "), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	//drawRoundBtn(145, 270, 305, 310, F("Accept"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
	//drawRoundBtn(315, 270, 470, 310, F("Cancel"), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
}


/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Value contained in total
*
* index: Number place
* total: Current total added value selected
*/
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

/*============== Decimal Keypad ==============*/
// User input keypad
void drawKeypadDec()
{
	uint16_t posY = 125;
	uint8_t numPad = 0x00;

	// Clear LCD
	//drawSquareBtn(131, 55, 479, 319, "", themeBackground, themeBackground, themeBackground, CENTER);

	for (uint8_t i = 0; i < 2; i++)
	{
		int posX = 145;
		for (uint8_t j = 0; j < 6; j++)
		{
			if (numPad < 0x10)
			{
				//drawRoundBtn(posX, posY, posX + 50, posY + 40, String(numPad, HEX), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
				posX += 55;
				numPad++;
			}
		}
		posY += 45;
	}

	//drawRoundBtn(365, 170, 470, 210, F("<---"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(145, 220, 250, 260, F("Input:"), menuBackground, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(255, 220, 470, 260, F(" "), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(145, 270, 305, 310, F("Accept"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(315, 270, 470, 310, F("Cancel"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
}


/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Value contained in total
*
* index: Number place
* total: Current total added value selected
*/
uint8_t keypadControllerDec(uint8_t& index, uint16_t& total)
{
	uint8_t input = keypadButtonsDec();

	if (input >= 0x00 && input < 0x10 && index < 4)
	{
		keypadInput[3] = keypadInput[2];
		keypadInput[2] = keypadInput[1];
		keypadInput[1] = keypadInput[0];
		keypadInput[0] = input;
		total = keypadInput[0] * 1 + keypadInput[1] * 10 + keypadInput[2] * 100 + keypadInput[3] * 1000;
		//drawRoundBtn(255, 220, 470, 260, String(total), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
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
		total = keypadInput[0] * 1 + keypadInput[1] * 10 + keypadInput[2] * 100 + keypadInput[3] * 1000;
		//drawRoundBtn(255, 220, 470, 260, String(total), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
		(index > 0) ? --index : 0;
		return KEY_NO_CHANGE;
	}

	return input;
}

UserInterfaceClass userKeyButtons[41];
/*============== Keyboard ==============*/
// User input keypad
void createKeyboardButtons()
{
	drawSquareBtn(0, 165, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);
	drawSquareBtn(0, 160, 480, 165, "", menuBorder, menuBorder, menuBorder, ALIGN_CENTER);
	userKeyButtons[0].setButton(4, 167, 45, 203, 1, true, 6, F("1#"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[1].setButton(47, 167, 81, 203, 1, true, 6, F("q"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[2].setButton(83, 167, 117, 203, 1, true, 6, F("w"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[3].setButton(119, 167, 153, 203, 1, true, 6, F("e"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[4].setButton(155, 167, 189, 203, 1, true, 6, F("r"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[5].setButton(191, 167, 225, 203, 1, true, 6, F("t"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[6].setButton(227, 167, 261, 203, 1, true, 6, F("y"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[7].setButton(263, 167, 297, 203, 1, true, 6, F("u"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[8].setButton(299, 167, 333, 203, 1, true, 6, F("i"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[9].setButton(335, 167, 369, 203, 1, true, 6, F("o"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[10].setButton(371, 167, 405, 203, 1, true, 6, F("p"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[11].setButton(407, 167, 476, 203, 1, true, 6, F("<x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);

	userKeyButtons[12].setButton(4, 205, 70, 241, 1, true, 6, F("ABC"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[13].setButton(72, 205, 106, 241, 1, true, 6, F("a"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[14].setButton(108, 205, 142, 241, 1, true, 6, F("s"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[15].setButton(144, 205, 178, 241, 1, true, 6, F("d"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[16].setButton(180, 205, 214, 241, 1, true, 6, F("f"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[17].setButton(216, 205, 250, 241, 1, true, 6, F("g"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[18].setButton(252, 205, 286, 241, 1, true, 6, F("h"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[19].setButton(288, 205, 322, 241, 1, true, 6, F("j"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[20].setButton(324, 205, 358, 241, 1, true, 6, F("k"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[21].setButton(360, 205, 394, 241, 1, true, 6, F("l"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[22].setButton(396, 205, 476, 241, 1, true, 6, F("<--"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);

	userKeyButtons[23].setButton(4, 243, 41, 279, 1, true, 6, F("_"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[24].setButton(43, 243, 80, 279, 1, true, 6, F("-"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[25].setButton(82, 243, 120, 279, 1, true, 6, F("z"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[26].setButton(122, 243, 160, 279, 1, true, 6, F("x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[27].setButton(162, 243, 200, 279, 1, true, 6, F("c"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[28].setButton(202, 243, 240, 279, 1, true, 6, F("v"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[29].setButton(242, 243, 280, 279, 1, true, 6, F("b"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[30].setButton(282, 243, 320, 279, 1, true, 6, F("n"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[31].setButton(322, 243, 360, 279, 1, true, 6, F("m"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[32].setButton(362, 243, 399, 279, 1, true, 6, F("."), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[33].setButton(401, 243, 438, 279, 1, true, 6, F(","), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[35].setButton(440, 243, 476, 279, 1, true, 6, F(":"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);

	userKeyButtons[36].setButton(4, 281, 64, 317, 1, true, 6, F("x"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[37].setButton(66, 281, 126, 317, 1, true, 6, F("<"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[38].setButton(128, 281, 352, 317, 1, true, 6, F(" "), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[39].setButton(354, 281, 414, 317, 1, true, 6, F(">"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
	userKeyButtons[40].setButton(416, 281, 476, 317, 1, true, 6, F("_/"), ALIGN_CENTER, themeBackground, menuBackground, menuBtnTextColor);
}

bool drawkeyboard(uint8_t &graphicLoaderState)
{
	for (uint8_t btn = 0; btn < 41; btn++)
	{
		drawRoundBtn(userKeyButtons[btn].getXStart(), userKeyButtons[btn].getYStart(), userKeyButtons[btn].getXStop(), userKeyButtons[btn].getYStop(), userKeyButtons[btn].getBtnText(), userKeyButtons[btn].getBtnColor(), userKeyButtons[btn].getBorderColor(), userKeyButtons[btn].getTextColor(), userKeyButtons[btn].getAlign(), userKeyButtons[btn].getRadius());
	}
	return 1;
	/*
	uint16_t posY = 56;
	uint8_t numPad = 0x00;
	uint8_t count = 0;
	const char keyboardInput[37] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
									 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
									 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
									 'u', 'v', 'w', 'x', 'y', 'z', '_' };

	// Clear LCD
	//drawSquareBtn(131, 55, 479, 319, "", themeBackground, themeBackground, themeBackground, ALIGN_CENTER);

	for (uint8_t i = 0; i < 4; i++)
	{
		int posX = 135;
		for (uint8_t j = 0; j < 10; j++)
		{
			if (count < 37)
			{
				//drawRoundBtn(posX, posY, posX + 32, posY + 40, String(keyboardInput[count]), menuBtnColor, menuBtnBorder, menuBtnText, ALIGN_CENTER);
				posX += 34;
				count++;
			}
		}
		posY += 43;
	}

	//drawRoundBtn(373, 185, 475, 225, F("<--"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(135, 230, 240, 270, F("Input:"), menuBackground, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(245, 230, 475, 270, F(""), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(135, 275, 305, 315, F("Accept"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	//drawRoundBtn(310, 275, 475, 315, F("Cancel"), menuBtnColor, menuBtnBorder, menuBtnText, CENTER);
	*/
}


/*
* No change returns 0xFF
* Accept returns 0xF1
* Cancel returns 0xF0
* Value contained in global keyboardInput
*
* index: Letter place (0-7)
*/
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