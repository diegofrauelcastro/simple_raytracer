#include "window_application.h"

#include "camera.h"
#include "scene.h"

#define WIDTH 1024
#define HEIGHT 512

int main(int argc, char* argv[])
{
    // Create application.
    WindowApplication windowApp;
    windowApp.InitApplication("Simple Raytracer", WIDTH, HEIGHT);

    // Init mesh.
    Mesh triMesh;
    Entity triangle("Triangle", &triMesh, Maths::Vector3(0.f, 0.f, 5.f));
    Scene scene({ &triangle });

    // Create a camera and render an image with it.
    Camera mainCam;
    mainCam.RenderFrame(windowApp);

    // Present the image and wait for user to close the window.
    windowApp.Present();
    windowApp.WaitUntilWindowClose();

    // Quit the application after closure.
    windowApp.QuitApplication();

    return 0;
}
