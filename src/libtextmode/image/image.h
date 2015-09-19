#ifndef IMAGE_H
#define IMAGE_H

#include "textmode.h"

font_t get_font(font_type_t font_type);

palette_t get_palette(palette_type_t palette_type);

class image_t
{
private:
    void draw_glyph_with_ninth_bit(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg, const bool& shift_fg_color);
    void draw_glyph(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg, const bool& shift_fg_color);
public:
    std::vector<uint8_t> rgb_data;
    std::vector<uint8_t> blink_rgb_data;
    uint32_t width = 0;
    uint32_t height = 0;
    image_t(textmode_t& artwork);
    void save_as_png(const std::string& out);
    void save_blink_image_as_png(const std::string& out);
};

#endif
