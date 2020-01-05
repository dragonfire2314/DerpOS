class graphics
{
private:
    enum DerpOS_Updates
    {
        keyboard_down, keyboard_up,
        l_mouse_button_down, l_mouse_button_up,
        r_mouse_button_down, r_mouse_button_down
    };

    unsigned char* mem;
public:
    graphics();
    ~graphics();

    void* init(bool isFullScreen, int width, int height);

    enum DerpOS_Updates get_Update(int* data);

    //Draw Functions
    void Draw_rect(int x, int y, int width, int height);
};