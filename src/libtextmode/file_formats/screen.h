#ifndef SCREEN_H
#define SCREEN_H

#include "../image_data.h"

class serial_screen_t
{
public:
    attribute_t current;

    void putc(const uint8_t&);

    void new_line();

    image_data_t get_image_data();

private:
    std::vector<std::vector<block_t>> data;
    bool start_of_line = true;
};

class screen_t
{
public:
    screen_t(const size_t& columns);

    attribute_t current;

    size_t x = 0;
    size_t y = 0;

    void use_packed_attribute(const uint8_t&);
    void putc(const uint8_t&);
    void new_line();

    image_data_t get_image_data();

protected:
    image_data_t image_data;
};

class ansi_screen_t : public screen_t
{
public:
    ansi_screen_t(const size_t& columns);

    size_t top_of_screen = 0;
    size_t bottom_of_screen = 24;

    uint8_t fg = 7;
    uint8_t bg = 0;

    bool fg_rgb_mode = false;
    bool bg_rgb_mode = false;

    rgb_t fg_rgb = {0, 0, 0};
    rgb_t bg_rgb = {0, 0, 0};

    bool bold = false;
    bool blink = false;
    bool inverse = false;

    void putc(const uint8_t&);
    void new_line();
    void reset_attributes();
    void up(const size_t&);
    void down(const size_t&);
    void right(const size_t&);
    void left(const size_t&);
    void move(const size_t& x, const size_t& y);
    void clear_until_end_of_screen();
    void clear_from_start_of_screen();
    void clear_screen();
    void clear_until_end_of_line();
    void clear_from_start_of_line();
    void clear_line();
    void save_position();
    void restore_position();
private:
    bool position_saved = false;
    size_t saved_x, saved_y;
};

#endif
