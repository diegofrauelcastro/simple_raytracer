#pragma once

#include "vector3.h"

// Forward declarations
class WindowApplication;
class Ray;

class Camera
{
public:
	/// CAMERA PARAMETERS ///

	Maths::Vector3 position;
	Maths::Vector3 direction;
	float focalLength = 1.0f;
	float aspectRatio = -1.f;


	/// CONSTRUCTOR & DESTRUCTOR ///

	Camera(Maths::Vector3 _position = Maths::Vector3::zero, Maths::Vector3 _direction = Maths::Vector3::forward, float _focalLength = 1.0f);
	~Camera() = default;

	
	/// PUBLICLY CALLABLE METHODS ///

	void RenderFrame(WindowApplication& _dstWindow);

private:
	/// CAMERA PROPERTIES ///

	Maths::Vector3 firstPixelLocation;
	Maths::Vector3 uPixelDelta;
	Maths::Vector3 vPixelDelta;
	float verticalFOV = 60;
	float viewportWidth = 0;
	float viewportHeight = 0;


	/// CLASS HELPERS ///

	void ComputeViewportData(WindowApplication& _dstWindow);
};