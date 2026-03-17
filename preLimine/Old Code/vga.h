/*
*  VGA TODO LIST
*  -make vga_cls just clear the ram and not pixel by pixel
*/

/*

    VGA INSTRUCTIONS:
        -Call vgaStart(2);
        -Enter a loop(or just only update when nessary)
        -Call vga_cls();
        -Render things
        -Call vga_update_buffer();

    code:
        vgaStart(2);
        while(1)
        {
            vga_cls();
            //RENDERING HERE
            vga_update_buffer();
        }

*/

char vgaStart(char mode);

void vga_buffer_pixel(short x, short y, short col);
void vga_draw_line(short x1, short y1, short x2, short y2, char col);
void vga_draw_triangle(short x1, short y1, short x2, short y2, short x3, short y3, char col);
void vga_draw_circle(short xc, short yc, short r, short col);  //Untested Function
void vga_cls(char col);
void vga_set_color(char color, char red, char green, char blue);
void vga_draw_text(char *str, short x, short y, char front_col, char back_col);

void vga_update_buffer();
