#include "game.hpp"

static int mod(int a, int base)
{
    return ((a % base) + base) % base;
}

static int find_num_alive_neighbours(GameState *game_state, int row, int col)
{
    return (game_state->cell_buffer_1[row][mod(col - 1, game_state->CELLS_HIGH)] +
            game_state->cell_buffer_1[row][mod(col + 1, game_state->CELLS_HIGH)] +
            game_state->cell_buffer_1[mod(row - 1, game_state->CELLS_WIDE)][col] +
            game_state->cell_buffer_1[mod(row + 1, game_state->CELLS_WIDE)][col] +
            game_state->cell_buffer_1[mod(row - 1, game_state->CELLS_WIDE)][mod(col - 1, game_state->CELLS_HIGH)] +
            game_state->cell_buffer_1[mod(row - 1, game_state->CELLS_WIDE)][mod(col + 1, game_state->CELLS_HIGH)] +
            game_state->cell_buffer_1[mod(row + 1, game_state->CELLS_WIDE)][mod(col - 1, game_state->CELLS_HIGH)] +
            game_state->cell_buffer_1[mod(row + 1, game_state->CELLS_WIDE)][mod(col + 1, game_state->CELLS_HIGH)]);
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

void process_cell(GameState *game_state, int row, int col)
{
    int alive_neighbours = find_num_alive_neighbours(game_state, row, col);

    // cell alive
    if (game_state->cell_buffer_1[row][col] == 1 && (alive_neighbours < 2 || alive_neighbours > 3))
    {
        game_state->cell_buffer_2[row][col] = 0;
    }

    // cell dead
    else if (game_state->cell_buffer_1[row][col] == 0 && alive_neighbours == 3)
    {
        game_state->cell_buffer_2[row][col] = 1;
    }
}

void spawn_cell(GameState *game_state, int row, int col)
{
    row = mod(row, game_state->CELLS_WIDE);
    col = mod(col, game_state->CELLS_HIGH);
    game_state->cell_buffer_1[row][col] = 1;
    game_state->cell_buffer_2[row][col] = 1;
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