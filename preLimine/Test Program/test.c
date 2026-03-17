#include "lib/derpOS.h"

//extern "C" int Usermain();

int Usermain()
{
	//putc('M', 1, 1, 0x40);
	//putc('M', 2, 2, 0x40);
	//putc('M', 3, 3, 0x40);
	//putc('M', 4, 4, 0x40);

	graphics_init(0x2000, 640, 480, 1);

	draw_rect(100, 100, 100, 100, (unsigned int*)0x2000);

	/*

	char letter = 'A';

	while(1)
	{
		for(int i = 0; i < 80; i++)
		{
			for(int t = 0; t < 25; t++)
			{
				putc(letter, i, t, 0x0F);
			}
		}
		letter++;
		if(letter == ('A' + 20)){letter = 'A';}
	}

	*/
}