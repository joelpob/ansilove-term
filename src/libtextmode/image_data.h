#ifndef SCREEN_DATA_H
#define SCREEN_DATA_H

#include <vector>
#include <cstddef>
#include <cstdint>
#include "options.h"

struct lab_t
{
    double red, green, blue;
};

struct rgb_t
{
    uint8_t red, green, blue;

    rgb_t from_ega_to_rgb();
    lab_t from_rgb_to_lab();
    bool operator!=(const rgb_t&);
};

struct palette_t
{
    std::vector<rgb_t> ega;
    std::vector<rgb_t> rgb;
    std::vector<lab_t> lab;
    size_t length = 0;

    palette_t();
    palette_t(std::vector<rgb_t> ega_palette);
    void push(rgb_t&);
    rgb_t& operator[](const size_t&);
};

struct attribute_t
{
    uint8_t fg = 7;
    uint8_t bg = 0;

    bool fg_rgb_mode = false;
    bool bg_rgb_mode = false;

    rgb_t fg_rgb = {0, 0, 0};
    rgb_t bg_rgb = {0, 0, 0};
};

struct block_t
{
    uint8_t code;
    attribute_t attr;
};

struct font_definition_t
{
    font_type_t type;
    size_t height;
    std::vector<std::vector<uint8_t>> bytes;
};

class font_t
{
private:
    void generate_bits();
public:
    font_type_t type;
    size_t length = 0;
    size_t width = 8;
    size_t height = 0;
    std::vector<std::vector<uint8_t>> bytes;
    std::vector<std::vector<bool>> bits;

    font_t();
    font_t(font_definition_t);
    font_t(std::vector<uint8_t> bytes, size_t height = 16, size_t length = 256, font_type_t type = font_type_t::custom);
};

class image_data_t
{
public:
    std::vector<block_t> data;
    palette_t palette;

    size_t columns = 0;
    size_t rows = 0;
    font_t font;

    void putc(const uint8_t&);

    void resize(const size_t&);
};

#endif
