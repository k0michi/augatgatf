#include <iostream>
#include <SDL3/SDL.h>

int main()
{
    auto result = SDL_Init(SDL_INIT_VIDEO);

    if (!result)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    auto window = SDL_CreateWindow("Hello SDL3", 640, 480, 0);

    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                break;
            }
        }
    }

    SDL_Quit();
    return 0;
}
