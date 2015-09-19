#include <iostream>
#include "scroller.h"
#include "../image/image.h"
#include <SDL2/SDL.h>
#include <vector>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* buffer;
SDL_Texture* buffer_blink;
SDL_DisplayMode display;

bool sdl_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetCurrentDisplayMode(0, &display);
    window = SDL_CreateWindow("ANSiLove-term", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_FULLSCREEN);
    if(window == NULL) {
        std::cerr << "SDL2: Could not create window." << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL) {
        std::cerr << "SDL2: Could not create renderer." << std::endl;
        return false;
    }
    buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, display.w, display.h);
    buffer_blink = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, display.w, display.h);
    return true;
}

void quit_sdl()
{
    SDL_DestroyTexture(buffer);
    SDL_DestroyTexture(buffer_blink);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_glyph(const size_t& x, const size_t& y, const uint8_t& code, const font_t& font, const rgb_t& fg, const rgb_t& bg, const bool& shift_fg_color)
{
    uint8_t* pixels = NULL;
    uint8_t* blink_pixels = NULL;
    int pitch;
    SDL_LockTexture(buffer, NULL, (void**)&pixels, &pitch);
    SDL_LockTexture(buffer_blink, NULL, (void**)&blink_pixels, &pitch);
    size_t pixel_offset = y * font.height * pitch + x * font.width * 4;
    uint8_t* pixel = pixels + pixel_offset;
    uint8_t* pixel_blink = blink_pixels + pixel_offset;
    auto bit = font.bits[code].begin();
    const rgb_t* col;
    size_t y_offset = pitch - font.width * 4;
    for(size_t y = 0; y < font.height; y += 1, pixel += y_offset, pixel_blink += y_offset) {
        for(size_t x = 0; x < font.width; x += 1, pixel += 4, pixel_blink += 4, bit += 1) {
            col = (*bit) ? &fg : &bg;
            *(pixel + 0) = col->blue;
            *(pixel + 1) = col->green;
            *(pixel + 2) = col->red;
            col = shift_fg_color ? &bg : col;
            *(pixel_blink + 0) = col->blue;
            *(pixel_blink + 1) = col->green;
            *(pixel_blink + 2) = col->red;
        }
    }
    SDL_UnlockTexture(buffer);
    SDL_UnlockTexture(buffer_blink);
}

void clear_screen()
{
    uint8_t* pixels = NULL;
    uint8_t* blink_pixels = NULL;
    int pitch;
    SDL_LockTexture(buffer, NULL, (void**)&pixels, &pitch);
    SDL_LockTexture(buffer_blink, NULL, (void**)&blink_pixels, &pitch);
    uint8_t* pixel = pixels;
    uint8_t* pixel_blink = blink_pixels;
    size_t end_of_screen = display.h * pitch;
    std::memset(pixel, 0, end_of_screen);
    std::memset(pixel_blink, 0, end_of_screen);
    SDL_UnlockTexture(buffer);
    SDL_UnlockTexture(buffer_blink);
}

void shift_display_up(const size_t& font_height)
{
    uint8_t* pixels = NULL;
    uint8_t* blink_pixels = NULL;
    int pitch;
    SDL_LockTexture(buffer, NULL, (void**)&pixels, &pitch);
    SDL_LockTexture(buffer_blink, NULL, (void**)&blink_pixels, &pitch);
    uint8_t* pixel = pixels;
    uint8_t* pixel_blink = blink_pixels;
    size_t row_of_text = font_height * pitch;
    size_t start_of_last_row = display.h * pitch - row_of_text;
    std::memcpy(pixel, pixel + row_of_text, start_of_last_row);
    std::memset(pixel + start_of_last_row, 0, row_of_text);
    std::memcpy(pixel_blink, pixel_blink + row_of_text, start_of_last_row);
    std::memset(pixel_blink + start_of_last_row, 0, row_of_text);
    SDL_UnlockTexture(buffer);
    SDL_UnlockTexture(buffer_blink);
}

bool display_as_scroller(textmode_t& artwork)
{
    SDL_Event event;
    font_t font = (artwork.image_data.font.length == 0) ? get_font(artwork.options.font_type) : artwork.image_data.font;
    palette_t palette = (artwork.image_data.palette.length == 0) ? get_palette(artwork.options.palette_type) : artwork.image_data.palette;
    int x = 0;
    int y = 0;
    int screen_start_x = (display.w / font.width - artwork.image_data.columns) / 2;
    rgb_t* fg;
    rgb_t* bg;
    auto block = artwork.image_data.data.begin();
    bool non_blink = (artwork.options.non_blink == non_blink_t::on);
    bool shift_fg_color;
    bool blink_state = false;
    size_t blink_counter = 0;
    clear_screen();
    while(block != artwork.image_data.data.end()) {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            return true;
        }
        fg = (block->attr.fg_rgb_mode) ? &block->attr.fg_rgb : &palette.rgb[block->attr.fg];
        bg = (block->attr.bg_rgb_mode) ? &block->attr.bg_rgb : &palette.rgb[block->attr.bg];
        if(!non_blink && !block->attr.fg_rgb_mode && !block->attr.bg_rgb_mode && block->attr.bg >= 8) {
            bg = &palette.rgb[block->attr.bg - 8];
            shift_fg_color = true;
        } else {
            shift_fg_color = false;
        }
        if(screen_start_x + x >= 0 && screen_start_x + x < display.w / font.width) {
            draw_glyph(screen_start_x + x, y, block->code, font, *fg, *bg, shift_fg_color);
        }
        block += 1;
        if(block == artwork.image_data.data.end()) {
            break;
        }
        x += 1;
        if(x == artwork.image_data.columns) {
            x = 0;
            y += 1;
            if(y == display.h / font.height) {
                shift_display_up(font.height);
                y -= 1;
            }
        }
        if(blink_counter == 250) {
            blink_state = !blink_state;
            blink_counter = 0;
        } else {
            blink_counter += 1;
        }
        if(blink_state) {
            SDL_RenderCopy(renderer, buffer_blink, NULL, NULL);
        } else {
            SDL_RenderCopy(renderer, buffer, NULL, NULL);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_Delay(1000);
    return false;
}