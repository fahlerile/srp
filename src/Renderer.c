#include "Renderer.h"
#include "utils.h"

/**
 * @brief Construct a new `Renderer`
 *
 * @param width width of a window
 * @param height height of a window
 * @param flags SDL flags to pass to `SDL_CreateWindowAndRenderer`
 * @return Renderer* a pointer to constructed `Renderer` or NULL on error (call SDL_GetError)
 */
Renderer* newRenderer(const char* window_title, int width, int height, uint32_t flags)
{
    SDL_Window* window;
    if ((window = SDL_CreateWindow(
        window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, flags
    )) == NULL)
        return NULL;

    SDL_Surface* screen;
    SDL_Surface* pixels;
    screen = SDL_GetWindowSurface(window);
    pixels = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBX8888);
    if (screen == NULL || pixels == NULL)
    {
        SDL_DestroyWindow(window);
        return NULL;
    }

    Renderer* renderer = xmalloc(sizeof(Renderer));
    *renderer = (Renderer) {
        .window = window,
        .screen = screen,
        .pixels = pixels,
        .dimensions = (Vector2i) {width, height}
    };
    return renderer;
}

void freeRenderer(Renderer* this)
{
    SDL_FreeSurface(this->pixels);
    SDL_DestroyWindow(this->window);
    xfree(this);
}

void rendererDrawPixel(Renderer* this, Vector2i point, Color color)
{
    SDL_LockSurface(this->pixels);
    {
        uint32_t* row = (uint32_t*) ((char*) this->pixels->pixels + this->pixels->pitch * point.y);
        row[point.x] = ColorToUint32RGBA(color);
    }
    SDL_UnlockSurface(this->pixels);
}

// @brief Save current buffer state as BMP
// @return 0 on success, -1 on error
int rendererSaveBuffer(Renderer* this, const char* filename)
{
    if (SDL_SaveBMP(this->screen, filename) != 0)
        return -1;
    return 0;
}

void rendererSwapBuffer(Renderer* this)
{
    SDL_BlitSurface(this->pixels, NULL, this->screen, NULL);
    SDL_UpdateWindowSurface(this->window);
}

void rendererClearBuffer(Renderer* this, Color color)
{
    SDL_FillRect(this->pixels, NULL, ColorToUint32RGBA(color));
}

