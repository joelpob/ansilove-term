#include "../textmode.h"
#include "xbin.h"
#include "screen.h"
#include "palettes.h"

void write_compressed_data_to_screen(screen_t& screen, file_t& file, const size_t& image_size)
{
    uint8_t uint8, iterations, code, attr;
    for(size_t i = 0; i < image_size;)
    {
        uint8 = file.read_byte();
        iterations = uint8 & 077;
        switch(uint8 >> 6) {
        case 1:
            code = file.read_byte();
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                attr = file.read_byte();
                screen.use_packed_attribute(attr);
                screen.putc(code);
            }
            break;
        case 2:
            attr = file.read_byte();
            screen.use_packed_attribute(attr);
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                code = file.read_byte();
                screen.putc(code);
            }
            break;
        case 3:
            code = file.read_byte();
            attr = file.read_byte();
            screen.use_packed_attribute(attr);
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                screen.putc(code);
            }
            break;
        default:
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                code = file.read_byte();
                attr = file.read_byte();
                screen.use_packed_attribute(attr);
                screen.putc(code);
            }
            break;
        }
    }
}

void write_uncompressed_data_to_screen(screen_t& screen, file_t& file, const size_t& image_size)
{
    uint8_t code, attr;
    for(size_t i = 0; i < image_size; ++i) {
        code = file.read_byte();
        attr = file.read_byte();
        screen.current.fg = attr & 0xf;
        screen.current.bg = attr >> 4;
        screen.putc(code);
    }
}

image_data_t read_xbin_file(file_t& file, const size_t& file_size, options_t& options)
{
    uint16_t columns, rows;
    uint8_t font_size;
    bool palette_flag, font_flag, compress_flag, non_blink_flag, chars_512_flag;
    palette_t ega_palette;
    std::vector<uint8_t> font;

    std::string id = file.read_string(4);
    if(id != "XBIN") {
        throw std::exception();
    }

    uint8_t eof_char = file.read_byte();
    if(eof_char != 0x1a) {
        throw std::exception();
    }

    columns = file.read_16_bit_word();
    rows = file.read_16_bit_word();
    font_size = file.read_byte();

    if(font_size == 0 || font_size > 32) {
        throw std::exception();
    }

    uint8_t flags = file.read_byte();
    palette_flag = ((flags >> 0) & 1) == 1;
    font_flag = ((flags >> 1) & 1) == 1;
    compress_flag = ((flags >> 2) & 1) == 1;
    non_blink_flag = ((flags >> 3) & 1) == 1;
    chars_512_flag = ((flags >> 4) & 1) == 1;
    if(flags >> 5 != 0) {
        throw std::exception();
    }

    if(palette_flag) {
        for(size_t i = 0; i < 16; ++i) {
            rgb_t ega_value = file.read_rgb();
            ega_palette.push(ega_value);
        }
        options.palette_type = palette_type_t::custom;
    } else {
        options.palette_type = palette_type_t::binary_text;
    }

    if(font_flag) {

        if(font_size == 0) {
            throw std::exception();
        }

        font.resize(font_size * (chars_512_flag ? 512 : 256));
        file.read_bytes(font);
        options.font_type = font_type_t::custom;
    } else {
        options.font_type = font_type_t::undefined;
    }

    screen_t screen(columns);
    if(compress_flag) {
        write_compressed_data_to_screen(screen, file, columns * rows);
    } else {
        write_uncompressed_data_to_screen(screen, file, columns * rows);
    }

    image_data_t image_data = screen.get_image_data();

    if(palette_flag) {
        image_data.palette = std::move(ega_palette);
    } else {
        image_data.palette = create_binary_text_palette();
    }

    options.non_blink = non_blink_flag ? non_blink_t::on : non_blink_t::off;

    return std::move(image_data);
}

xbin_t::xbin_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_xbin_file(file, sauce.file_size, options);
    options.letter_space = letter_space_t::undefined;
    options.aspect_ratio = aspect_ratio_t::undefined;
    options.font_type = font_type_t::undefined;
    type = textmode_type_t::xbin;
}
