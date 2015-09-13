#include <iostream>
#include "image.h"
#include "textmode.h"
#include "../lodepng/lodepng.h"
#include "font_definitions.h"
#include "palettes.h"

font_t get_font(font_type_t font_type)
{
    switch(font_type)
    {
        case font_type_t::custom:
        case font_type_t::ibm_vga:
        case font_type_t::ibm_vga_437:
        case font_type_t::invalid:
        case font_type_t::undefined:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga50:
        case font_type_t::ibm_vga50_437:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga25g:
        case font_type_t::ibm_vga25g_437:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_ega:
        case font_type_t::ibm_ega_437:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega43:
        case font_type_t::ibm_ega43_437:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga_720:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga_737:
            return font_t(ibm_vga_737);
        case font_type_t::ibm_vga_775:
            return font_t(ibm_vga_775);
        case font_type_t::ibm_vga_819:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga_850:
            return font_t(ibm_vga_850);
        case font_type_t::ibm_vga_852:
            return font_t(ibm_vga_852);
        case font_type_t::ibm_vga_855:
            return font_t(ibm_vga_855);
        case font_type_t::ibm_vga_857:
            return font_t(ibm_vga_857);
        case font_type_t::ibm_vga_858:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga_860:
            return font_t(ibm_vga_860);
        case font_type_t::ibm_vga_861:
            return font_t(ibm_vga_861);
        case font_type_t::ibm_vga_862:
            return font_t(ibm_vga_862);
        case font_type_t::ibm_vga_863:
            return font_t(ibm_vga_863);
        case font_type_t::ibm_vga_864:
            return font_t(ibm_vga_864);
        case font_type_t::ibm_vga_865:
            return font_t(ibm_vga_865);
        case font_type_t::ibm_vga_866:
            return font_t(ibm_vga_866);
        case font_type_t::ibm_vga_869:
            return font_t(ibm_vga_869);
        case font_type_t::ibm_vga_872:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga_kam:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga_maz:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga_mik:
            return font_t(ibm_vga);
        case font_type_t::ibm_vga50_720:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga50_737:
            return font_t(ibm_vga50_737);
        case font_type_t::ibm_vga50_775:
            return font_t(ibm_vga50_775);
        case font_type_t::ibm_vga50_819:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga50_850:
            return font_t(ibm_vga50_850);
        case font_type_t::ibm_vga50_852:
            return font_t(ibm_vga50_852);
        case font_type_t::ibm_vga50_855:
            return font_t(ibm_vga50_855);
        case font_type_t::ibm_vga50_857:
            return font_t(ibm_vga50_857);
        case font_type_t::ibm_vga50_858:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga50_860:
            return font_t(ibm_vga50_860);
        case font_type_t::ibm_vga50_861:
            return font_t(ibm_vga50_861);
        case font_type_t::ibm_vga50_862:
            return font_t(ibm_vga50_862);
        case font_type_t::ibm_vga50_863:
            return font_t(ibm_vga50_863);
        case font_type_t::ibm_vga50_864:
            return font_t(ibm_vga50_864);
        case font_type_t::ibm_vga50_865:
            return font_t(ibm_vga50_865);
        case font_type_t::ibm_vga50_866:
            return font_t(ibm_vga50_866);
        case font_type_t::ibm_vga50_869:
            return font_t(ibm_vga50_869);
        case font_type_t::ibm_vga50_872:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga50_kam:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga50_maz:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga50_mik:
            return font_t(ibm_vga50);
        case font_type_t::ibm_vga25g_720:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_737:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_775:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_819:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_850:
            return font_t(ibm_vga25g_850);
        case font_type_t::ibm_vga25g_852:
            return font_t(ibm_vga25g_852);
        case font_type_t::ibm_vga25g_855:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_857:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_858:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_860:
            return font_t(ibm_vga25g_860);
        case font_type_t::ibm_vga25g_861:
            return font_t(ibm_vga25g_861);
        case font_type_t::ibm_vga25g_862:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_863:
            return font_t(ibm_vga25g_863);
        case font_type_t::ibm_vga25g_864:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_865:
            return font_t(ibm_vga25g_865);
        case font_type_t::ibm_vga25g_866:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_869:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_872:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_kam:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_maz:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_vga25g_mik:
            return font_t(ibm_vga25g);
        case font_type_t::ibm_ega_720:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega_737:
            return font_t(ibm_vga_737);
        case font_type_t::ibm_ega_775:
            return font_t(ibm_vga_775);
        case font_type_t::ibm_ega_819:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega_850:
            return font_t(ibm_vga_850);
        case font_type_t::ibm_ega_852:
            return font_t(ibm_vga_852);
        case font_type_t::ibm_ega_855:
            return font_t(ibm_vga_855);
        case font_type_t::ibm_ega_857:
            return font_t(ibm_vga_857);
        case font_type_t::ibm_ega_858:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega_860:
            return font_t(ibm_vga_860);
        case font_type_t::ibm_ega_861:
            return font_t(ibm_vga_861);
        case font_type_t::ibm_ega_862:
            return font_t(ibm_vga_862);
        case font_type_t::ibm_ega_863:
            return font_t(ibm_vga_863);
        case font_type_t::ibm_ega_864:
            return font_t(ibm_vga_864);
        case font_type_t::ibm_ega_865:
            return font_t(ibm_vga_865);
        case font_type_t::ibm_ega_866:
            return font_t(ibm_vga_866);
        case font_type_t::ibm_ega_869:
            return font_t(ibm_vga_869);
        case font_type_t::ibm_ega_872:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega_kam:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega_maz:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega_mik:
            return font_t(ibm_vga);
        case font_type_t::ibm_ega43_720:
            return font_t(ibm_vga50);
        case font_type_t::ibm_ega43_737:
            return font_t(ibm_vga50_737);
        case font_type_t::ibm_ega43_775:
            return font_t(ibm_vga50_775);
        case font_type_t::ibm_ega43_819:
            return font_t(ibm_vga50);
        case font_type_t::ibm_ega43_850:
            return font_t(ibm_vga50_850);
        case font_type_t::ibm_ega43_852:
            return font_t(ibm_vga50_852);
        case font_type_t::ibm_ega43_855:
            return font_t(ibm_vga50_855);
        case font_type_t::ibm_ega43_857:
            return font_t(ibm_vga50_857);
        case font_type_t::ibm_ega43_858:
            return font_t(ibm_vga50);
        case font_type_t::ibm_ega43_860:
            return font_t(ibm_vga50_860);
        case font_type_t::ibm_ega43_861:
            return font_t(ibm_vga50_861);
        case font_type_t::ibm_ega43_862:
            return font_t(ibm_vga50_862);
        case font_type_t::ibm_ega43_863:
            return font_t(ibm_vga50_863);
        case font_type_t::ibm_ega43_864:
            return font_t(ibm_vga50_864);
        case font_type_t::ibm_ega43_865:
            return font_t(ibm_vga50_865);
        case font_type_t::ibm_ega43_866:
            return font_t(ibm_vga50_866);
        case font_type_t::ibm_ega43_869:
            return font_t(ibm_vga50_869);
        case font_type_t::ibm_ega43_872:
            return font_t(ibm_vga50);
        case font_type_t::ibm_ega43_kam:
            return font_t(ibm_vga50);
        case font_type_t::ibm_ega43_maz:
            return font_t(ibm_vga50);
        case font_type_t::ibm_ega43_mik:
            return font_t(ibm_vga50);
        case font_type_t::amiga_topaz_1:
            return font_t(ibm_vga50);
        case font_type_t::amiga_topaz_1_plus:
            return font_t(amiga_topaz_1_plus);
        case font_type_t::amiga_topaz_2:
            return font_t(amiga_topaz_2);
        case font_type_t::amiga_topaz_2_plus:
            return font_t(amiga_topaz_2_plus);
        case font_type_t::amiga_p0t_noodle:
            return font_t(amiga_p0t_noodle);
        case font_type_t::amiga_microknight:
            return font_t(amiga_microknight);
        case font_type_t::amiga_microknight_plus:
            return font_t(amiga_microknight_plus);
        case font_type_t::amiga_mosoul:
            return font_t(amiga_mosoul);
        case font_type_t::c64_petscii_unshifted:
            return font_t(c64_petscii_unshifted);
        case font_type_t::c64_petscii_shifted:
            return font_t(c64_petscii_shifted);
        case font_type_t::atari_atascii:
            return font_t(atari_atascii);
    }
}

void image_t::draw_glyph_with_ninth_bit(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg, const bool& shift_fg_color)
{
    size_t pixel_offset = (y * font.height * width + x * (font.width + 1)) * 3;
    auto pixel = rgb_data.begin() + pixel_offset;
    auto blink_pixel = blink_rgb_data.begin() + pixel_offset;
    auto bit = font.bits[code].begin();
    const rgb_t* col;
    size_t y_offset = width * 3 - (font.width + 1) * 3;
    for(size_t y = 0; y < font.height; y += 1, pixel += y_offset, blink_pixel += y_offset) {
        for(size_t x = 0; x < font.width; x += 1, pixel += 3, blink_pixel += 3, bit += 1) {
            col = (*bit) ? &fg : &bg;
            *(pixel + 0) = col->red;
            *(pixel + 1) = col->green;
            *(pixel + 2) = col->blue;
            col = shift_fg_color ? &bg : col;
            *(blink_pixel + 0) = col->red;
            *(blink_pixel + 1) = col->green;
            *(blink_pixel + 2) = col->blue;
        }
        col = (code >= 192 && code <= 223) ? col : &bg;
        *(pixel + 0) = col->red;
        *(pixel + 1) = col->green;
        *(pixel + 2) = col->blue;
        pixel += 3;
        col = shift_fg_color ? &bg : col;
        *(blink_pixel + 0) = col->red;
        *(blink_pixel + 1) = col->green;
        *(blink_pixel + 2) = col->blue;
        blink_pixel += 3;
    }
}

void image_t::draw_glyph(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg, const bool& shift_fg_color)
{
    size_t pixel_offset = (y * font.height * width + x * font.width) * 3;
    auto pixel = rgb_data.begin() + pixel_offset;
    auto blink_pixel = blink_rgb_data.begin() + pixel_offset;
    auto bit = font.bits[code].begin();
    const rgb_t* col;
    size_t y_offset = width * 3 - font.width * 3;
    for(size_t y = 0; y < font.height; y += 1, pixel += y_offset, blink_pixel += y_offset) {
        for(size_t x = 0; x < font.width; x += 1, pixel += 3, blink_pixel += 3, bit += 1) {
            col = (*bit) ? &fg : &bg;
            *(pixel + 0) = col->red;
            *(pixel + 1) = col->green;
            *(pixel + 2) = col->blue;
            col = shift_fg_color ? &bg : col;
            *(blink_pixel + 0) = col->red;
            *(blink_pixel + 1) = col->green;
            *(blink_pixel + 2) = col->blue;
        }
    }
}

image_t::image_t(textmode_t& artwork)
{
    font_t font = (artwork.image_data.font.length == 0) ? get_font(artwork.options.font_type) : artwork.image_data.font;
    palette_t palette = (artwork.image_data.palette.length == 0) ? palette_t(ansi_palette) : artwork.image_data.palette;
    if(artwork.options.letter_space == letter_space_t::nine_pixels) {
        width = artwork.image_data.columns * (font.width + 1);
    } else {
        width = artwork.image_data.columns * font.width;
    }
    height = artwork.image_data.rows * font.height;
    rgb_data.resize(width * height * 3);
    blink_rgb_data.resize(width * height * 3);
    auto block = artwork.image_data.data.begin();
    rgb_t* fg;
    rgb_t* bg;
    bool non_blink = (artwork.options.non_blink == non_blink_t::on);
    bool shift_fg_color;
    for(size_t y = 0; y < artwork.image_data.rows; y += 1) {
        for(size_t x = 0; x < artwork.image_data.columns; x += 1, block += 1) {
            fg = (block->attr.fg_rgb_mode) ? &block->attr.fg_rgb : &palette.rgb[block->attr.fg];
            bg = (block->attr.bg_rgb_mode) ? &block->attr.bg_rgb : &palette.rgb[block->attr.bg];
            if(!non_blink && !block->attr.fg_rgb_mode && !block->attr.bg_rgb_mode && block->attr.bg >= 8) {
                bg = &palette.rgb[block->attr.bg - 8];
                shift_fg_color = true;
            } else {
                shift_fg_color = false;
            }
            if(artwork.options.letter_space == letter_space_t::nine_pixels) {
                draw_glyph_with_ninth_bit(x, y, block->code, font, *fg, *bg, shift_fg_color);
            } else {
                draw_glyph(x, y, block->code, font, *fg, *bg, shift_fg_color);
            }
        }
    }
}

void save_png(std::vector<uint8_t>& rgb_data, const uint32_t& width, const uint32_t& height, const std::string& out)
{
    std::unique_ptr<uint8_t[]> uint8_array(new uint8_t[rgb_data.size()]);
    std::copy(rgb_data.begin(), rgb_data.end(), uint8_array.get());
    lodepng::encode(out, uint8_array.get(), width, height, LCT_RGB, 8);
}

void image_t::save_as_png(const std::string& out)
{
    save_png(rgb_data, width, height, out);
}

void image_t::save_blink_image_as_png(const std::string& out)
{
    save_png(blink_rgb_data, width, height, out);
}
