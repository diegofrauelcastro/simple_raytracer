#include "window_application.h"

#include "DebugLog/debug.h"
#include "vector3.h"
#include <SDL3/SDL.h>
#include <iostream>

void WindowApplication::InitApplication(const char* _windowName, unsigned int _width, unsigned int _height)
{
    LOG_CLEAN("\n\n===== INITIALIZATION =====\n")

    // Initialize SDL.
    LOG_APP("Initializing SDL...")
    if (!SDL_Init(SDL_INIT_VIDEO))
        LOG_THROW("/!\\ Couldn't initialize SDL! : ", SDL_GetError())

    // Create window and renderer and ensure the creation was successful.
    LOG_APP("Creating window, renderer and screen texture...")
    SDL_CreateWindowAndRenderer(_windowName, _width, _height, SDL_WINDOW_RESIZABLE, &window, &renderer);
    if (!window || !renderer)
    {
        SDL_Quit();
        LOG_THROW("/!\\ Couldn't create window and renderer! : ", SDL_GetError())
    }
    else
        LOG_APP("Window and renderer created.")

    // Create texture and ensure the creation was successful.
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, _width, _height);
    if (!texture)
        LOG_THROW("/!\\ Couldn't create texture! : ", SDL_GetError())
    else
        LOG_APP("Screen texture created.")

    // Mark this application as initialized.
    name = _windowName;
    bHasBeenInitialized = true;
    width = _width;
    height = _height;
    // Resize pixel buffer.
    pixels.resize(_width * _height);
    LOG_CLEAN("")
    LOG_APP("Initialization complete.")
}

void WindowApplication::WaitUntilWindowClose()
{
    LOG_CLEAN("\n\n===== APPLICATION RUN =====\n")
    LOG_APP("App currently running...")

    SDL_Event event;
    while (bIsRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                bIsRunning = false;
                LOG_APP("User requested window close...")
            }
        }
    }
}

void WindowApplication::SetPixel(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b)
{
    // Ensure the requested pixel exists within the bounds of the buffer.
    if (x >= width || y >= height)
    {
        LOG_APP("/!\\ Warning : Tried to access pixel out of bounds. Skipping...")
        return;
    }

    // Append all rgb values into one 32 bit uint for the background fill, with alpha set 255.
    uint32_t color = (0xFF << 24) | (r << 16) | (g << 8) | b;

    // Set the color.
    pixels[y * width + x] = color;
}

void WindowApplication::SetPixel(unsigned int x, unsigned int y, Maths::Vector3& _rgb)
{
    SetPixel(x, y, (uint8_t)_rgb.x, (uint8_t)_rgb.y, (uint8_t)_rgb.z);
}

void WindowApplication::Clear(uint8_t r, uint8_t g, uint8_t b)
{
    // Append all rgb values into one 32 bit uint for the background fill, with alpha set 255.
    uint32_t color = (0xFF << 24) | (r << 16) | (g << 8) | b;

    // Fill the pixel buffer with that single color.
    std::fill(pixels.begin(), pixels.end(), color);
}

void WindowApplication::Present()
{
    if (!window || !texture || !renderer)
        LOG_THROW("/!\\ Cannot present! Missing window, texture or renderer.")

    // Update texture with the pixel buffer.
    SDL_UpdateTexture(texture, nullptr, pixels.data(), width * sizeof(uint32_t));
    // Render the texture.
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void WindowApplication::Delay(uint32_t _ms)
{
    LOG_CLEAN("")
    LOG_APP("User requested a %dms delay.", _ms)
    SDL_Delay(_ms);
}

void WindowApplication::QuitApplication()
{
    // Do not attempt quitting if the application has never been marked as initialized.
    if (!bHasBeenInitialized) return;

    LOG_CLEAN("\n\n===== TERMINATION =====\n")

    LOG_APP("Closing window and destroying renderer...")
    // Destroy texture, renderer and window.
    if (texture)  SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);    
    texture = nullptr;
    renderer = nullptr;
    window = nullptr;

    // Clear pixel buffer.
    pixels.clear();
    pixels.shrink_to_fit();

    // Quit SDL.
    SDL_Quit();

    LOG_CLEAN("")
    LOG_APP("Termination complete.\n")

    // Log destruction.
    Debug::Log::GetInstance()->Destroy();
}
