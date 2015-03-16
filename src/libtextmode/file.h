#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include "image_data.h"

class file_t
{
public:
    file_t(const std::string&);

    uint8_t read_byte();
    void read_bytes(std::vector<uint8_t>& bytes);
    void read_bytes(uint8_t*, const size_t&);
    uint16_t read_16_bit_word();
    uint32_t read_32_bit_word();
    uint32_t read_big_endian_32_bit_word();
    std::string read_string(const size_t&);
    rgb_t read_rgb();
    void read_palette(std::vector<rgb_t>& palette);
    void seek(const size_t&);
    void seek_from_start(const size_t&);
    void seek_from_end(const size_t&);
    size_t tell();
private:
    std::ifstream ifs;
};

#endif
