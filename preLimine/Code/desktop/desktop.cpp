#include "desktop.h"
#include "../graphics/screen.h"
#include "../graphics/draw/image.h"
#include "../io/serial.h"
#include "../global/constants.h"
#include "../global/error.h"

#include "../external/upng/upng.h"

//Functions
void desktop_Init();
void desktop_creatWindow();

//Creates windows for use by the desktop
struct Desktop {
    Window windows[MAX_WINDOWS];
    uint8 windowUsage[MAX_WINDOWS];
    uint16 windowCount = 0;
};

Desktop desktop;
Image fontSheet;


void start_desktop()
{
    biltBuffer();

    desktop_Init();

    desktop_creatWindow();

    Vector2 pos;
    pos.x = 0;
    pos.y = 0;
    window_draw_character(&desktop.windows[0], 'C', pos);

    render_window(0);
    biltBuffer();

    while(1);
}

void render_window(uint16 windowID) 
{
    //display a letter
    blitFrameToFrame(
        desktop.windows[windowID].frameBuffer, double_buffer,
        desktop.windows[windowID].location.x, desktop.windows[windowID].location.y,
        desktop.windows[windowID].size.x, desktop.windows[windowID].size.y
    );
}

void desktop_creatWindow()
{
    //Find the first empty window
    int available = -1;
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (desktop.windowUsage[i] == 0) {
            available = i;
            break;
        }
    }

    if (available == -1)
        Error_Bad("Trying to create a window when max windows are already created.");
    
    desktop.windows[available] = window_createWindow();
}

void desktop_Init()
{
    //Load font
    //Load the font table from the drive
    File_info info = readFile('C', "FONT.PNG");
    if (info.data == 0) serial_write_string("File data not returned \r\n");
	else serial_write_string("File Found and returned\r\n");

    //Convert the png to pixel data
    upng_t* upng;
    upng = upng_new_from_bytes(info.data, info.fileSize);
    if (upng != NULL) {
        upng_decode(upng);
        if (upng_get_error(upng) == UPNG_EOK) {
            
        }
        else {
            //Throw error, crash OS
            Error_Critical("Font sheet failed to load for desktop.");
        }
    }

    fontSheet.data = upng_get_buffer(upng);
    fontSheet.width = upng_get_width(upng);
    fontSheet.height = upng_get_height(upng);
    fontSheet.bpp = upng_get_bpp(upng);
    fontSheet.bitDepth = upng_get_bitdepth(upng);

    //Init the windows
    Init_Windows(&fontSheet);

    //Clear desktop vars
    for (int i = 0; i < MAX_WINDOWS; i++) 
    {
        desktop.windowUsage[i] = 0;
    }
}