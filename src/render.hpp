#include <SDL2/SDL_render.h>

typedef struct
{
    int rr;
    int gg;
    int bb;
    int aa;
} RenderColor;

const RenderColor COLOR_GRAY_DARK = {25, 25, 25, SDL_ALPHA_OPAQUE};
const RenderColor COLOR_GRAY_LIGHT = {155, 155, 155, SDL_ALPHA_OPAQUE};
const RenderColor COLOR_WHITE = {245, 245, 245, SDL_ALPHA_OPAQUE};

void render_set_color(SDL_Renderer *renderer, RenderColor color);
void render_rectangle(SDL_Renderer *renderer, int xx, int yy, int ww, int hh, bool filled);