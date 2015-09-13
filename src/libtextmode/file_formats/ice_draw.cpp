#include "../textmode.h"
#include "ice_draw.h"
#include "screen.h"

image_data_t read_ice_draw_file(file_t& file, const size_t& file_size)
{
    screen_t screen(80);

    file.seek_from_start(12);
    std::vector<uint8_t> font_bytes(4096);

    uint8_t code, attr, iterations;
    for(size_t i = file.tell(); i < file_size - (48 + 4096); i += 2) {
        code = file.read_byte();
        attr = file.read_byte();
        if(code == 1) {
            iterations = file.read_byte();
            file.seek(1);
            code = file.read_byte();
            attr = file.read_byte();
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

    image_data_t image_data = screen.get_image_data();

    file.read_bytes(font_bytes);

    for(size_t i = 0; i < 16; ++i) {
        rgb_t ega_value = file.read_rgb();
        image_data.palette.push(ega_value);
    }

    image_data.font = font_t(font_bytes);

    return std::move(image_data);
}

ice_draw_t::ice_draw_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_ice_draw_file(file, sauce.file_size);
    options.non_blink = non_blink_t::on;
    options.font_type = font_type_t::custom;
    options.palette_type = palette_type_t::custom;
    type = textmode_type_t::ice_draw;
}
