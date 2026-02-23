#include "window_application.h"

#include "camera.h"
#include "scene.h"

#define WIDTH 620
#define HEIGHT 512
#define SAMPLES_PER_PIXEL 10
#define MAX_RECURSION_DEPTH 5
#define ENABLE_MULTITHREADING true

#ifdef DEBUG
	#define TILE_SIZE 8
#else
	#define TILE_SIZE 32 // Only useful if multithreading is enabled.
#endif

int main(int argc, char* argv[])
{
    // Create application.
    WindowApplication windowApp;
    windowApp.InitApplication("Simple Raytracer", WIDTH, HEIGHT, true);

	///////////

	// Materials
	DiffuseMaterial defaultMat;
	DiffuseMaterial centerSphereMat; centerSphereMat.albedo = Color(0.1f, 0.2f, 0.5f);
	DiffuseMaterial groundMat; groundMat.albedo = Color(0.8f, 0.8f, 0.f);
	MetallicMaterial metallicMat;

    // Init HARDCODED scene.
	Scene scene("TestScene");

    /*// Triangles (1 and 2)
	Mesh triMesh;
	Entity& triangle1 = scene.GetRoot().AddChild("Triangle1");
	MeshRendererComponent* mt1 = triangle1.AddComponent<MeshRendererComponent>("MeshRenderer");
	mt1->SetMesh(triMesh);
	mt1->SetMaterial(defaultMat);
	triangle1.GetTransform().SetPosition(Maths::Vector3(0.f, -0.2f, -2.5f));
	triangle1.GetTransform().SetRotation(-30, 0.f, 0.f);

	Entity& triangle2 = scene.GetRoot().AddChild("Triangle2");
	MeshRendererComponent* mt2 = triangle2.AddComponent<MeshRendererComponent>("MeshRenderer");
	mt2->SetMesh(triMesh);
	mt2->SetMaterial(defaultMat);
	triangle2.GetTransform().SetPosition(Maths::Vector3(-1.f, 0.f, -5.f));*/

    // Ground
	Mesh groundMesh("GroundMesh", "Assets/Meshes/Ground/GroundCube.obj");
	Entity& ground = scene.GetRoot().AddChild("Ground");
	MeshRendererComponent* m1 = ground.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(groundMesh);
	m1->SetMaterial(groundMat);
	ground.GetTransform().SetPosition(Maths::Vector3(0.f, -1.f, -5.f));

	// Back wall
	Entity& backWall = scene.GetRoot().AddChild("BackWall");
	m1 = backWall.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(groundMesh);
	m1->SetMaterial(defaultMat);
	backWall.GetTransform().SetPosition(Maths::Vector3(0.f, 0.f, -10.f));
	backWall.GetTransform().SetRotation(90.f, 0.f, 0.f);

	// Icosphere
	Mesh icosphereMesh("IcosphereMesh", "Assets/Meshes/icosphere.obj");

	//Entity& icosphere = scene.GetRoot().AddChild("Icosphere");
	//MeshRendererComponent* m = icosphere.AddComponent<MeshRendererComponent>("MeshRenderer");
	//m->SetMesh(icosphereMesh);
	//m->SetMaterial(defaultMat);
	//icosphere.GetTransform().SetPosition(Maths::Vector3(-1.f, 0.2f, -2.f));
	//icosphere.GetTransform().SetScale(Maths::Vector3(0.5f, 0.5f, 0.5f));

	Entity& icosphere2 = scene.GetRoot().AddChild("Icosphere2");
	MeshRendererComponent* m = icosphere2.AddComponent<MeshRendererComponent>("MeshRenderer");
	m->SetMesh(icosphereMesh);
	m->SetMaterial(centerSphereMat);
	icosphere2.GetTransform().SetPosition(Maths::Vector3(-2.f, 0.2f, -5.f));
	icosphere2.GetTransform().SetScale(Maths::Vector3(0.5f, 0.5f, 0.5f));

	//Entity& icosphere3 = scene.GetRoot().AddChild("Icosphere3");
	//m = icosphere3.AddComponent<MeshRendererComponent>("MeshRenderer");
	//m->SetMesh(icosphereMesh);
	//m->SetMaterial(defaultMat);
	//icosphere3.GetTransform().SetPosition(Maths::Vector3(1.f, 0.2f, -2.0f));
	//icosphere3.GetTransform().SetScale(Maths::Vector3(0.5f, 0.5f, 0.5f));


	///////////

	// Start the scene (call "Start()" on all entities and components).
	scene.StartScene();
	scene.UpdateTransformsFromRoot();
	scene.UpdateFrameCachedComponents();

    // Create a camera and render an image with it.
    Camera mainCam;
	mainCam.samplesPerPixel = SAMPLES_PER_PIXEL;
	mainCam.maxRecursionDepth = MAX_RECURSION_DEPTH;
	mainCam.enableMultithreading = ENABLE_MULTITHREADING;
	mainCam.tileSize = TILE_SIZE;
    mainCam.RenderFrame(windowApp, scene);

    // Present the image and wait for user to close the window.
    windowApp.Present();
    windowApp.WaitUntilWindowClose();

    // Quit the application after closure.
    windowApp.QuitApplication();

    return 0;
}
