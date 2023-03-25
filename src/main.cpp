#include <iostream>
#include <SDL2/SDL.h>

// Constants
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Globals
SDL_Window *main_window;
SDL_Renderer *main_renderer;

//
// Initialise sdl2 and game state
//
int initialise()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL_Init failed with error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // Handle window creation
    main_window = SDL_CreateWindow(
        "Game of Life SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (main_window == nullptr)
    {
        std::cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Handle renderer creation
    main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_PRESENTVSYNC);

    return EXIT_SUCCESS;
}

//
// Peform cleanup
//
void shutdown()
{
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(main_renderer);
    SDL_Quit();
}

//
// Called every frame for rendering the game state to the window
//
void draw()
{
    SDL_SetRenderDrawColor(main_renderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(main_renderer);
    SDL_RenderPresent(main_renderer);
}

//
// Demo program
//
int main()
{
    if (initialise() != EXIT_SUCCESS)
    {
        shutdown();
    }

    // Main loop start
    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
                break;
            }
        }
        draw();
    }

    // Exit gracefully
    shutdown();
    return EXIT_SUCCESS;
}