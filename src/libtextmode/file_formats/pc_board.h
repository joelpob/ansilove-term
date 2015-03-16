#ifndef PC_BOARD_H
#define PC_BOARD_H

#include "../textmode.h"

class pc_board_t : public textmode_t
{
public:
    pc_board_t(const std::string&);
};

#endif
