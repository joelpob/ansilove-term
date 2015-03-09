#ifndef SCREEN_DATA_H
#define SCREEN_DATA_H

#include <vector>

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

    void push(rgb_t&);
    bool empty();
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

class image_data_t
{
public:
    std::vector<block_t> data;
    palette_t palette;

    size_t columns = 0;
    size_t rows = 0;

    void putc(const uint8_t&);

    void resize(const size_t&);
};

#endif
