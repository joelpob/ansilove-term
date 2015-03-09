#ifndef SAUCE_H
#define SAUCE_H

#include <cstdlib>
#include <vector>
#include <fstream>
#include "options.h"

class sauce_t
{
public:
    sauce_t();
    bool load(std::ifstream&);
    options_t get_options();

    bool exists;
    std::string version, title, author, group, year, month, day;
    size_t file_size;
    data_type_t data_type;
    file_type_t file_type;
    uint16_t columns, rows, width, height, colors, depth, sample_rate;
    non_blink_t non_blink;
    letter_space_t letter_space;
    aspect_ratio_t aspect_ratio;
    std::string font_name;
    font_type_t font_type;
    std::vector<std::string> comments;
};

std::ostream& operator<<(std::ostream&, const sauce_t&);

#endif
