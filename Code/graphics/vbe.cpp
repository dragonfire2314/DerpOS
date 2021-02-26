//This code sets up vesa vbe2 graphics linear buffer
//By Tanner Kern

#include "vbe.h"
#include "../io/serial.h"

struct regs16_t{
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, eflags;
} __attribute__ ((packed));

extern "C" void int32(unsigned char intnum, regs16_t *regs);

struct vbe_info_structure {
   char signature[4];   // must be "VESA" to indicate valid VBE support
   short version;         // VBE version; high byte is major version, low byte is minor version
   int oem;         // segment:offset pointer to OEM
   int capabilities;      // bitfield that describes card capabilities
   int video_modes;      // segment:offset pointer to list of supported video modes
   char video_memory;      // amount of video memory in 64KB blocks
   char software_rev;      // software revision
   int vendor;         // segment:offset to card vendor string
   int product_name;      // segment:offset to card model name
   int product_rev;      // segment:offset pointer to product revision
   char reserved[222];      // reserved for future expansion
   char oem_data[256];      // OEM BIOSes store their strings in this area
} __attribute__ ((packed));

struct vbe_mode_info_structure {
   short attributes;      // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
   char window_a;         // deprecated
   char window_b;         // deprecated
   short granularity;      // deprecated; used while calculating bank numbers
   short window_size;
   short segment_a;
   short segment_b;
   int win_func_ptr;      // deprecated; used to switch banks from protected mode without returning to real mode
   short pitch;         // number of bytes per horizontal line
   short width;         // width in pixels
   short height;         // height in pixels
   char w_char;         // unused...
   char y_char;         // ...
   char planes;
   char bpp;         // bits per pixel in this mode
   char banks;         // deprecated; total number of banks in this mode
   char memory_model;
   char bank_size;      // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
   char image_pages;
   char reserved0;

   char red_mask;
   char red_position;
   char green_mask;
   char green_position;
   char blue_mask;
   char blue_position;
   char reserved_mask;
   char reserved_position;
   char direct_color_attributes;

   int framebuffer;      // physical address of the linear frame buffer; write here to draw to the screen
   int off_screen_mem_off;
   short off_screen_mem_size;   // size of memory in the framebuffer but not being displayed on the screen
   char reserved1[206];
} __attribute__ ((packed));

vbe_mode_info_structure *vbeModeInfo = (vbe_mode_info_structure*)0xEE00;
vbe_info_structure *vbeInfo = (vbe_info_structure*)0xF000;

unsigned int* frameBuffer;
unsigned char* kenel_frameBuffer;

int vbe_width;
int vbe_height;

Video_Info vbe_init()
{
    Video_Info info;

    vbeInfo->signature[0] = 'V';
    vbeInfo->signature[1] = 'E';
    vbeInfo->signature[2] = 'S';
    vbeInfo->signature[3] = 'A';

    regs16_t regs;
    regs.ax = 0x4f00;
    regs.di = 0xF000;
    int32(0x10, &regs);

    regs16_t regs2;
    regs2.ax = 0x4F01;
    regs2.cx = 0x112;
    regs2.di = 0xEE00;
    int32(0x10, &regs2);

    regs16_t regs3;
    regs3.ax = 0x4F02;
    regs3.bx = 0x4112;
    int32(0x10, &regs3);

    frameBuffer = (unsigned int*)vbeModeInfo->framebuffer;
    kenel_frameBuffer = (unsigned char*)frameBuffer - 0xc0000000;
    vbe_width = vbeModeInfo->width;
    vbe_height = vbeModeInfo->height;

    asm("cli");

    info.frameBuffer = (uint8*)vbeModeInfo->framebuffer;
    info.kenel_frameBuffer = (uint8*)frameBuffer - 0xc0000000;
    info.width =  vbeModeInfo->width;
    info.height = vbeModeInfo->height;
    info.bpp = vbeModeInfo->bpp;

    return info;
}