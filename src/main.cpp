#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"
#include "render.hpp"

// Constants
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int UNIT_SIZE = 2;
const int STEP_INCREMENT = 25;
const int MIN_SPAWN_BOX_SIZE = 1;
const int MAX_SPAWN_BOX_SIZE = (WINDOW_HEIGHT / UNIT_SIZE) / 2;
const int MIN_STEP_LENGTH = 0;
const int MAX_STEP_LENGTH = 100;

// Global data
SDL_Window *main_window;
SDL_Renderer *main_renderer;
SDL_Event event;
GameState *game_state;
Uint64 delay;
Uint64 ticks;
int x_mouse, y_mouse;
int mouse_buttons_state;

// Global controls
bool paused = true;
bool quit = false;
bool fill_cells = true;
int step_length = 50;
int spawn_box_size = 3;

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
// Called every frame for handling input
//
void input()
{
    // Respond to mouse
    mouse_buttons_state = SDL_GetMouseState(&x_mouse, &y_mouse);
    if (mouse_buttons_state == 1 || mouse_buttons_state == 4)
    {
        int offset = floor(spawn_box_size / 2);
        int row_selected = x_mouse / UNIT_SIZE;
        int col_selected = y_mouse / UNIT_SIZE;
        for (int ii = row_selected; ii < row_selected + spawn_box_size; ii++)
        {
            for (int jj = col_selected; jj < col_selected + spawn_box_size; jj++)
            {
                int row = ii - offset;
                int col = jj - offset;
                if (mouse_buttons_state == 1)
                {
                    spawn_cell(game_state, row, col);
                }
                else if (mouse_buttons_state == 4)
                {
                    kill_cell(game_state, row, col);
                }
            }
        }
    }

    // Respond to events
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEWHEEL:
        {
            int signnum = (0 < event.wheel.y) - (event.wheel.y < 0); // [-1, 0, 1]
            if (signnum == 1)
            {
                spawn_box_size = std::clamp(spawn_box_size * 2,
                                            MIN_SPAWN_BOX_SIZE, MAX_SPAWN_BOX_SIZE);
            }
            else if (signnum == -1)
            {
                spawn_box_size = std::clamp(spawn_box_size / 2,
                                            MIN_SPAWN_BOX_SIZE, MAX_SPAWN_BOX_SIZE);
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
            }
            if (event.key.keysym.sym == SDLK_f)
            {
                fill_cells = !fill_cells;
            }
            if (event.key.keysym.sym == SDLK_UP)
            {
                step_length = std::clamp(step_length + STEP_INCREMENT,
                                         MIN_STEP_LENGTH, MAX_STEP_LENGTH);
            }
            if (event.key.keysym.sym == SDLK_DOWN)
            {
                step_length = std::clamp(step_length - STEP_INCREMENT,
                                         MIN_STEP_LENGTH, MAX_STEP_LENGTH);
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
    bool updating = !paused && ticks >= delay;
    if (updating)
    {
        delay = ticks + step_length;
    }

    // Background
    render_set_color(main_renderer, COLOR_GRAY_DARK);
    SDL_RenderClear(main_renderer);

    // Cells
    render_set_color(main_renderer, COLOR_WHITE);
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        for (int jj = 0; jj < game_state->CELLS_HIGH; jj++)
        {
            // Render cells
            if (game_state->front_buffer[ii][jj] == 1)
            {
                render_rectangle(main_renderer, ii * UNIT_SIZE, jj * UNIT_SIZE,
                                 UNIT_SIZE, UNIT_SIZE, fill_cells);
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

    // Spawn area
    render_set_color(main_renderer, COLOR_GRAY_LIGHT);
    render_rectangle(main_renderer,
                     ((x_mouse / UNIT_SIZE) * UNIT_SIZE) - ((spawn_box_size) / 2 * UNIT_SIZE),
                     ((y_mouse / UNIT_SIZE) * UNIT_SIZE) - ((spawn_box_size) / 2 * UNIT_SIZE),
                     (spawn_box_size)*UNIT_SIZE,
                     (spawn_box_size)*UNIT_SIZE, false);

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

    delay = step_length;
    while (!quit)
    {
        input();
        update();
    }

    shutdown();
    return EXIT_SUCCESS;
}