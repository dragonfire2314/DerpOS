#include "image.h"
#include "../screen.h"
#include "../../core/mmu.h"
#include "../../io/serial.h"

void image_view_blit(const Image* img, View_rect rect, FrameBuffer targetBuffer, int x, int y, int width, int height)
{
    unsigned char* mem2 = (unsigned char*)img->data;

    uint32 screen_width = targetBuffer.size.x;

    for(int p = 0; p < height; p++){

        uint32 bufferRow = (screen_width * 3 * (p + y)) + x * 3;
        uint32 imgRow    = (img->width * 3 * (p + rect.y)) + rect.x * 3; 

        for(int i = 0; i < width * 3; i += 3)
        {
            targetBuffer.frame[bufferRow + i]   =   mem2[imgRow + i    ];
            targetBuffer.frame[bufferRow + i + 1] = mem2[imgRow + i + 1];
            targetBuffer.frame[bufferRow + i + 2] = mem2[imgRow + i + 2];
        }
    }
}

// //Wisth and height are unued right now, just use 0 or NULL
// void image_blit(const Image* img, int x, int y, int width, int height)
// {
//     unsigned char* mem2 = (unsigned char*)img->data;
//     //Copy mem to scaling bank and scaling it to size
//     // unsigned char* scal = (unsigned char*)malloc_page_aligned(width*height*3);

//     // for(int yPixel = 0; yPixel < height; yPixel++)
//     // {
//     //     for(int xPixel = 0; xPixel < width; xPixel++)
//     //     {
//     //         unsigned int srcX = ((float)xPixel / (float)width) * img->width;
//     //         unsigned int srcY = ((float)yPixel / (float)height) * img->height;

//     //         //Plot pixel in new mem
//     //         scal[(yPixel*width*3)+xPixel*3] = mem2[(srcY*img->width*3)+srcX*3];
//     //         scal[(yPixel*width*3)+xPixel*3+1] = mem2[(srcY*img->width*3)+srcX*3+1];
//     //         scal[(yPixel*width*3)+xPixel*3+2] = mem2[(srcY*img->width*3)+srcX*3+2];
//     //     }
//     // }

//     for(int p = 0; p < height; p++){
//         //copy a row over
//         unsigned int iLim = width*3;
//         //Calculate indexes for the row (could be optimized in many ways)
//         int index = ((y+p)*vbe_width*3)+x*3;
//         unsigned int image_index = (p*width*3);
//         //Cull image if it goes off screen right
//         //Need to implement offscreen bottom culling
//         if(x*3 + width*3 >= vbe_width*3){ iLim = vbe_width*3 - x*3; }
//         //Plot pixels in bgr format
//         for(int i = 0; i < iLim; i+=3)
//         {
//             frame_double_buffer[index+i] = mem2[image_index+i];
//             frame_double_buffer[index+i+1] = mem2[image_index+i+1];
//             frame_double_buffer[index+i+2] = mem2[image_index+i+2];
//         }
//     }

//     //release memory
//     //free_page_aligned(scal);
// }