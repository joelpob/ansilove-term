#include "../textmode.h"
#include "tundra_draw.h"
#include "screen.h"
#include "palettes.h"

image_data_t read_tundra_draw_file(file_t& file, const size_t& file_size, const size_t& columns)
{
    screen_t screen(columns == 0 ? 80 : columns);

    uint8_t magic_number = file.read_byte();
    std::string id = file.read_string(8);

    if(magic_number != 24 || id != "TUNDRA24") {
        throw std::exception();
    }

    uint8_t code;

    screen.current.fg_rgb_mode = true;
    screen.current.bg_rgb_mode = true;

    for(size_t i = file.tell(); i < file_size; ++i) {
        code = file.read_byte();
        if(code == 1) {
            screen.y = file.read_big_endian_32_bit_word();
            screen.x = file.read_big_endian_32_bit_word();
            i += 8;
        } else if(code == 2) {
            code = file.read_byte();
            file.seek(1);
            screen.current.fg_rgb = file.read_rgb();
            i += 5;
        }
        if(code == 4) {
            code = file.read_byte();
            file.seek(1);
            screen.current.bg_rgb = file.read_rgb();
            i += 5;
        }
        if(code == 6) {
            code = file.read_byte();
            file.seek(1);
            screen.current.fg_rgb = file.read_rgb();
            file.seek(1);
            screen.current.bg_rgb = file.read_rgb();
            i += 9;
        }
        if (code != 1 && code != 2 && code != 4 && code != 6) {
            screen.putc(code);
        }
    }

    return screen.get_image_data();
}

tundra_draw_t::tundra_draw_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_tundra_draw_file(file, sauce.file_size, size_t(sauce.columns));
    options.non_blink = non_blink_t::on;
    image_data.palette = create_binary_text_palette();
    options.palette_type = palette_type_t::truecolor;
    type = textmode_type_t::tundra_draw;
}
