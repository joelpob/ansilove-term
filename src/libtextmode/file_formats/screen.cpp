#include "screen.h"
#include <algorithm>

void serial_screen_t::putc(const uint8_t& code)
{
    if(start_of_line) {
        start_of_line = false;
        data.push_back(std::vector<block_t>());
    }
    data.back().push_back(block_t{code, current});
}

void serial_screen_t::new_line()
{
    start_of_line = true;
}

bool compare_length(std::vector<block_t>& a, std::vector<block_t>& b)
{
    return a.size() < b.size();
}

image_data_t serial_screen_t::get_image_data()
{
    image_data_t image_data;
    if(data.size() > 0) {
        auto longest = std::max_element(data.begin(), data.end(), compare_length);
        image_data.columns = longest->size();
        image_data.rows = data.size();
        for(auto& i:data) {
            i.resize(image_data.columns);
            image_data.data.insert(image_data.data.end(), i.begin(), i.end());
        }
    }
    return std::move(image_data);
}

screen_t::screen_t(const size_t& columns)
{
    image_data.columns = columns;
}

image_data_t screen_t::get_image_data()
{
    image_data.data.resize(image_data.columns * image_data.rows);
    return std::move(image_data);
}

void screen_t::use_packed_attribute(const uint8_t& attr)
{
    current.fg = attr & 0xf;
    current.bg = attr >> 4;
}

void screen_t::putc(const uint8_t& code)
{
    size_t pos = y * image_data.columns + x;
    if(pos >= image_data.data.size()) {
        image_data.data.resize(pos / image_data.columns + 10'000 * image_data.columns);
    }
    image_data.data[pos] = block_t{code, current};
    x += 1;

    if(x == image_data.columns) {
        new_line();
    }

    if(y + 1 > image_data.rows) {
        image_data.rows = y + 1;
    }
}

void screen_t::new_line()
{
    y += 1;
    x = 0;
}

void ansi_screen_t::reset_attributes()
{
    bold = false;
    blink = false;
    inverse = false;

    fg = 7;
    bg = 0;

    fg_rgb_mode = false;
    bg_rgb_mode = false;
}

ansi_screen_t::ansi_screen_t(const size_t& columns)
    : screen_t(columns)
{
}

inline void adjust_screen(const size_t& y, size_t& top_of_screen, size_t& bottom_of_screen)
{
    if(y > bottom_of_screen) {
        top_of_screen += 1;
        bottom_of_screen += 1;
    }
}

void ansi_screen_t::putc(const uint8_t& code)
{
    current.fg = bold ? fg + 8 : fg;
    current.bg = blink ? bg + 8 : bg;

    current.fg_rgb_mode = fg_rgb_mode;
    current.bg_rgb_mode = bg_rgb_mode;

    if(current.fg_rgb_mode) {
        current.fg_rgb = fg_rgb;
    }

    if(current.bg_rgb_mode) {
        current.bg_rgb = bg_rgb;
    }

    if(inverse) {
        std::swap(current.fg, current.bg);
        std::swap(current.fg_rgb_mode, current.bg_rgb_mode);
        std::swap(current.fg_rgb, current.bg_rgb);
    }

    screen_t::putc(code);
    adjust_screen(y, top_of_screen, bottom_of_screen);
}

void ansi_screen_t::new_line()
{
    screen_t::new_line();
    adjust_screen(y, top_of_screen, bottom_of_screen);
}

void ansi_screen_t::up(const size_t& value)
{
    if(y < value) {
        y = 0;
    } else {
        y -= value;
    }
    if(y < top_of_screen) {
        y = top_of_screen;
    }
}

void ansi_screen_t::down(const size_t& value)
{
    y += value;
    if(y > bottom_of_screen) {
        y = bottom_of_screen;
    }
}

void ansi_screen_t::right(const size_t& value)
{
    if(x + value < image_data.columns) {
        x += value;
    } else {
        x = image_data.columns - 1;
    }
}

void ansi_screen_t::left(const size_t& value)
{
    if(x < value) {
        x = 0;
    } else {
        x -= value;
    }
}

void ansi_screen_t::move(const size_t& set_x, const size_t& set_y)
{
    x = set_x - 1;
    y = set_y - 1 + top_of_screen;
}

void ansi_screen_t::clear_until_end_of_screen()
{
    size_t temp_x = x;
    size_t temp_y = y;
    x = 0;
    current = attribute_t();
    while(!(x == image_data.columns && y == bottom_of_screen - 1)) {
        screen_t::putc(0);
    } 
    x = temp_x;
    y = temp_y;
}

void ansi_screen_t::clear_from_start_of_screen()
{
    size_t temp_x = x;
    size_t temp_y = y;
    x = 0;
    y = 0;
    current = attribute_t();
    while(!(x == image_data.columns && y == temp_y)) {
        screen_t::putc(0);
    } 
    x = temp_x;
    y = temp_y;
}

void ansi_screen_t::clear_screen()
{
    size_t columns = image_data.columns;
    image_data = image_data_t();
    image_data.columns = columns;
    x = 0;
    y = 0;
}

void ansi_screen_t::clear_until_end_of_line()
{
    size_t temp_x = x;
    current = attribute_t();
    while(x < image_data.columns) {
        screen_t::putc(0);
    }
    x = temp_x;
}

void ansi_screen_t::clear_from_start_of_line()
{
    size_t temp_x = x;
    x = 0;
    current = attribute_t();
    while(x < temp_x + 1) {
        screen_t::putc(0);
    }
    x = temp_x;
}

void ansi_screen_t::clear_line()
{
    size_t temp_x = x;
    x = 0;
    current = attribute_t();
    while(x < image_data.columns) {
        screen_t::putc(0);
    }
    x = temp_x;
}

void ansi_screen_t::save_position()
{
    position_saved = true;
    saved_x = x;
    saved_y = y;
}

void ansi_screen_t::restore_position()
{
    if(position_saved) {
        x = saved_x;
        y = saved_y;
    }
}
