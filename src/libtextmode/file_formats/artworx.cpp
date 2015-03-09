#include "../textmode.h"
#include "artworx.h"
#include "screen.h"

uint8_t artworx_palette_order[] = {0, 1, 2, 3, 4, 5, 20, 7, 56, 57, 58, 59, 60, 61, 62, 63};

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

image_data_t read_artworx_file(std::ifstream& ifs, const size_t& file_size)
{
    screen_t screen(80);
    palette_t ega_palette;
    std::vector<uint8_t> font(4096);

    {
        uint8_t version;
        read(ifs, version);
        if(version != 1) {
            throw std::exception();
        }
    }

    {
        std::vector<rgb_t> full_palette(64);
        read(ifs, full_palette);
        for(size_t i = 0; i < sizeof(artworx_palette_order); ++i) {
            rgb_t& rgb = full_palette[artworx_palette_order[i]];
            ega_palette.push(rgb);
        }
    }

    read(ifs, font);

    {
        uint8_t code, attr;
        for(size_t i = ifs.tellg(); i < file_size; i += 2) {
            read(ifs, code);
            read(ifs, attr);
            screen.use_packed_attribute(attr);
            screen.putc(code);
        }
    }

    image_data_t image_data = screen.get_image_data();

    image_data.palette = std::move(ega_palette);

    return std::move(image_data);
}

artworx_t::artworx_t(std::ifstream& ifs)
    : textmode_t(ifs)
{
    image_data = read_artworx_file(ifs, sauce.file_size);

    options.non_blink = non_blink_t::on;
    options.font_type = font_type_t::custom;
    options.palette_type = palette_type_t::custom;
    type = textmode_type_t::artworx;
}
