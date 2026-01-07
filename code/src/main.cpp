#include "window_application.h"

#define WIDTH 512
#define HEIGHT 512

int main(int argc, char* argv[])
{
    // Create application.
    WindowApplication windowApp;
    windowApp.InitApplication("Simple Raytracer", WIDTH, HEIGHT);

    // Test write to screen and present.
    windowApp.Clear(0, 0, 0);
    for (int y = 0; y < WIDTH; ++y)
        for (int x = 0; x < HEIGHT; ++x)
            windowApp.SetPixel(x, y, x % 256, y % 256, 128);
    windowApp.Present();
    windowApp.Run();

    // Quit the application after closure.
    windowApp.QuitApplication();

    return 0;
}
