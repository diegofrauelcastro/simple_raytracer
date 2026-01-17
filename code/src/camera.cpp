#include "camera.h"
#include "DebugLog/debug.h"
#include "window_application.h"
#include "ray.h"

using namespace Maths;

#define DEG2RAD 0.01745329252f

Camera::Camera(Maths::Vector3 _position, Maths::Vector3 _direction, float _focalLength)
	: position(_position)
	, direction(_direction)
	, focalLength(_focalLength)
{
}

void Camera::RenderFrame(WindowApplication& _dstWindow)
{
	uint32_t w = _dstWindow.GetWidth();
	uint32_t h = _dstWindow.GetHeight();
	if (h == 0)
	{
		LOG_APP("/!\\ Warning : Tried to draw on a window which height is null. Skipping...")
		return;
	}

	// If ratio has never been calculated, or if it has changed since last frame, recalculate it.
	if (aspectRatio < 0.00001f || fabs(aspectRatio - _dstWindow.GetAspectRatio()) > 0.00001f)
		ComputeViewportData(_dstWindow);

	// Render to window screen.
	_dstWindow.Clear(0, 0, 0);
	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++x)
		{
			// Get current pixel's position and direction.
			Vector3 pixelCenter = firstPixelLocation + x * uPixelDelta + y * vPixelDelta;
			Vector3 rayDirection = (pixelCenter - position).Normalize();

			// Create a ray for this pixel.
			Ray ray(position, rayDirection);

			// Launch the ray and determine its color.
			Vector3 pixelColor = Ray::LaunchRay(ray);
			_dstWindow.SetPixel(x, y, pixelColor);
		}
	}
}

void Camera::ComputeViewportData(WindowApplication& _dstWindow)
{
	LOG_CLEAN("")
	LOG_APP("Obsolete viewport data! Recalculating...")

	// Record window's aspect ratio.
	aspectRatio = _dstWindow.GetAspectRatio();

	// Viewport dimension values.
	viewportHeight = 2.0f * focalLength * tanf(verticalFOV * DEG2RAD * 0.5f);
	viewportWidth = viewportHeight * aspectRatio;
	Vector3 viewportUvector = Vector3(viewportWidth, 0.f, 0.f);
	Vector3 viewportVvector = Vector3(0.f, -viewportHeight, 0.f);
	Vector3 viewportUpperLeft = position - Vector3(0.f, 0.f, focalLength) - viewportUvector * 0.5f - viewportVvector * 0.5f;

	// Pixel delta values.
	uPixelDelta = viewportUvector / (float)_dstWindow.GetWidth();
	vPixelDelta = viewportVvector / (float)_dstWindow.GetHeight();

	// Record world position of the upper left pixel (first one in the array).
	firstPixelLocation = viewportUpperLeft + 0.5f * (uPixelDelta + vPixelDelta);
}

