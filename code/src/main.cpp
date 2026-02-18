#include "window_application.h"

#include "camera.h"
#include "scene.h"

#define WIDTH 512
#define HEIGHT 512

int main(int argc, char* argv[])
{
    // Create application.
    WindowApplication windowApp;
    windowApp.InitApplication("Simple Raytracer", WIDTH, HEIGHT, true);

	///////////

    // Init HARDCODED scene.
	Scene scene("TestScene");

    // Triangles (1 and 2)
    Mesh triMesh;
	Entity& triangle1 = scene.GetRoot().AddChild("Triangle1");
	triangle1.AddComponent<MeshRendererComponent>("MeshRenderer")->SetMesh(triMesh);
	triangle1.GetTransform().SetPosition(Maths::Vector3(0.f, 0.f, -5.f));

	Entity& triangle2 = scene.GetRoot().AddChild("Triangle2");
	triangle2.AddComponent<MeshRendererComponent>("MeshRenderer")->SetMesh(triMesh);
	triangle2.GetTransform().SetPosition(Maths::Vector3(-1.f, 0.f, -7.5f));

    // Ground
	Mesh groundMesh("GroundMesh", "Assets/Meshes/Ground/GroundCube.obj");
	Entity& ground = scene.GetRoot().AddChild("Ground");
	ground.AddComponent<MeshRendererComponent>("MeshRenderer")->SetMesh(groundMesh);
	ground.GetTransform().SetPosition(Maths::Vector3(0.f, -1.f, -5.f));

	// Icosphere
	Mesh icosphereMesh("IcosphereMesh", "Assets/Meshes/icosphere.obj");
	Entity& icosphere = scene.GetRoot().AddChild("Icosphere");
	icosphere.AddComponent<MeshRendererComponent>("MeshRenderer")->SetMesh(icosphereMesh);
	icosphere.GetTransform().SetPosition(Maths::Vector3(1.f, 0.f, -5.f));

	///////////

	// Start the scene (call "Start()" on all entities and components).
	scene.StartScene();
	scene.UpdateTransformsFromRoot();
	scene.UpdateFrameCachedComponents();

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
