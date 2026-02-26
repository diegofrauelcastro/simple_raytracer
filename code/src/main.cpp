#include "window_application.h"

#include "camera.h"
#include "scene.h"

#define WIDTH 900
#define HEIGHT 512
#define SAMPLES_PER_PIXEL 200
#define MAX_RECURSION_DEPTH 100
#define ENABLE_MULTITHREADING true

// Only useful if multithreading is enabled.
#ifdef DEBUG
	#define ENABLE_MULTITHREADING false
	#define TILE_SIZE 8
#else
	#define ENABLE_MULTITHREADING true
	#define TILE_SIZE 32
#endif

int main(int argc, char* argv[])
{
    // Create application.
    WindowApplication windowApp;
    windowApp.InitApplication("Simple Raytracer", WIDTH, HEIGHT, true);

	///////////

	// Materials
	DiffuseMaterial defaultMat;
	DiffuseMaterial centerSphereMat; centerSphereMat.albedo = Color(0.f, 0.f, 0.8f);
	DiffuseMaterial groundMat; groundMat.albedo = Color(0.8f, 0.8f, 0.f);
	MetallicMaterial metallicMat; metallicMat.albedo = Color(0.9f, 0.9f, 0.9f);
	MetallicMaterial goldMat; goldMat.albedo = Color(0.9f, 0.9f, 0.f);

    // Init HARDCODED scene.
	Scene scene("TestScene");

	// Init light.
	LightComponent* l = scene.GetRoot().AddChild("Light").AddComponent<LightComponent>("Light");
	l->GetTransform().SetPosition(Maths::Vector3(-2.f, 3.2f, -4.f));
	l->SetColor(Color::white);

    // Triangles (1 and 2)
	Mesh triMesh;
	Entity& triangle1 = scene.GetRoot().AddChild("Triangle1");
	MeshRendererComponent* mt1 = triangle1.AddComponent<MeshRendererComponent>("MeshRenderer");
	mt1->SetMesh(triMesh);
	mt1->SetMaterial(defaultMat);
	triangle1.GetTransform().SetPosition(Maths::Vector3(0.f, 0.5f, -2.f));
	triangle1.GetTransform().SetRotation(-30, 0.f, 0.f);

	/*Entity& triangle2 = scene.GetRoot().AddChild("Triangle2");
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
	ground.GetTransform().SetPosition(Maths::Vector3(0.f, -1.f, 0.f));

	// Ceiling
	//Entity& ceiling = scene.GetRoot().AddChild("Ceiling");
	//m1 = ceiling.AddComponent<MeshRendererComponent>("MeshRenderer");
	//m1->SetMesh(groundMesh);
	//m1->SetMaterial(groundMat);
	//ceiling.GetTransform().SetPosition(Maths::Vector3(0.f, 3.5f, -5.f));

	// Back wall
	Entity& backWall = scene.GetRoot().AddChild("BackWall");
	m1 = backWall.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(groundMesh);
	m1->SetMaterial(metallicMat);
	backWall.GetTransform().SetPosition(Maths::Vector3(0.f, -3.f, -5.f));
	backWall.GetTransform().SetRotation(90.f, 0.f, 0.f);

	// Right wall
	Entity& rightWall = scene.GetRoot().AddChild("RightWall");
	m1 = rightWall.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(groundMesh);
	m1->SetMaterial(metallicMat);
	rightWall.GetTransform().SetPosition(Maths::Vector3(2.5f, -3.f, 0.f));
	rightWall.GetTransform().SetRotation(90.f, 90.f, 0.f);

	// Left wall
	Entity& leftWall = scene.GetRoot().AddChild("LeftWall");
	m1 = leftWall.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(groundMesh);
	m1->SetMaterial(metallicMat);
	leftWall.GetTransform().SetPosition(Maths::Vector3(-2.5f, -3.f, 0.f));
	leftWall.GetTransform().SetRotation(90.f, 90.f, 0.f);

	// Front wall
	Entity& frontWall = scene.GetRoot().AddChild("FrontWall");
	m1 = frontWall.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(groundMesh);
	m1->SetMaterial(metallicMat);
	frontWall.GetTransform().SetPosition(Maths::Vector3(0.f, -3.f, 2.5f));
	frontWall.GetTransform().SetRotation(90.f, 0.f, 0.f);

	// Pillars
	Mesh pillarMesh("PillarCube", "Assets/Meshes/Ground/cube.obj");

	// Back left pillar
	Entity& backLeft = scene.GetRoot().AddChild("BackLeftPillar");
	m1 = backLeft.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(pillarMesh);
	m1->SetMaterial(groundMat);
	backLeft.GetTransform().SetPosition(Maths::Vector3(-2.5f, 0.f, -5.f));
	backLeft.GetTransform().SetScale(1.f, 5.f, 1.f);

	// Back right pillar
	Entity& backRight = scene.GetRoot().AddChild("BackRightPillar");
	m1 = backRight.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(pillarMesh);
	m1->SetMaterial(groundMat);
	backRight.GetTransform().SetPosition(Maths::Vector3(2.5f, 0.f, -5.f));
	backRight.GetTransform().SetScale(1.f, 5.f, 1.f);

	// Front left pillar
	Entity& frontLeft = scene.GetRoot().AddChild("FrontLeftPillar");
	m1 = frontLeft.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(pillarMesh);
	m1->SetMaterial(groundMat);
	frontLeft.GetTransform().SetPosition(Maths::Vector3(-2.5f, 0.f, 5.f));
	frontLeft.GetTransform().SetScale(1.f, 5.f, 1.f);

	// Front right pillar
	Entity& frontRight = scene.GetRoot().AddChild("FrontRightPillar");
	m1 = frontRight.AddComponent<MeshRendererComponent>("MeshRenderer");
	m1->SetMesh(pillarMesh);
	m1->SetMaterial(groundMat);
	frontRight.GetTransform().SetPosition(Maths::Vector3(2.5f, 0.f, 5.f));
	frontRight.GetTransform().SetScale(1.f, 5.f, 1.f);

	// Icosphere
	Mesh icosphereMesh("IcosphereMesh", "Assets/Meshes/icosphere.obj");

	Entity& icosphere = scene.GetRoot().AddChild("Icosphere");
	MeshRendererComponent* m = icosphere.AddComponent<MeshRendererComponent>("MeshRenderer");
	m->SetMesh(icosphereMesh);
	m->SetMaterial(goldMat);
	icosphere.GetTransform().SetPosition(Maths::Vector3(0.f, -0.5f, 0.f));
	icosphere.GetTransform().SetScale(Maths::Vector3(0.5f, 0.5f, 0.5f));

	//Entity& icosphere2 = scene.GetRoot().AddChild("Icosphere2");
	//m = icosphere2.AddComponent<MeshRendererComponent>("MeshRenderer");
	//m->SetMesh(icosphereMesh);
	//m->SetMaterial(defaultMat);
	//icosphere2.GetTransform().SetPosition(Maths::Vector3(-0.75f, -0.6f, 0.4f));
	//icosphere2.GetTransform().SetScale(Maths::Vector3(0.3f, 0.3f, 0.3f));

	Entity& icosphere3 = scene.GetRoot().AddChild("Icosphere3");
	m = icosphere3.AddComponent<MeshRendererComponent>("MeshRenderer");
	m->SetMesh(icosphereMesh);
	m->SetMaterial(centerSphereMat);
	icosphere3.GetTransform().SetPosition(Maths::Vector3(0.75f, -0.6f, 0.4f));
	icosphere3.GetTransform().SetScale(Maths::Vector3(0.3f, 0.3f, 0.3f));

	// Triangulated complex mesh
	Mesh complexMesh("ComplexMesh", "Assets/Meshes/merwann_prime.obj");
	Entity& complexEntity = scene.GetRoot().AddChild("Icosphere");
	m = complexEntity.AddComponent<MeshRendererComponent>("MeshRenderer");
	m->SetMesh(complexMesh);
	m->SetMaterial(defaultMat);
	complexEntity.GetTransform().SetPosition(Maths::Vector3(-0.75f, -0.6f, 0.4f));

	///////////

	// Start the scene (call "Start()" on all entities and components).
	scene.StartScene();
	scene.UpdateTransformsFromRoot();
	scene.UpdateFrameCachedComponents();

    // Create a camera and render an image with it.
    Camera mainCam;
	mainCam.position = Maths::Vector3(0.f, 0.f, 3.f);
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
