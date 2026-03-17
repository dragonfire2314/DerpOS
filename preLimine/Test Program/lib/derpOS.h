extern void putc(unsigned char character, unsigned char xPos, unsigned char yPos, unsigned char color);

extern void graphics_init(unsigned int mem, int width, int hieght, int process_id);

void draw_rect(int x, int y, int width, int height, unsigned int* mem);