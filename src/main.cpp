#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"

// Constants
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int UNIT_SIZE = 5;

// Globals
SDL_Window *main_window;
SDL_Renderer *main_renderer;
GameState *game_state;

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

    // TEMP dummy state
    game_state->cell_buffer_1[20][20] = 1;
    game_state->cell_buffer_2[20][20] = 1;
    game_state->cell_buffer_1[20][21] = 1;
    game_state->cell_buffer_2[20][21] = 1;
    game_state->cell_buffer_1[21][20] = 1;
    game_state->cell_buffer_2[21][20] = 1;
    game_state->cell_buffer_1[21][21] = 1;
    game_state->cell_buffer_2[21][21] = 1;
    // END TEMP

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
// Called every frame for rendering game state to the window
//
void draw()
{
    SDL_SetRenderDrawColor(main_renderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(main_renderer);

    SDL_SetRenderDrawColor(main_renderer, 245, 245, 245, SDL_ALPHA_OPAQUE);
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        for (int jj = 0; jj < game_state->CELLS_HIGH; jj++)
        {
            game_state->cell_buffer_1[ii][jj] = game_state->cell_buffer_2[ii][jj];

            if (game_state->cell_buffer_1[ii][jj] == 1)
            {
                SDL_Rect rectangle = {
                    (ii * UNIT_SIZE),
                    (jj * UNIT_SIZE),
                    UNIT_SIZE, UNIT_SIZE};
                SDL_RenderFillRect(main_renderer, &rectangle);
            }
        }
    }

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

    // Game loop start
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