#ifndef ICE_DRAW_H
#define ICE_DRAW_H

#include "../textmode.h"

class ice_draw_t : public textmode_t
{
public:
    ice_draw_t(std::ifstream&);
};

#endif
