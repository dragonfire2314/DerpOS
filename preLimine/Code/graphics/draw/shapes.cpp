#include "shapes.h"
#include "../screen.h"

void draw_rect (uint16 xPos, uint16 yPos, uint16 width, uint16 height, uint32 color)
{
    for(int i = 0; i < height; i++)
    {
        //Get offset for row
        int offset = (info.width * (yPos + i) * 3) + (xPos * 3);
        //Draw row
        for(int t = 0; t < width * 3; t+=3)
        {
            double_buffer.frame[offset + t    ] = (color & 0xFF); //B
            double_buffer.frame[offset + t + 1] = (color & 0xFF00) >> 8; //G
            double_buffer.frame[offset + t + 2] = (color & 0xFF0000) >> 16; //R
        }
    }
}