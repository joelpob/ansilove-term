#ifndef SAUCE_H
#define SAUCE_H

#include <cstdlib>
#include <vector>
#include <fstream>
#include "options.h"
#include "file.h"

class sauce_t
{
public:
    sauce_t(const std::string&);
    options_t get_options();

    bool exists;
    std::string version, title, author, group, year, month, day;
    size_t file_size = 0;
    data_type_t data_type = data_type_t::undefined;
    file_type_t file_type = file_type_t::undefined;
    uint16_t columns = 0;
    uint16_t rows = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t colors = 0;
    uint16_t depth = 0;
    uint16_t sample_rate = 0;
    non_blink_t non_blink = non_blink_t::undefined;
    letter_space_t letter_space = letter_space_t::undefined;
    aspect_ratio_t aspect_ratio = aspect_ratio_t::undefined;
    std::string font_name;
    font_type_t font_type = font_type_t::undefined;
    std::vector<std::string> comments;
};

std::ostream& operator<<(std::ostream&, const sauce_t&);

#endif
