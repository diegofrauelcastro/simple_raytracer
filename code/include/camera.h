#pragma once

#include "vector3.h"

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

	Maths::Vector3 firstPixelLocation;
	Maths::Vector3 uPixelDelta;
	Maths::Vector3 vPixelDelta;
	float aspectRatio = -1.f;
	float verticalFOV = 60;
	float viewportWidth = 0;
	float viewportHeight = 0;


	/// CLASS HELPERS ///

	void ComputeViewportData(WindowApplication& _dstWindow);
};