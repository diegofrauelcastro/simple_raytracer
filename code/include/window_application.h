#pragma once

#include <vector>

// Forward declarations
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class WindowApplication
{
public:
	WindowApplication() = default;
	~WindowApplication() = default;

	/// PUBLIC METHODS ///

	void InitApplication(const char* _windowName, unsigned int _width, unsigned int _height);
	void Run();
	void QuitApplication();
	void SetPixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b);
	void Clear(uint8_t r, uint8_t g, uint8_t b);
	void Present();
	void Delay(uint32_t _ms);
	
	
	/// GETTERS ///

	bool GetIsRunning() const { return bIsRunning; }

private:
	SDL_Window* window = nullptr;				// SDL Window handle.
	SDL_Renderer* renderer = nullptr;			// SDL Renderer handle.
	SDL_Texture* texture = nullptr;				// SDL Screen texture handle.
	std::vector<uint32_t> pixels;				// Pixel buffer of the screen, to be written into the SDL texture.

	const char* name = nullptr;					// Window name.
	unsigned int width = 0;						// Window width.
	unsigned int height = 0;					// Windows height.
	bool bHasBeenInitialized = false;			// Flag to remember application state.
	bool bIsRunning = true;
};