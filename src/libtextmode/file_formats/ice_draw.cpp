#include "../textmode.h"
#include "ice_draw.h"
#include "screen.h"

inline void read(std::ifstream& ifs, uint8_t& data)
{
    ifs.read(reinterpret_cast<char*>(&data), 1);
    if(ifs.fail()) {
        ifs.clear();
        throw std::exception();
    }
}

inline void read(std::ifstream& ifs, rgb_t& rgb)
{
    read(ifs, rgb.red);
    read(ifs, rgb.green);
    read(ifs, rgb.blue);
}

template <typename T>
inline void read(std::ifstream& ifs, std::vector<T>& data_vector)
{
    for(auto& data:data_vector) {
        read(ifs, data);
    }
}

image_data_t read_ice_draw_file(std::ifstream& ifs, const size_t& file_size)
{
    screen_t screen(80);

    ifs.seekg(12, std::ios_base::cur);
    std::vector<uint8_t> font(4096);

    {
        uint8_t code, attr, iterations;
        for(size_t i = ifs.tellg(); i < file_size - (48 + 4096); i += 2) {
            read(ifs, code);
            read(ifs, attr);
            if(code == 1) {
                read(ifs, iterations);
                ifs.seekg(1, std::ios_base::cur);
                read(ifs, code);
                read(ifs, attr);
                screen.use_packed_attribute(attr);
                i += 4;
                for(size_t k = 0; k < iterations; ++k) {
                    screen.putc(code);
                }
            } else {
                screen.use_packed_attribute(attr);
                screen.putc(code);
            }
        }
    }

    image_data_t image_data = screen.get_image_data();

    read(ifs, font);

    for(size_t i = 0; i < 16; ++i) {
        rgb_t ega_value;
        read(ifs, ega_value);
        image_data.palette.push(ega_value);
    }

    return std::move(image_data);
}

ice_draw_t::ice_draw_t(std::ifstream& ifs)
    : textmode_t(ifs)
{
    image_data = read_ice_draw_file(ifs, sauce.file_size);
    options.non_blink = non_blink_t::on;
    options.font_type = font_type_t::custom;
    options.palette_type = palette_type_t::custom;
    type = textmode_type_t::ice_draw;
}
