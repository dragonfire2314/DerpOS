#include "vgaText.h"
#include "../io/serial.h"
#include "../global/types.h"

//Variables
char *vgaMem = (char*)0xb8000;

void puts(char *str, int x, int y, unsigned short color)
{
	int j = 0;
	int i = x;
	while (str[j] != '\0') {
		vgaMem[((80 * y) + i) * 2] = str[j];
		vgaMem[(((80 * y) + i) * 2) + 1] = color;
		++j;
		i++;
	}
}

void putc(char letter, int x, int y, unsigned short color)
{
	vgaMem[((80 * y) + x) * 2] = letter; //Places the charater
	vgaMem[(((80 * y) + x) * 2) + 1] = color; //Selects the color of the character
}

void cls(unsigned short color)
{
	int j = 0;
	while (j < 4000) {
		// blank character 
		vgaMem[j] = ' ';
		// attribute-byte
		vgaMem[j+1] = color;
		j = j + 2;
	}
}