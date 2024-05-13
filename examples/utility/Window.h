#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "Framebuffer.h"

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	
	bool running;
} Window;

Window* newWindow(size_t width, size_t height, char* title, bool fullscreen);
void freeWindow(Window* this);

void windowPollEvents(Window* this);
void windowPresent(Window* this, Framebuffer* fb);

