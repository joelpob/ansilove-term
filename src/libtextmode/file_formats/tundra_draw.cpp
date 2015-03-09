#include "../textmode.h"
#include "tundra_draw.h"
#include "screen.h"
#include "palettes.h"

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

inline std::string get_string(std::ifstream& ifs, const size_t& length)
{
    char bytes[length];
    ifs.read(bytes, length);
    if(ifs.fail()) {
        ifs.clear();
        throw std::exception();
    }
    return std::string{bytes, length};
}

uint32_t read_tundra_32_bit_number(std::ifstream& ifs)
{
    uint8_t bytes[4];
    read(ifs, bytes[0]);
    read(ifs, bytes[1]);
    read(ifs, bytes[2]);
    read(ifs, bytes[3]);
    return (uint32_t(bytes[0]) << 24) + (uint32_t(bytes[1]) << 16) + (uint32_t(bytes[2]) << 8) + uint32_t(bytes[3]);
}

image_data_t read_tundra_draw_file(std::ifstream& ifs, const size_t& file_size, const size_t& columns)
{
    screen_t screen(columns == 0 ? 80 : columns);

    {
        uint8_t magic_number;
        std::string id;

        read(ifs, magic_number);
        id = get_string(ifs, 8);

        if(magic_number != 24 || id != "TUNDRA24") {
            throw std::exception();
        }
    }

    {
        uint8_t code;

        screen.current.fg_rgb_mode = true;
        screen.current.bg_rgb_mode = true;

        for(size_t i = ifs.tellg(); i < file_size; ++i) {
            read(ifs, code);
            if(code == 1) {
                screen.y = size_t(read_tundra_32_bit_number(ifs));
                screen.x = size_t(read_tundra_32_bit_number(ifs));
                i += 8;
            } else if(code == 2) {
                read(ifs, code);
                ifs.seekg(1, std::ios_base::cur);
                read(ifs, screen.current.fg_rgb);
                i += 5;
            }
            if(code == 4) {
                read(ifs, code);
                ifs.seekg(1, std::ios_base::cur);
                read(ifs, screen.current.bg_rgb);
                i += 5;
            }
            if(code == 6) {
                read(ifs, code);
                ifs.seekg(1, std::ios_base::cur);
                read(ifs, screen.current.fg_rgb);
                ifs.seekg(1, std::ios_base::cur);
                read(ifs, screen.current.bg_rgb);
                i += 9;
            }
            if (code != 1 && code != 2 && code != 4 && code != 6) {
                screen.putc(code);
            }
        }
    }

    return screen.get_image_data();
}

tundra_draw_t::tundra_draw_t(std::ifstream& ifs)
    : textmode_t(ifs)
{
    image_data = read_tundra_draw_file(ifs, sauce.file_size, size_t(sauce.columns));
    options.non_blink = non_blink_t::on;
    image_data.palette = create_binary_text_palette();
    options.palette_type = palette_type_t::truecolor;
    type = textmode_type_t::tundra_draw;
}
