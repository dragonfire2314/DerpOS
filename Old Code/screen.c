#include "system.h"

#define SCREENSIZE 4000

char *screen = (char*)0xb8000;

void putc(char letter, int x, int y, unsigned short color)
{
	screen[((80 * y) + x) * 2] = letter; //Places the charater
	screen[(((80 * y) + x) * 2) + 1] = color; //Selects the color of the character
}

void puts(char *str, int x, int y, unsigned short color)
{
	int j = 0;
	int i = x;
	while (str[j] != '\0') {
		screen[((80 * y) + i) * 2] = str[j];
		screen[(((80 * y) + i) * 2) + 1] = color;
		++j;
		i++;
	}
}

void putd(char data, char x, char y, unsigned short color)
{
    putc(symbol_map[((data & 0xF0) >> 4)], x+2, y, color);
    putc(symbol_map[(data & 0x0F)], x+3, y, color);
}

void cls(unsigned short color)
{
	int j = 0;
	while (j < SCREENSIZE) {
		/* blank character */
		screen[j] = ' ';
		/* attribute-byte */
		screen[j+1] = color;
		j = j + 2;
	}
}

unsigned short MakeColor(unsigned char bg, unsigned char fg)
{
	return (bg << 4) | (fg & 0x0F);
}