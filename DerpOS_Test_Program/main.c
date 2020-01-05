extern void pix();

char *t = (char*)0x00;

void pmain()
{
    t[0] = 'e';
    t[1] = 0x0F;
    t[2] = 10;
    t[3] = 10;

    pix();
    //return;
    while(1){}
}