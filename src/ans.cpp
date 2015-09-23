#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include "textmode.h"
#include "term/text.h"
#include "image/image.h"
#include "scroller/scroller.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
size_t width;
size_t height;

enum output_type_t {
    text,
    ansi,
    xterm256,
    xterm24bit,
    png,
    scroller
};

struct ans_options_t
{
    output_type_t output_type = output_type_t::xterm256;
    bool version = false;
    bool help = false;
};

struct arguments_t
{
    std::vector<std::string> files;
    ans_options_t options;
};

arguments_t get_command_line_arguments(const int& argc, const char* argv[])
{
    arguments_t arguments;
    if(argc == 1) {
        arguments.options.help = true;
    } else {
        for(size_t i = 1; i < argc; ++i) {
            std::string argument = std::string(argv[i]);
            if(argument.size() > 2 && argument[0] == '-' && argument[1] == '-') {
                if(argument == "--help") {
                    arguments.options.help = true;
                } else if(argument == "--version") {
                    arguments.options.version = true;
                } else if(argument == "--text") {
                    arguments.options.output_type = output_type_t::text;
                } else if(argument == "--ansi") {
                    arguments.options.output_type = output_type_t::ansi;
                } else if(argument == "--xterm256") {
                    arguments.options.output_type = output_type_t::xterm256;
                } else if(argument == "--xterm24bit") {
                    arguments.options.output_type = output_type_t::xterm24bit;
                } else if(argument == "--png") {
                    arguments.options.output_type = output_type_t::png;
                } else if(argument == "--scroller") {
                    arguments.options.output_type = output_type_t::scroller;
                } else {
                    throw std::move(argument);
                }
            } else {
                arguments.files.push_back(std::move(argument));
            }
        }
    }
    return std::move(arguments);
}

void sdl_init()
{
    SDL_DisplayMode mode;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetDesktopDisplayMode(0, &mode);
    window = SDL_CreateWindow("ANSiLove-term", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_FULLSCREEN);
    SDL_ShowCursor(SDL_DISABLE);
    width = size_t(mode.w);
    height = size_t(mode.h);
    if(window == NULL) {
        std::cerr << "SDL2: Could not create window." << std::endl;
        std::exit(-1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        std::cerr << "SDL2: Could not create renderer." << std::endl;
        std::exit(-1);
    }
}

void sdl_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char const *argv[])
{
    arguments_t arguments;
    try {
        arguments = get_command_line_arguments(argc, argv);
    } catch(std::string option) {
        std::cout << "Unknown option: " << option << std::endl;
        return -1;
    }
    if(arguments.options.help) {
        std::cout << "usage: ans [--version] [--help] [--text] [--ansi] [--xterm256] [--xterm24bit]" << std::endl;
        std::cout << "       [file ...]" << std::endl;
        std::cout << std::endl;
        std::cout << "    --text            Display as plain-text" << std::endl;
        std::cout << "    --ansi            Display with ANSi escape sequences" << std::endl;
        std::cout << "    --xterm256        Display with XTerm's 256-color palette" << std::endl;
        std::cout << "    --xterm24bit      Display with 24-Bit escape sequences" << std::endl;
        std::cout << "    --png             Generate a PNG image" << std::endl;
        std::cout << "    --scroller        Display artwork as a scrolling display" << std::endl;
        return 0;
    }
    if(arguments.options.version) {
        std::cout << "ans version 0.1" << std::endl;
        return 0;
    }
    switch(arguments.options.output_type) {
    case output_type_t::scroller:
        sdl_init();
        scroller_init(renderer, width, height);
        break;
    default:
        break;
    }
    for(auto filename:arguments.files) {
        try {
            auto artwork = load_artwork(filename);
            switch(arguments.options.output_type) {
            case output_type_t::text:
                display_as_text(std::cout, artwork);
                break;
            case output_type_t::ansi:
                display_as_ansi(std::cout, artwork);
                break;
            case output_type_t::xterm256:
                display_as_xterm256(std::cout, artwork);
                break;
            case output_type_t::xterm24bit:
                display_as_xterm24bit(std::cout, artwork);
                break;
            case output_type_t::png:
                {
                    image_t image(artwork);
                    image.save_as_png(filename + ".png");
                }
                break;
            case output_type_t::scroller:
                if(display_as_scroller(window, width, height, renderer, artwork)) {
                    scroller_quit();
                    sdl_quit();
                    return 0;
                }
                break;
            }
        } catch(file_format_not_recognized_t e) {
            std::cerr << "File format not recognized: " << filename << std::endl;
        } catch(std::exception e) {
            std::cerr << "An error occurred whilst attempting to read " << filename << std::endl;
        }
    }
    switch(arguments.options.output_type) {
    case output_type_t::scroller:
        scroller_quit();
        sdl_quit();
        break;
    default:
        break;
    }
    return 0;
}
