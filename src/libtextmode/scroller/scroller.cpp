#include "scroller.h"
#include "../image/image.h"
#include "timer.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

SDL_Texture* buffer;
SDL_Texture* blink_buffer;
size_t scroller_y = 0;
size_t image_y = 0;
uint8_t old_font_height = 0;
#ifdef _WIN32
char file_seperator = '\\';
#else
char file_seperator =  '/';
#endif

void clear_buffers(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(renderer, buffer);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, blink_buffer);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
}

void scroller_init(SDL_Renderer* renderer, const size_t& width, const size_t& height)
{
    buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height);
    blink_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height);
    clear_buffers(renderer);
}

void scroller_quit()
{
    SDL_DestroyTexture(buffer);
    SDL_DestroyTexture(blink_buffer);
}

void render_glyph(uint8_t* pixels, const int& pitch, const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg)
{
    size_t pixel_offset = y * font.height * pitch + x * font.width * 4;
    uint8_t* pixel = pixels + pixel_offset;
    auto bit = font.bits[code].begin();
    size_t y_offset = pitch - font.width * 4;
    for(size_t y = 0; y < font.height; y += 1, pixel += y_offset) {
        for(size_t x = 0; x < font.width; x += 1, pixel += 4, bit += 1) {
            if(*bit) {
                *(pixel + 0) = fg.red;
                *(pixel + 1) = fg.green;
                *(pixel + 2) = fg.blue;
                *(pixel + 3) = 255;
            } else {
                *(pixel + 3) = 0;
            }
        }
    }
}

SDL_Texture* render_glyphs(SDL_Renderer* renderer, font_t& font, palette_t& palette)
{
    SDL_Texture* glyphs = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, font.width * font.length, font.height * palette.length);
    SDL_SetTextureBlendMode(glyphs, SDL_BLENDMODE_BLEND);
    uint8_t* pixels = NULL;
    int pitch;
    SDL_LockTexture(glyphs, NULL, (void**)&pixels, &pitch);
    for(size_t y = 0; y < 16; y += 1) {
        for(size_t x = 0; x < 256; x += 1) {
            render_glyph(pixels, pitch, x, y, x, font, palette.rgb[y]);
        }
    }
    SDL_UnlockTexture(glyphs);
    return glyphs;
}

void draw_glyph(SDL_Renderer* renderer, SDL_Texture* glyphs, SDL_Texture* buffer, SDL_Texture* blink_buffer, const size_t& x, const size_t& y, const font_t& font, const palette_t& palette, const block_t& block, const bool& non_blink = true)
{
    SDL_Rect src = {int(block.code * font.width), int(block.attr.fg * font.height), int(font.width), int(font.height)};
    SDL_Rect dst = {int(x * font.width), int(y * font.height), int(font.width), int(font.height)};
    uint8_t bg = block.attr.bg;
    if(!non_blink && bg >= 8) {
        bg -= 8;
    }
    SDL_SetRenderTarget(renderer, buffer);
    SDL_SetRenderDrawColor(renderer, palette.rgb[bg].red, palette.rgb[bg].green, palette.rgb[bg].blue, 255);
    SDL_RenderFillRect(renderer, &dst);
    SDL_RenderCopy(renderer, glyphs, &src, &dst);
    if(blink_buffer != NULL) {
        SDL_SetRenderTarget(renderer, blink_buffer);
        if(!non_blink && block.attr.bg >= 8) {
            SDL_RenderFillRect(renderer, &dst);
        } else {
            SDL_RenderCopy(renderer, buffer, &dst, &dst);
        }
    }
}

void draw_glyph_with_ninth_bit(SDL_Renderer* renderer, SDL_Texture* glyphs, SDL_Texture* buffer, SDL_Texture* blink_buffer, const size_t& x, const size_t& y, const font_t& font, const palette_t& palette, const block_t& block, const bool& non_blink = true)
{
    SDL_Rect src = {int(block.code * (font.width)), int(block.attr.fg * font.height), int(font.width), int(font.height)};
    SDL_Rect dst = {int(x * (font.width + 1)), int(y * font.height), int(font.width), int(font.height)};
    uint8_t bg = block.attr.bg;
    if(!non_blink && bg >= 8) {
        bg -= 8;
    }
    SDL_SetRenderTarget(renderer, buffer);
    SDL_SetRenderDrawColor(renderer, palette.rgb[bg].red, palette.rgb[bg].green, palette.rgb[bg].blue, 255);
    SDL_RenderFillRect(renderer, &dst);
    SDL_RenderCopy(renderer, glyphs, &src, &dst);
    if(block.code >= 192 && block.code <= 223) {
        src.x += 7;
        src.w = 1;
        dst.x += 8;
        dst.w = 1;
        SDL_RenderCopy(renderer, glyphs, &src, &dst);
    }
    if(blink_buffer != NULL) {
        SDL_SetRenderTarget(renderer, blink_buffer);
        dst.x -= 8;
        dst.w = font.width + 1;
        if(!non_blink && block.attr.bg >= 8) {
            SDL_RenderFillRect(renderer, &dst);
        } else {
            SDL_RenderCopy(renderer, buffer, &dst, &dst);
        }
    }
}

class popup_t
{
private:
    SDL_Texture* popup;
    SDL_Rect src_rect = {0, 0, 0, 0};
    SDL_Rect dst_rect = {0, 16, 0, 0};
public:
    popup_t(SDL_Renderer* renderer, textmode_t& artwork)
    {
        std::string popup_text = artwork.sauce.title.empty() ? artwork.filename.substr(artwork.filename.find_last_of(file_seperator) + 1) : artwork.sauce.title;
        if(!artwork.sauce.author.empty()) {
            popup_text += " by " + artwork.sauce.author;
        }
        popup_text = " " + popup_text + " ";
        font_t font = get_font(font_type_t::ibm_vga);
        palette_t palette = get_palette(palette_type_t::ansi);
        SDL_Texture* glyphs = render_glyphs(renderer, font, palette);
        popup = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, popup_text.size() * font.width, font.height);
        src_rect.w = font.width * popup_text.size();
        src_rect.h = font.height;
        dst_rect.w = src_rect.w;
        dst_rect.h = src_rect.h;
        block_t block;
        block.attr.fg = 15;
        size_t x = 0;
        for(auto& i:popup_text) {
            block.code = (uint8_t) i;
            draw_glyph(renderer, glyphs, popup, NULL, x, 0, font, palette, block);
            x += 1;
        }
        SDL_DestroyTexture(glyphs);
    }
    void draw(SDL_Renderer* renderer, const size_t& width, const size_t& height)
    {
        dst_rect.x = width - src_rect.w - 16;
        dst_rect.y = height - src_rect.h - 16;
        SDL_RenderCopy(renderer, popup, &src_rect, &dst_rect);
    }
    ~popup_t()
    {
        SDL_DestroyTexture(popup);
    }
};

bool display_as_scroller(SDL_Window* window, const size_t& width, const size_t& height, SDL_Renderer* renderer, textmode_t& artwork, const bool& continuous)
{
    font_t font = (artwork.image_data.font.length == 0) ? get_font(artwork.options.font_type) : artwork.image_data.font;
    palette_t palette = (artwork.image_data.palette.length == 0) ? get_palette(artwork.options.palette_type) : artwork.image_data.palette;
    SDL_Texture* glyphs = render_glyphs(renderer, font, palette);
    SDL_Texture* current_buffer = buffer;
    popup_t popup(renderer, artwork);
    size_t scroller_x = 0;
    timer_t timer;
    bool blink_mode = false;
    SDL_Event event;
    auto block = artwork.image_data.data.begin();
    int screen_start_x;
    bool nine_pixel = (artwork.options.letter_space == letter_space_t::nine_pixels);
    bool non_blink = (artwork.options.non_blink == non_blink_t::on);
    if(nine_pixel) {
        screen_start_x = (width / (font.width + 1) - artwork.image_data.columns) / 2;
    } else {
        screen_start_x = (width / font.width - artwork.image_data.columns) / 2;
    }
    SDL_Rect line_clear = {0, 0, int(width), int(font.height)};
    SDL_Rect render_pos = {0, 0, int(width), int(height)};
    if(!continuous || (font.height != old_font_height)) {
        clear_buffers(renderer);
        scroller_y = 0;
        image_y = 0;
    }
    old_font_height = font.height;
    while(block != artwork.image_data.data.end()) {
        if(SDL_PollEvent(&event) == 1) {
            if(event.type == SDL_QUIT) {
                return true;
            }
        }
        if(nine_pixel) {
            if(screen_start_x + int(scroller_x) >= 0 && screen_start_x + int(scroller_x) < width / (font.width + 1)) {
                draw_glyph_with_ninth_bit(renderer, glyphs, buffer, blink_buffer, screen_start_x + scroller_x, scroller_y, font, palette, *block, non_blink);
            }
        } else {
            if(screen_start_x + int(scroller_x) >= 0 && screen_start_x + int(scroller_x) < width / font.width) {
                draw_glyph(renderer, glyphs, buffer, blink_buffer, screen_start_x + scroller_x, scroller_y, font, palette, *block, non_blink);
            }
        }
        block += 1;
        scroller_x += 1;
        if(scroller_x == artwork.image_data.columns) {
            scroller_x = 0;
            scroller_y += 1;
            image_y += 1;
            if(scroller_y == height / font.height) {
                scroller_y = 0;
            }
            if(image_y > scroller_y) {
                line_clear.y = scroller_y * font.height;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_SetRenderTarget(renderer, buffer);
                SDL_RenderFillRect(renderer, &line_clear);
                SDL_SetRenderTarget(renderer, blink_buffer);
                SDL_RenderFillRect(renderer, &line_clear);
            }
        }
        SDL_SetRenderTarget(renderer, NULL);
        if(image_y > scroller_y) {
            render_pos.y = height - (scroller_y + 1) * font.height;
            SDL_RenderCopy(renderer, current_buffer, NULL, &render_pos);
            render_pos.y -= height;
            SDL_RenderCopy(renderer, current_buffer, NULL, &render_pos);
        } else {
            SDL_RenderCopy(renderer, current_buffer, NULL, NULL);
        }
        if(scroller_x % (artwork.image_data.columns / 8) == 0) {
            popup.draw(renderer, width, height);
            SDL_RenderPresent(renderer);
        }
        if(timer.check() > 250.0) {
            timer.reset();
            blink_mode = !blink_mode;
            current_buffer = blink_mode ? blink_buffer : buffer;
        }
    }
    SDL_DestroyTexture(glyphs);
    return false;
}