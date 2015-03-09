#ifndef ARTWORX_H
#define ARTWORX_H

#include "../textmode.h"

class artworx_t : public textmode_t
{
public:
    artworx_t(std::ifstream&);
};

#endif
