/**
 * Container for all related game state.
 */
typedef struct
{
    int **front_buffer;
    int **back_buffer;
    int CELLS_WIDE;
    int CELLS_HIGH;
} GameState;

/**
 * Creates a grid of cells, where each cell is size "unit_size".
 *
 * \param width width of the grid (e.g. the screen)
 * \param height height of the grid (e.g. the screen)
 * \param unit_size partitioning divisor for the width and height
 * \returns pointer to the GameState
 */
GameState *create_game_state(int width, int height, int unit_size);

/**
 * Applies the game rules to the given cell.
 *
 * \param game_state pointer to the GameState
 * \param row cell's x location
 * \param col cell's y location
 * \param population_size pointer to population tracker
 */
void process_cell(GameState *game_state, int row, int col, int *population_size);

/**
 * Create a cell at the specified location.
 *
 * The function applies a modulo wrapper to ensure the cells remain
 * within the buffer on a toroidal surface.
 *
 * \param game_state pointer to the GameState
 * \param row cell's x location
 * \param col cell's y location
 * \param population_size pointer to population tracker
 */
void spawn_cell(GameState *game_state, int row, int col, int *population_size);

/**
 * Kill a cell at the specified location.
 *
 * The function applies a modulo wrapper to ensure the cells remain
 * within the buffer on a toroidal surface.
 *
 * \param game_state pointer to the GameState
 * \param row cell's x location
 * \param col cell's y location
 * \param population_size pointer to population tracker
 */
void kill_cell(GameState *game_state, int row, int col, int *population_size);

/**
 * Brings the back buffer to the front for rendering; to be called once
 * cell processing is complete.
 *
 * The game state persists two identical matrix buffers: the back and front.
 * The front buffer represents the current source of truth, and is used for
 * rendering; the back buffer is reserved for updating state, so that the
 * currently rendered state remains unaffected.
 *
 * \param game_state pointer to the GameState
 */
void swap_buffers(GameState *game_state);

/**
 * Frees all heap memory associated with a GameState struct.
 *
 * \param game_state pointer to the GameState
 */
void destroy_game_state(GameState *game_state);