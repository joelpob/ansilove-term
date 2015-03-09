#ifndef ANSI_H
#define ANSI_H

#include "../textmode.h"

class ansi_t : public textmode_t
{
public:
    ansi_t(std::ifstream&);
};

#endif
