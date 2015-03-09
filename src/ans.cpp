#include <fstream>
#include <iostream>
#include "textmode.h"
#include "term/text.h"

int main(int argc, char const *argv[])
{
    for(size_t i = 1; i < argc; ++i) {
        auto filename = std::string(argv[i]);
        try {
            auto artwork = load_artwork(filename);
            display_as_xterm256(std::cout, artwork);
        } catch(file_format_not_recognized_t e) {
            std::cout << "File format not recognized: " << filename << std::endl;
        } catch(std::exception e) {
            std::cout << "An error occurred whilst attempting to read " << filename << std::endl;
        }
    }
    return 0;
}
