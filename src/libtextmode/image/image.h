#ifndef IMAGE_H
#define IMAGE_H

#include "textmode.h"

class image_t
{
private:
    void draw_glyph_with_ninth_bit(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg);
    void draw_glyph(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg);
public:
    std::vector<uint8_t> data;
    uint32_t width = 0;
    uint32_t height = 0;
    image_t(textmode_t& artwork);
    void save_as_png(const std::string& out);
};

#endif
