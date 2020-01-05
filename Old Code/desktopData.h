enum WindowRenderType {
    LINEAR_BUFFER,
    WINDOW_BUILDER,
    SOFTWARE_3D
};

enum WindowScalingTpye{
    NEAREST_NEIGHBOOR,
    BILINEAR,
};

enum WindowStretchType{
    NO_STRETCH,
    STRETCH,
};

struct Window
{
    int posX;
    int posY;
    int width;
    int height;

    unsigned int* windowMemLoc;

    char RenderType;
    char ScalingTpye;
    char StretchType;

    bool isVisiable;
    bool isFocused;

    bool isUpdate;
};

struct Desktop
{
    bool Update;
};