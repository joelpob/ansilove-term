#include "../textmode.h"
#include "artworx.h"
#include "screen.h"

uint8_t artworx_palette_order[] = {0, 1, 2, 3, 4, 5, 20, 7, 56, 57, 58, 59, 60, 61, 62, 63};

image_data_t read_artworx_file(file_t& file, const size_t& file_size)
{
    screen_t screen(80);
    palette_t ega_palette;
    std::vector<uint8_t> font_bytes(4096);

    uint8_t version = file.read_byte();
    if(version != 1) {
        throw std::exception();
    }

    std::vector<rgb_t> full_palette(64);
    file.read_palette(full_palette);
    for(size_t i = 0; i < sizeof(artworx_palette_order); ++i) {
        rgb_t& rgb = full_palette[artworx_palette_order[i]];
        ega_palette.push(rgb);
    }

    file.read_bytes(font_bytes);

    uint8_t code, attr;
    for(size_t i = file.tell(); i < file_size; i += 2) {
        code = file.read_byte();
        attr = file.read_byte();
        screen.use_packed_attribute(attr);
        screen.putc(code);
    }

    image_data_t image_data = screen.get_image_data();

    image_data.palette = std::move(ega_palette);
    image_data.font = font_t(font_bytes);

    return std::move(image_data);
}

artworx_t::artworx_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_artworx_file(file, sauce.file_size);
    options.non_blink = non_blink_t::on;
    options.font_type = font_type_t::custom;
    options.palette_type = palette_type_t::custom;
    type = textmode_type_t::artworx;
}
