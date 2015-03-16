#ifndef ASCII_H
#define ASCII_H

#include "../textmode.h"

class ascii_t : public textmode_t
{
public:
    ascii_t(const std::string&);
};

#endif
