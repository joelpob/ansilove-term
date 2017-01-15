#include <fstream>
#include <iostream>
#include "textmode.h"
#include "term/text.h"
#include "image/image.h"

size_t width;
size_t height;

enum output_type_t {
    text,
    ansi,
    xterm256,
    xterm24bit,
    png
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
                } else {
                    throw std::move(argument);
                }
            } else {
                arguments.files.push_back(std::move(argument));
            }
        }
    }
    return arguments;
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
        std::cout << "       [--png] [file ...]" << std::endl;
        std::cout << std::endl;
        std::cout << "    --text                 Display as plain-text" << std::endl;
        std::cout << "    --ansi                 Display with ANSi escape sequences" << std::endl;
        std::cout << "    --xterm256             Display with XTerm's 256-color palette" << std::endl;
        std::cout << "    --xterm24bit           Display with 24-Bit escape sequences" << std::endl;
        std::cout << "    --png                  Generate a PNG image" << std::endl;
        return 0;
    }
    if(arguments.options.version) {
        std::cout << "ans version 0.1" << std::endl;
        return 0;
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
            }
        } catch(file_format_not_recognized_t e) {
            std::cerr << "File format not recognized: " << filename << std::endl;
        } catch(std::exception e) {
            std::cerr << "An error occurred whilst attempting to read " << filename << std::endl;
        }
    }
    return 0;
}
