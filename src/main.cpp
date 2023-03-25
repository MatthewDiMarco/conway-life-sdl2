#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"

// Constants
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int UNIT_SIZE = 2;

// Demo globals
SDL_Window *main_window;
SDL_Renderer *main_renderer;
SDL_Event event;
GameState *game_state;
bool paused = true;
bool quit = false;
bool updating = false;
int step_length = 1;
int xMouse, yMouse;
Uint64 delay = step_length;
Uint64 ticks;

//
// Setup everything needed for the game loop
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

    // Handle game creation
    game_state = create_game_state(WINDOW_WIDTH, WINDOW_HEIGHT, UNIT_SIZE);

    return EXIT_SUCCESS;
}

//
// Called before quitting for cleanup
//
void shutdown()
{
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(main_renderer);
    SDL_Quit();
    destroy_game_state(game_state);
}

//
// Called every frame for handling input events
//
void input()
{
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
        {
            SDL_GetMouseState(&xMouse, &yMouse);
            int xMatrix = xMouse / UNIT_SIZE;
            int yMatrix = yMouse / UNIT_SIZE;
            if (xMatrix >= 0 && xMatrix < game_state->CELLS_WIDE &&
                yMatrix >= 0 && yMatrix < game_state->CELLS_HIGH)
            {
                for (int ii = xMatrix - 10; ii < xMatrix + 10; ii++)
                {
                    for (int jj = yMatrix - 10; jj < yMatrix + 10; jj++)
                    {
                        spawn_cell(game_state, ii, jj);
                    }
                }
            }
            break;
        }

        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                paused = !paused;
                if (paused)
                {
                    std::cout << "paused" << std::endl;
                }
                else
                {
                    std::cout << "resumed" << std::endl;
                }
            }
            break;
        }
        }
    }
}

//
// Called every frame for processing game state
//
void update()
{
    ticks = SDL_GetTicks64();
    updating = !paused && ticks >= delay;
    if (updating)
    {
        delay = ticks + step_length;
    }

    // Background
    SDL_SetRenderDrawColor(main_renderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(main_renderer);

    // Cells
    SDL_SetRenderDrawColor(main_renderer, 245, 245, 245, SDL_ALPHA_OPAQUE);
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        for (int jj = 0; jj < game_state->CELLS_HIGH; jj++)
        {
            // Render cells
            if (game_state->front_buffer[ii][jj] == 1)
            {
                SDL_Rect rectangle = {
                    (ii * UNIT_SIZE),
                    (jj * UNIT_SIZE),
                    UNIT_SIZE, UNIT_SIZE};
                SDL_RenderFillRect(main_renderer, &rectangle);
            }

            // Process cells
            if (updating)
            {
                process_cell(game_state, ii, jj);
            }
        }
    }

    // Bring the processed cells to the front for rendering on the next call
    if (updating)
    {
        swap_buffers(game_state);
    }

    // Commit render
    SDL_RenderPresent(main_renderer);
}

//
// Run demo
//
int main()
{
    if (initialise() != EXIT_SUCCESS)
    {
        shutdown();
    }

    while (!quit)
    {
        input();
        update();
    }

    shutdown();
    return EXIT_SUCCESS;
}