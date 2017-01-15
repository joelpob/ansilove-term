#include "file.h"

file_t::file_t(const std::string& filename)
    : ifs(filename, std::ios_base::in | std::ios_base::binary)
{
    if(!ifs || ifs.bad()) {
        throw std::exception();
    }
}

uint8_t file_t::read_byte()
{
    uint8_t byte;
    ifs.read(reinterpret_cast<char*>(&byte), 1);
    if(ifs.fail()) {
        ifs.clear();
        throw std::exception();
    }
    return byte;
}

void file_t::read_bytes(std::vector<uint8_t>& bytes)
{
    for(auto& byte:bytes) {
        ifs.read(reinterpret_cast<char*>(&byte), 1);
        if(ifs.fail()) {
            ifs.clear();
            throw std::exception();
        }
    }
}

void file_t::read_bytes(uint8_t* bytes, const size_t& size)
{
    for(size_t i = 0; i < size; ++i) {
        ifs.read(reinterpret_cast<char*>(&bytes[i]), 1);
        if(ifs.fail()) {
            ifs.clear();
            throw std::exception();
        }
    }
}

uint16_t file_t::read_16_bit_word()
{
    uint8_t bytes[2];
    read_bytes(bytes, sizeof(bytes));
    return bytes[0] + (bytes[1] << 8);
}

uint32_t file_t::read_32_bit_word()
{
    uint8_t bytes[4];
    read_bytes(bytes, sizeof(bytes));
    return bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
}

uint32_t file_t::read_big_endian_32_bit_word()
{
    uint8_t bytes[4];
    read_bytes(bytes, sizeof(bytes));
    return (uint32_t(bytes[0]) << 24) + (uint32_t(bytes[1]) << 16) + (uint32_t(bytes[2]) << 8) + uint32_t(bytes[3]);
}

std::string file_t::read_string(const size_t& size)
{
    char bytes[size];
    ifs.read(bytes, size);
    if(ifs.fail()) {
        ifs.clear();
        return std::string();
    }
    return std::string{bytes, size};
}

rgb_t file_t::read_rgb()
{
    rgb_t rgb;
    rgb.red = read_byte();
    rgb.green = read_byte();
    rgb.blue = read_byte();
    return rgb;
}

void file_t::read_palette(std::vector<rgb_t>& palette)
{
    for(auto& rgb:palette) {
        rgb = read_rgb();
    }
}

void file_t::seek(const size_t& pos)
{
    ifs.seekg(pos, std::ios_base::cur);
}

void file_t::seek_from_start(const size_t& pos)
{
    ifs.seekg(pos, std::ios_base::beg);
}

void file_t::seek_from_end(const size_t& pos)
{
    ifs.seekg(-int(pos), std::ios_base::end);
}

size_t file_t::tell()
{
    return ifs.tellg();
}
