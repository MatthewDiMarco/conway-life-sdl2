#include "game.hpp"

static int mod(int a, int base)
{
    return ((a % base) + base) % base;
}

GameState *create_game_state(int width, int height, int unit_size)
{
    GameState *game_state = new GameState;
    game_state->CELLS_WIDE = (width / unit_size);
    game_state->CELLS_HIGH = (height / unit_size);

    game_state->cell_buffer_1 = new int *[game_state->CELLS_WIDE];
    game_state->cell_buffer_2 = new int *[game_state->CELLS_WIDE];
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        game_state->cell_buffer_1[ii] = new int[game_state->CELLS_HIGH];
        game_state->cell_buffer_2[ii] = new int[game_state->CELLS_HIGH];
        for (int jj = 0; jj < game_state->CELLS_HIGH; jj++)
        {
            game_state->cell_buffer_1[ii][jj] = 0;
            game_state->cell_buffer_2[ii][jj] = 0;
        }
    }

    return game_state;
}

void destroy_game_state(GameState *game_state)
{
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        delete[] game_state->cell_buffer_1[ii];
        delete[] game_state->cell_buffer_2[ii];
    }
    delete[] game_state->cell_buffer_1;
    delete[] game_state->cell_buffer_2;
    delete game_state;
}