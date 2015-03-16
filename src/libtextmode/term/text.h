#ifndef TERMINAL_TEXT_H
#define TERMINAL_TEXT_H

#include <ostream>
#include "../textmode.h"

void display_as_text(std::ostream&, textmode_t&);
void display_as_ansi(std::ostream&, textmode_t&);
void display_as_xterm256(std::ostream&, textmode_t&);
void display_as_xterm24bit(std::ostream&, textmode_t&);

#endif
