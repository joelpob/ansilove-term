#ifndef TEXTMODE_H
#define TEXTMODE_H

#include "sauce.h"
#include "image_data.h"
#include "file.h"

class file_format_not_recognized_t : public std::exception {};

enum class textmode_type_t
{
    ansi,
    ansiedit,
    artworx,
    ascii,
    binary_text,
    ice_draw,
    pc_board,
    tundra_draw,
    xbin,
    undefined
};

class textmode_t
{
public:
    textmode_t(const std::string& filename);

    sauce_t sauce;
    options_t options;
    image_data_t image_data;
    textmode_type_t type;

    std::string title, author, group;
};

textmode_t load_artwork(std::string);

std::ostream& operator<<(std::ostream&, textmode_t&);

#endif
