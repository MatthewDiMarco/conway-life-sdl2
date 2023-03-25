typedef struct
{
    int **cell_buffer_1;
    int **cell_buffer_2;
    int CELLS_WIDE;
    int CELLS_HIGH;
} GameState;

/**
 * Creates a grid as per the provided arguments
 *
 * \param width width of the grid (e.g. the screen)
 * \param height height of the grid (e.g. the screen)
 * \param unit_size partitioning divisor for the width and height
 * \returns pointer to the GameState
 */
GameState *create_game_state(int width, int height, int unit_size);

/**
 * Frees all heap memory associated with a GameState struct.
 *
 * \param game_state pointer to the GameState
 */
void destroy_game_state(GameState *game_state);