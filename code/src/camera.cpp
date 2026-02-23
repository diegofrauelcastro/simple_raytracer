#include "camera.h"
#include "DebugLog/debug.h"
#include "window_application.h"
#include "ray.h"
#include "scene.h"

using namespace Maths;

#define DEG2RAD 0.01745329252f

Camera::Camera(const Vector3& _position, const Vector3& _direction, float _focalLength)
	: position(_position)
	, direction(_direction)
	, focalLength(_focalLength)
	, threadPool(std::thread::hardware_concurrency())
{
}

Camera::Camera(const Camera& _copy)
	: position(_copy.position)
	, direction(_copy.direction)
	, focalLength(_copy.focalLength)
	, threadPool(std::thread::hardware_concurrency())

	, firstPixelLocation(_copy.firstPixelLocation)
	, uPixelDelta(_copy.uPixelDelta)
	, vPixelDelta(_copy.vPixelDelta)
	, verticalFOV(_copy.verticalFOV)
	, aspectRatio(_copy.aspectRatio)
	, viewportWidth(_copy.viewportWidth)
	, viewportHeight(_copy.viewportHeight)
{
}

void PrintProgress(float progress) {
	const int barWidth = 50;

	std::cout << "\r[";
	int pos = (int)(barWidth * progress);
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "#";
		else std::cout << " ";
	}
	std::cout << "] " << int(progress * 100.0f) << "%";
	std::cout.flush();
}

void Camera::RenderFrame(WindowApplication& _dstWindow, const Scene& _scene)
{
	uint32_t w = _dstWindow.GetWidth();
	uint32_t h = _dstWindow.GetHeight();
	if (w == 0 || h == 0)
	{
		LOG_APP("/!\\ Warning : Tried to draw on a window which width or height is null. Skipping...")
		return;
	}

	// If ratio has never been calculated, or if it has changed since last frame, recalculate it.
	if (aspectRatio < 0.00001f || fabs(aspectRatio - _dstWindow.GetAspectRatio()) > 0.00001f)
		ComputeViewportData(_dstWindow);

	if (_dstWindow.GetDebugTelemetry())
		LOG_APP("TELEMETRY: Start launching rays.")

	int pixelCounter = 0;
	float totalPixels = (float)(w * h);

	// Render to window screen.
	_dstWindow.Clear(0, 0, 0);

	// Multithread tiles.
	isRenderingInProgress = true;
	uint32_t tileSize = 32;
	uint32_t tilesX = (w + tileSize - 1) / tileSize;
	uint16_t tilesY = (h + tileSize - 1) / tileSize;
	uint32_t tileCount = tilesX * tilesY;

	std::vector<bool> tileCompletionStatus(tileCount, false);
	int currentTileIndex = 0;
	for (uint32_t y = 0; y < h; y += tileSize)
	{
		for (uint32_t x = 0; x < w; x += tileSize)
		{
			threadPool.Submit([this, &tileCompletionStatus, currentTileIndex, x, y, tileSize, &w, &h, &_dstWindow, &_scene]()
			{
				tileCompletionStatus[currentTileIndex] = RenderTile(_dstWindow, _scene, x, y, tileSize, tileSize);
			});
			currentTileIndex++;
		}
	}
	// Wait for all tiles to be rendered while showing progress in the console.
	while (isRenderingInProgress)
	{
		int completedTiles = 0;
		for (bool status : tileCompletionStatus)
			if (status) completedTiles++;
		PrintProgress((float)completedTiles / (float)tileCount);
		if (completedTiles == tileCount)
			isRenderingInProgress = false;
	}
	LOG_CLEAN("");
	LOG_APP("TELEMETRY: Stop launching rays.")
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

bool Camera::RenderTile(WindowApplication& _dstWindow, const Scene& _scene, uint32_t xStart, uint32_t yStart, uint32_t tileWidth, uint32_t tileHeight) const
{
	for (uint32_t y = yStart; y < yStart + tileHeight && y < _dstWindow.GetHeight(); ++y)
	{
		for (uint32_t x = xStart; x < xStart + tileWidth && x < _dstWindow.GetWidth(); ++x)
		{
			// Get current pixel's position and direction.
			Vector3 pixelCenter = firstPixelLocation + (float)x * uPixelDelta + (float)y * vPixelDelta;
			Vector3 rayDirection = (pixelCenter - position).Normalize();

			// Multisampling the same pixel.
			int sampleCount = 15;
			Vector3 uncappedPixelColor = Vector3::zero;
			for (int i = 0; i < sampleCount; i++)
			{
				// Calculate jittered ray for this pixel.
				float uOffset = ((float)rand() / (float)RAND_MAX - 0.5f) * uPixelDelta.GetMagnitude();
				float vOffset = ((float)rand() / (float)RAND_MAX - 0.5f) * vPixelDelta.GetMagnitude();
				Vector3 jitteredPixelCenter = pixelCenter + uOffset * uPixelDelta.Normalize() + vOffset * vPixelDelta.Normalize();
				Vector3 jitteredRayDirection = (jitteredPixelCenter - position).Normalize();

				// Create a ray for this jittered pixel, launch and accumulate the color.
				Ray ray(jitteredPixelCenter, jitteredRayDirection);
				uncappedPixelColor += Ray::LaunchRay(ray, _scene, 5).ToFloatVector3();
			}
			// Average the accumulated color and write it to the window.
			uncappedPixelColor /= (float)sampleCount;
			_dstWindow.SetPixel(x, y, Color(uncappedPixelColor).ToByteVector3());
		}
	}

	return true;
}

