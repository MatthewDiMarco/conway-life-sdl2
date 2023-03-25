#include <iostream>
#include "game.hpp"
#include "util.hpp"

static int find_num_alive_neighbours(GameState *game_state, int row, int col)
{
    return (signnum(game_state->front_buffer[row][mod(col - 1, game_state->CELLS_HIGH)]) +
            signnum(game_state->front_buffer[row][mod(col + 1, game_state->CELLS_HIGH)]) +
            signnum(game_state->front_buffer[mod(row - 1, game_state->CELLS_WIDE)][col]) +
            signnum(game_state->front_buffer[mod(row + 1, game_state->CELLS_WIDE)][col]) +
            signnum(game_state->front_buffer[mod(row - 1, game_state->CELLS_WIDE)][mod(col - 1, game_state->CELLS_HIGH)]) +
            signnum(game_state->front_buffer[mod(row - 1, game_state->CELLS_WIDE)][mod(col + 1, game_state->CELLS_HIGH)]) +
            signnum(game_state->front_buffer[mod(row + 1, game_state->CELLS_WIDE)][mod(col - 1, game_state->CELLS_HIGH)]) +
            signnum(game_state->front_buffer[mod(row + 1, game_state->CELLS_WIDE)][mod(col + 1, game_state->CELLS_HIGH)]));
}

GameState *create_game_state(int width, int height, int unit_size)
{
    GameState *game_state = new GameState;
    game_state->CELLS_WIDE = (width / unit_size);
    game_state->CELLS_HIGH = (height / unit_size);

    game_state->front_buffer = new int *[game_state->CELLS_WIDE];
    game_state->back_buffer = new int *[game_state->CELLS_WIDE];
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        game_state->front_buffer[ii] = new int[game_state->CELLS_HIGH];
        game_state->back_buffer[ii] = new int[game_state->CELLS_HIGH];
        for (int jj = 0; jj < game_state->CELLS_HIGH; jj++)
        {
            game_state->front_buffer[ii][jj] = 0;
            game_state->back_buffer[ii][jj] = 0;
        }
    }

    return game_state;
}

void process_cell(GameState *game_state, int row, int col, int *population_size)
{
    int alive_neighbours = find_num_alive_neighbours(game_state, row, col);

    // cell alive
    if (game_state->front_buffer[row][col] >= 1)
    {
        if (alive_neighbours < 2 || alive_neighbours > 3)
        {
            game_state->back_buffer[row][col] = 0;
            (*population_size)--;
        }
        else
        {
            game_state->back_buffer[row][col] = std::min(game_state->back_buffer[row][col] + 1, 255);
        }
    }

    // cell dead
    else if (game_state->front_buffer[row][col] == 0)
    {
        if (alive_neighbours == 3)
        {
            game_state->back_buffer[row][col] = 1;
            (*population_size)++;
        }
        else
        {
            game_state->back_buffer[row][col] = 0;
        }
    }
}

void spawn_cell(GameState *game_state, int row, int col, int *population_size)
{
    row = mod(row, game_state->CELLS_WIDE);
    col = mod(col, game_state->CELLS_HIGH);
    if (game_state->front_buffer[row][col] < 1)
    {
        game_state->front_buffer[row][col] = 1;
        (*population_size)++;
    }
}

void kill_cell(GameState *game_state, int row, int col, int *population_size)
{
    row = mod(row, game_state->CELLS_WIDE);
    col = mod(col, game_state->CELLS_HIGH);
    if (game_state->front_buffer[row][col] != 0)
    {
        game_state->front_buffer[row][col] = 0;
        (*population_size)--;
    }
}

void swap_buffers(GameState *game_state)
{
    int **temp = game_state->front_buffer;
    game_state->front_buffer = game_state->back_buffer;
    game_state->back_buffer = temp;
}

void destroy_game_state(GameState *game_state)
{
    for (int ii = 0; ii < game_state->CELLS_WIDE; ii++)
    {
        delete[] game_state->back_buffer[ii];
        delete[] game_state->front_buffer[ii];
    }
    delete[] game_state->back_buffer;
    delete[] game_state->front_buffer;
    delete game_state;
}