//DerpOS graphics driver
//By. Tanner Kern

#include "graphics.h"
#include "vbe.h"
#include "mmu.h"
#include "testFeatures.h"

unsigned int* frame_double_buffer;

void graphic_init()
{
    //Create double buffer
    frame_double_buffer = k_malloc_large((1024*768*3));
}

//Memsets 48 bits (Does not work)
void graphics_memset(int startLoc, int data, int size)
{
    for(int i = 0; i < size; i++)
    {
        frame_double_buffer[startLoc+i*3] = data;
    }
}

void graphics_putPixel(int x, int y, char r, char g, char b)
{
    frame_double_buffer[(1024*y*3)+(x*3)  ] = r;
    frame_double_buffer[(1024*y*3)+(x*3)+1] = g;
    frame_double_buffer[(1024*y*3)+(x*3)+2] = b;
}

//Replace with 48bit memset for increased speed
void graphics_rect(int x, int y, int width, int height, char r, char g, char b)
{
    for(int i = 0; i < height; i++)
    {
        //Get offset for row
        int offset = (1024*(y+i)*3)+(x*3);
        //Draw row
        for(int t = 0; t < width*3; t+=3)
        {
            frame_double_buffer[offset+t] = r;
            frame_double_buffer[offset+t+1] = g;
            frame_double_buffer[offset+t+2] = b;
        }
    }
}

//Wisth and height are unued right now, just use 0 or NULL
void graphics_blit_image(struct Image* img, int x, int y, int width, int height)
{
    unsigned char* mem2 = (unsigned char*)img->Mem;
    //Copy mem to scaling bank and scaling it to size
    unsigned char* scal = (unsigned char*)k_malloc_large(width*height*3);

    for(int yPixel = 0; yPixel < height; yPixel++)
    {
        for(int xPixel = 0; xPixel < width; xPixel++)
        {
            unsigned int srcX = ((float)xPixel / (float)width) * img->width;
            unsigned int srcY = ((float)yPixel / (float)height) * img->height;

            //Plot pixel in new mem
            scal[(yPixel*width*3)+xPixel*3] = mem2[(srcY*img->width*3)+srcX*3];
            scal[(yPixel*width*3)+xPixel*3+1] = mem2[(srcY*img->width*3)+srcX*3+1];
            scal[(yPixel*width*3)+xPixel*3+2] = mem2[(srcY*img->width*3)+srcX*3+2];
        }
    }

    for(int p = 0; p < height; p++){
        //copy a row over
        unsigned int iLim = width*3;
        //Calculate indexes for the row (could be optimized in many ways)
        int index = ((y+p)*vbe_width*3)+x*3;
        unsigned int image_index = (p*width*3);
        //Cull image if it goes off screen right
        //Need to implement offscreen bottom culling
        if(x*3 + width*3 >= vbe_width*3){ iLim = vbe_width*3 - x*3; }
        //Plot pixels in bgr format
        for(int i = 0; i < iLim; i+=3)
        {
            frame_double_buffer[index+i] = scal[image_index+i];
            frame_double_buffer[index+i+1] = scal[image_index+i+1];
            frame_double_buffer[index+i+2] = scal[image_index+i+2];
        }
    }

    /*
    unsigned char* mem2 = (unsigned char*)img->Mem;
    for(int p = 0; p < img->height; p++){
        //copy a row over
        unsigned int iLim = img->width*3;
        //Calculate indexes for the row (could be optimized in many ways)
        int index = ((y+p)*vbe_width*3)+x*3;
        unsigned int image_index = (p*img->width*3);
        //Cull image if it goes off screen right
        //Need to implement offscreen bottom culling
        if(x*3 + img->width*3 >= vbe_width*3){ iLim = vbe_width*3 - x*3; }
        //Plot pixels in bgr format
        for(int i = 0; i < iLim; i+=3)
        {
            frame_double_buffer[index+i] = mem2[image_index+i];
            frame_double_buffer[index+i+1] = mem2[image_index+i+1];
            frame_double_buffer[index+i+2] = mem2[image_index+i+2];
        }
    }
    */
}

void graphics_cls(char r, char g, char b)
{
    graphics_rect(0, 0, 1024, 768, r, g, b);
}

void graphics_present()
{
    for(int i = 0; i < (1024*768*3); i++)
    {
        kenel_frameBuffer[i] = frame_double_buffer[i];
    }
}

void graphics_readImageFromDisk(struct Image* img, int diskSector, int sectorCount)
{
    asm("cli");
    //Load img from disk to ram
    unsigned int* data = read(diskSector);
    //for(int i = diskSector+1; i < diskSector+sectorCount; i++)
    //{
    //    readSector(i);
    //}
    //Load the stucture with data (Will be stored in disk properties at some point)
    img->bpp = 24;
    img->byteCount = sectorCount * 512;
    img->height = 10;
    img->width = 10;
    img->Mem = data;

    //asm("sti");
}