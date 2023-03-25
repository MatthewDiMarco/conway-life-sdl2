#include "render.hpp"

void render_set_color(SDL_Renderer *renderer, RenderColor color)
{
    SDL_SetRenderDrawColor(renderer, color.rr, color.gg, color.bb, color.aa);
}

void render_rectangle(SDL_Renderer *renderer, int xx, int yy, int ww, int hh, bool filled)
{
    SDL_Rect rectangle = {xx, yy, ww, hh};
    if (filled)
    {
        SDL_RenderFillRect(renderer, &rectangle);
    }
    else
    {
        SDL_RenderDrawRect(renderer, &rectangle);
    }
}