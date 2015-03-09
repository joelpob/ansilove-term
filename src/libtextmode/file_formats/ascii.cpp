#include "../textmode.h"
#include "ascii.h"
#include "screen.h"

inline void read(std::ifstream& ifs, uint8_t& data)
{
    ifs.read(reinterpret_cast<char*>(&data), 1);
    if(ifs.fail()) {
        ifs.clear();
        throw std::exception();
    }
}

image_data_t read_ascii_file(std::ifstream& ifs, const size_t& file_size)
{
    serial_screen_t screen;
    uint8_t code;
    for(size_t i = 0; i < file_size; ++i)
    {
        read(ifs, code);
        switch(code)
        {
        case '\n':
            screen.new_line();
            break;
        case '\r':
            break;
        default:
            screen.putc(code);
            break;
        }
    }

    return screen.get_image_data();
}

ascii_t::ascii_t(std::ifstream& ifs)
    : textmode_t(ifs)
{
    image_data = read_ascii_file(ifs, sauce.file_size);
    options.palette_type = palette_type_t::none;
    type = textmode_type_t::ascii;
}
