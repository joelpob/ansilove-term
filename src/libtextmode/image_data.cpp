#include "image_data.h"
#include <cmath>

rgb_t rgb_t::from_ega_to_rgb()
{
    if((red > 63) || (green > 63) || (blue > 63)) {
        throw std::exception();
    }

    rgb_t rgb;

    rgb.red = (red << 2) | (red >> 4);
    rgb.green = (green << 2) | (green >> 4);
    rgb.blue = (blue << 2) | (blue >> 4);

    return std::move(rgb);
}

lab_t rgb_t::from_rgb_to_lab()
{
    lab_t lab = {double(red), double(green), double(blue)};
    lab.red = lab.red / 255.0;
    lab.green = lab.green / 255.0;
    lab.blue = lab.blue / 255.0;
    if(lab.red > 0.04045) {
        lab.red = pow((lab.red + 0.055) / 1.055, 2.4);
    } else {
        lab.red = lab.red / 12.92;
    }
    if(lab.green > 0.04045) {
        lab.green = pow((lab.green + 0.055) / 1.055, 2.4);
    } else {
        lab.green = lab.green / 12.92;
    }
    if(lab.blue > 0.04045) {
        lab.blue = pow((lab.blue + 0.055) / 1.055, 2.4);
    } else {
        lab.blue = lab.blue / 12.92;
    }
    lab.red = lab.red * 100.0;
    lab.green = lab.green * 100.0;
    lab.blue = lab.blue * 100.0;
    double x = lab.red * 0.4124 + lab.green * 0.3576 + lab.blue * 0.1805;
    double y = lab.red * 0.2126 + lab.green * 0.7152 + lab.blue * 0.0722;
    double z = lab.red * 0.0193 + lab.green * 0.1192 + lab.blue * 0.9505;
    x = x / 95.047;
    y = y / 100.000;
    z = z / 108.883;
    if (x > 0.008856) {
        x = pow(x, 1.0 / 3.0);
    } else {
        x = (7.787 * x) + (16.0 / 116.0);
    }
    if (y > 0.008856) {
        y = pow(y, 1.0 / 3.0);
    } else {
        y = (7.787 * y) + (16.0 / 116.0);
    }
    if (z > 0.008856) {
        z = pow(z, 1.0 / 3.0);
    } else {
        z = (7.787 * z) + (16.0 / 116.0);
    }
    lab.red = (116.0 * y) - 16.0;
    lab.green = 500.0 * (x - y);
    lab.blue = 200.0 * (y - z);
    return std::move(lab);
}

bool rgb_t::operator!=(const rgb_t& other)
{
    return (red != other.red) || (green != other.green) || (blue != other.blue);
}

palette_t::palette_t()
{
}

palette_t::palette_t(std::vector<rgb_t> ega_palette)
{
    for(auto& ega_value:ega_palette) {
        push(ega_value);
    }
}

void palette_t::push(rgb_t& ega_value)
{
    ega.push_back(ega_value);
    rgb_t rgb_value = ega_value.from_ega_to_rgb();
    lab.push_back(rgb_value.from_rgb_to_lab());
    rgb.push_back(std::move(rgb_value));
    length += 1;
}

rgb_t& palette_t::operator[](const size_t& index)
{
    return rgb[index];
}

void font_t::generate_bits()
{
    for(auto i = bytes.begin(); i < bytes.end(); i += 1) {
        std::vector<bool> bits;
        for(auto j = i->begin(); j < i->end(); j += 1) {
            for(int k = 7; k >= 0; k -= 1) {
                bits.push_back(((*j >> k) & 1) == 1);
            }
        }
        this->bits.push_back(bits);
    }
}

font_t::font_t()
{
}

font_t::font_t(font_definition_t font_definition)
{
    this->type = font_definition.type;
    length = font_definition.bytes.size();
    this->height = font_definition.height;
    this->bytes = font_definition.bytes;
    generate_bits();
}

font_t::font_t(std::vector<uint8_t> bytes, size_t height, size_t length, font_type_t type)
{
    this->type = type;
    this->length = length;
    this->height = height;
    for(auto i = bytes.begin(); i < bytes.end(); i += height) {
        this->bytes.push_back(std::vector<uint8_t>(i, i + height));
    }
    generate_bits();
}
