#pragma once

#include "vector3.h"
#include "threadpool.h"

// Forward declarations
class WindowApplication;
class Ray;
class Scene;

class Camera
{
public:
	/// CAMERA PARAMETERS ///

	Maths::Vector3 position;
	Maths::Vector3 direction;
	float focalLength = 1.0f;


	/// CONSTRUCTOR & DESTRUCTOR ///

	Camera(const Maths::Vector3& _position = Maths::Vector3::zero, const Maths::Vector3& _direction = Maths::Vector3::forward, float _focalLength = 1.0f);
	Camera(const Camera& _copy);
	~Camera() = default;

	
	/// PUBLICLY CALLABLE METHODS ///

	void RenderFrame(WindowApplication& _dstWindow, const Scene& _scene);

private:
	/// CAMERA PROPERTIES ///

	ThreadPool threadPool;
	Maths::Vector3 firstPixelLocation;
	Maths::Vector3 uPixelDelta;
	Maths::Vector3 vPixelDelta;
	float aspectRatio = -1.f;
	float verticalFOV = 60;
	float viewportWidth = 0;
	float viewportHeight = 0;
	bool isRenderingInProgress = false;


	/// CLASS HELPERS ///

	void ComputeViewportData(WindowApplication& _dstWindow);
	bool RenderTile(WindowApplication& _dstWindow, const Scene& _scene, uint32_t xStart, uint32_t yStart, uint32_t tileWidth, uint32_t tileHeight) const;
};