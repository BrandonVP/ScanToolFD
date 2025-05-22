/*
===========================================================================
Name        : icons.h
Author      : Brandon Van Pelt
Created:    : 07/05/2023
Description : Print custom icons
===========================================================================
*/

#include "icons.h"

// The bits are reversed when exported from icon studio
unsigned int Icons::reverseBits(uint8_t num)
{
    unsigned int NO_OF_BITS = sizeof(num) * 2;
    unsigned int reverse_num = 0, i, temp;

    for (i = 0; i < NO_OF_BITS; i++)
    {
        temp = (num & (1 << i));
        if (temp) reverse_num |= (1 << ((NO_OF_BITS - 1) - i));
    }
    return reverse_num;
}

// Print custom made icons
void Icons::printIcon(uint16_t x, uint16_t y, const uint8_t icon[], uint8_t maxRow, uint8_t maxColumn, uint16_t color1, uint16_t color2)
{
    int16_t i = 0, row, column, bit, temp;
    for (row = 0; row < maxRow; row++) //64
    {
        for (column = 0; column < maxColumn; column++) // 8
        {
            temp = icon[i];
            for (bit = 7; bit >= 0; bit--)
            {
                if (temp & 1)
                {
                    display.drawPixel(x + (column * 8) + (8 - bit), y + row, color1);
                }
                else
                {
                    // display.drawPixel(x + (column * 8) + (8 - bit), y + row, menuBtnColor); // Optionally print the other side of the image in a color different than the background
                }
                temp >>= 1;
            }
            i++;
        }
    }
}
