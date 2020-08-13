#include "desktop.h"
#include "../graphics/screen.h"

#include "../util/vector.h"

void desktop_loop();


void begin_desktop()
{
    biltBuffer();

    //Create Menu bar
        //File Button

        //Current application title
    //Create Task bar
        //Start button
        //Buttons the open windows that are running
    //Start the loop
    desktop_loop();
}

void desktop_loop()
{
    while(1)
    {
        presentScreen();
    }
}