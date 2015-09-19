#ifndef SCROLLER_H
#define SCROLLER_H

#include "../textmode.h"

bool sdl_init();

void quit_sdl();

bool display_as_scroller(textmode_t& artwork);

#endif
