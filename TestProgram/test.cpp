#include "lib/derpOS.h"
#include "lib/graphics/window.h"

//extern "C" int Usermain();

void window_callback(uint32 type, uint32 param1, uint32 param2);

int Usermain()
{
	WindowInfo info;
	info.bbp = 24;
	info.width = 300;
	info.height = 300;
	info.windowName = 'T';

	//createWindow(window_callback, &info);

	while(1){}
}

void window_callback(uint32 type, uint32 param1, uint32 param2)
{

}
