#include <SDL3/SDL.h>
#include <iostream>
#include "vector2.h"

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL could not initialize! SDL_Error: |%s|\n", SDL_GetError());
        return 1;
    }
    Maths::Vector2 vec = Maths::Vector2::zero;
    std::cout << vec  <<std::endl;

    SDL_Window* window = SDL_CreateWindow(
        "SDL3 Test 2 seconds",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Delay(2000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
