//Image data type
struct Image
{
    //Resolution
    int width;
    int height;
    int bpp;
    //Image Location
    unsigned int* Mem;
    //Image size
    unsigned int byteCount;
};


void graphics_putPixel(int x, int y, char r, char g, char b);
void graphics_rect(int x, int y, int width, int height, char r, char g, char b);
void graphics_cls(char r, char g, char b);
void graphics_present();
void graphic_init();
void graphics_blit_rect(int x, int y, int width, int height, unsigned int* mem);
void graphics_readImageFromDisk(struct Image* img, int diskSector, int sectorCount);
void graphics_blit_image(struct Image* img, int x, int y, int width, int height);