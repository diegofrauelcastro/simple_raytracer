#include "window_application.h"

#include "camera.h"
#include "scene.h"

#define WIDTH 1600
#define HEIGHT 900

int main(int argc, char* argv[])
{
    // Create application.
    WindowApplication windowApp;
    windowApp.InitApplication("Simple Raytracer", WIDTH, HEIGHT, true);

    // Init mesh.
    Mesh triMesh;
    Entity triangle1("Triangle1", &triMesh, Maths::Vector3(0.f, 0.f, -5.f));
    Entity triangle2("Triangle2", &triMesh, Maths::Vector3(-1.f, 0.f, -7.5f));
    Mesh groundMesh("GroundMesh", "Assets/Meshes/Ground/GroundCube.obj");
    Entity ground("Ground", &groundMesh, Maths::Vector3(0.f, -1.f, -5.f));
    Scene scene({ &ground, &triangle1, &triangle2 });

    // Create a camera and render an image with it.
    Camera mainCam;
    mainCam.RenderFrame(windowApp, scene);

    // Present the image and wait for user to close the window.
    windowApp.Present();
    windowApp.WaitUntilWindowClose();

    // Quit the application after closure.
    windowApp.QuitApplication();

    return 0;
}
