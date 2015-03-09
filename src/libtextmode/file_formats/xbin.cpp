#include "../textmode.h"
#include "xbin.h"
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

inline void read(std::ifstream& ifs, uint16_t& data)
{
    uint8_t data_buffer[2];
    read(ifs, data_buffer[0]);
    read(ifs, data_buffer[1]);
    data = data_buffer[0] + (data_buffer[1] << 8);
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

void write_compressed_data_to_screen(screen_t& screen, std::ifstream& ifs, const size_t& image_size)
{
    uint8_t uint8, iterations, code, attr;
    for(size_t i = 0; i < image_size;)
    {
        read(ifs, uint8);
        iterations = uint8 & 077;
        switch(uint8 >> 6) {
        case 1:
            read(ifs, code);
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                read(ifs, attr);
                screen.use_packed_attribute(attr);
                screen.putc(code);
            }
            break;
        case 2:
            read(ifs, attr);
            screen.use_packed_attribute(attr);
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                read(ifs, code);
                screen.putc(code);
            }
            break;
        case 3:
            read(ifs, code);
            read(ifs, attr);
            screen.use_packed_attribute(attr);
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                screen.putc(code);
            }
            break;
        default:
            for(size_t j = 0; j <= iterations; ++j, ++i) {
                read(ifs, code);
                read(ifs, attr);
                screen.use_packed_attribute(attr);
                screen.putc(code);
            }
            break;
        }
    }
}

void write_uncompressed_data_to_screen(screen_t& screen, std::ifstream& ifs, const size_t& image_size)
{
    uint8_t code, attr;
    for(size_t i = 0; i < image_size; ++i) {
        read(ifs, code);
        read(ifs, attr);
        screen.current.fg = attr & 0xf;
        screen.current.bg = attr >> 4;
        screen.putc(code);
    }
}

image_data_t read_xbin_file(std::ifstream& ifs, const size_t& file_size, options_t& options)
{
    uint16_t columns, rows;
    uint8_t font_size;
    bool palette_flag, font_flag, compress_flag, non_blink_flag, chars_512_flag;
    palette_t ega_palette;
    std::vector<uint8_t> font;

    {
        std::string id = get_string(ifs, 4);
        if(id != "XBIN") {
            throw std::exception();
        }
    }

    {
        uint8_t eof_char;
        read(ifs, eof_char);
        if(eof_char != 0x1a) {
            throw std::exception();
        }
    }

    read(ifs, columns);
    read(ifs, rows);
    read(ifs, font_size);

    if(font_size == 0 || font_size > 32) {
        throw std::exception();
    }

    {
        uint8_t flags;
        read(ifs, flags);
        palette_flag = ((flags >> 0) & 1) == 1;
        font_flag = ((flags >> 1) & 1) == 1;
        compress_flag = ((flags >> 2) & 1) == 1;
        non_blink_flag = ((flags >> 3) & 1) == 1;
        chars_512_flag = ((flags >> 4) & 1) == 1;
        if(flags >> 5 != 0) {
            throw std::exception();
        }
    }

    if(palette_flag) {
        for(size_t i = 0; i < 16; ++i) {
            rgb_t ega_value;
            read(ifs, ega_value);
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
        read(ifs, font);
        options.font_type = font_type_t::custom;
    } else {
        options.font_type = font_type_t::undefined;
    }

    {
        screen_t screen(columns);
        if(compress_flag) {
            write_compressed_data_to_screen(screen, ifs, columns * rows);
        } else {
            write_uncompressed_data_to_screen(screen, ifs, columns * rows);
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
}

xbin_t::xbin_t(std::ifstream& ifs)
    : textmode_t(ifs)
{
    image_data = read_xbin_file(ifs, sauce.file_size, options);
    options.letter_space = letter_space_t::undefined;
    options.aspect_ratio = aspect_ratio_t::undefined;
    options.font_type = font_type_t::undefined;
    type = textmode_type_t::xbin;
}
