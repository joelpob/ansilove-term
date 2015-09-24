#ifndef SCROLLER_H
#define SCROLLER_H

#include <SDL2/SDL.h>
#include "../textmode.h"

void scroller_init(SDL_Renderer*, const size_t& width, const size_t& height);

void scroller_quit();

bool display_as_scroller(SDL_Window*, const size_t& width, const size_t& height, SDL_Renderer*, textmode_t&, const bool& continuous);

#endif
