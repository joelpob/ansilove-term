#include "../textmode.h"
#include "ansiedit.h"
#include "screen.h"
#include <math.h>

struct ansiedit_block_t {
    std::string id;
    uint32_t block_length;
    std::vector<uint8_t> data;

    void decompress();
};

inline uint32_t obtain_32_bit_value(const std::vector<uint8_t>& data, const size_t& offset)
{
    return uint32_t(data[offset]) + (uint32_t(data[offset + 1]) << 8) + (uint32_t(data[offset + 2]) << 16) + (uint32_t(data[offset + 3]) << 24);
}

inline uint16_t obtain_16_bit_value(const std::vector<uint8_t>& data, const size_t& offset)
{
    return uint16_t(data[offset]) + (uint16_t(data[offset + 1]) << 8);
}

void ansiedit_block_t::decompress()
{
    uint16_t input_pointer, pointer_pos, pointer_length;
    if(data.size() < 5) {
        throw std::exception();
    }
    uint32_t uncompressed_size = obtain_32_bit_value(data, 0);
    std::vector<uint8_t> uncompressed_data(uncompressed_size);
    uint8_t pointer_length_width = data[4];
    uint16_t pointer_length_mask = pow(uint16_t(2), pointer_length_width) - 1;
    size_t compressed_pointer = 5;
    for(size_t coding_pos = 0; coding_pos < uncompressed_size; ++coding_pos) {
        input_pointer = obtain_16_bit_value(data, compressed_pointer);
        compressed_pointer += 2;
        pointer_pos = input_pointer >> pointer_length_width;
        if(pointer_pos > 0) {
            pointer_length = (input_pointer & pointer_length_mask) + 1;
            for(size_t pointer_offset = coding_pos - pointer_pos; pointer_length > 0; --pointer_length) {
                if(coding_pos >= uncompressed_size || pointer_offset >= uncompressed_size) {
                    throw std::exception();
                }
                uncompressed_data[coding_pos] = uncompressed_data[pointer_offset];
                coding_pos += 1;
                pointer_offset += 1;
            }
        } else {
            pointer_length = 0;
        }
        if(coding_pos >= uncompressed_size || compressed_pointer >= block_length) {
            throw std::exception();
        }
        uncompressed_data[coding_pos] = data[compressed_pointer];
        compressed_pointer += 1;
    }
    data = std::move(uncompressed_data);
}

ansiedit_block_t parse_block(const std::vector<uint8_t>& data)
{
    ansiedit_block_t block;

    if(data.size() < 9) {
        throw std::exception();
    }

    for(size_t i = 0; i < 4; ++i) {
        block.id += data[i];
    }

    bool compressed = (data[4] == 1);

    block.block_length = obtain_32_bit_value(data, 5);

    if(data.size() < block.block_length + 9) {
        throw std::exception();
    }

    block.data = std::vector<uint8_t>(data.begin() + 9, data.begin() + 9 + block.block_length);

    if(compressed) {
        block.decompress();
    }

    return std::move(block);
}

image_data_t decode_display(const ansiedit_block_t& block, options_t& options)
{
    if(block.block_length < 5) {
        throw std::exception();
    }

    uint16_t columns = obtain_16_bit_value(block.data, 0);
    uint16_t rows = obtain_16_bit_value(block.data, 2);
    bool non_blink = (block.data[4] == 1);

    options.non_blink = non_blink ? non_blink_t::on : non_blink_t::off;

    screen_t screen(columns);

    if(block.block_length != 5 + columns * rows * 2) {
        throw std::exception();
    }

    for(size_t i = 5; i < block.block_length; i += 2) {
        screen.use_packed_attribute(block.data[i + 1]);
        screen.putc(block.data[i]);
    }

    return screen.get_image_data();
}

palette_t decode_ega_palette(const ansiedit_block_t& block)
{
    if(block.block_length != 48) {
        throw std::exception();
    }

    palette_t ega_palette;

    for(size_t i = 0; i < 48; i += 3) {
        rgb_t ega_value = rgb_t{block.data[i], block.data[i + 1], block.data[i + 2]};
        ega_palette.push(ega_value);
    }

    return std::move(ega_palette);
}

void decode_metadata(const ansiedit_block_t& block, std::string& title, std::string& author, std::string& group)
{
    size_t i = 0;
    for(const auto& ascii:block.data) {
        if(ascii == 0) {
            i += 1;
            if(i == 3) {
                break;
            }
        } else {
            switch(i) {
            case 0:
                title += ascii;
                break;
            case 1:
                author += ascii;
                break;
            case 2:
                group += ascii;
                break;
            default:
                break;
            }
        }
    }

    if(title.size() + author.size() + group.size() + 3 != block.data.size()) {
        throw std::exception();
    }
}

image_data_t read_ansiedit_file(file_t& file, const size_t& file_size, options_t& options, std::string& title, std::string& author, std::string& group)
{
    std::vector<uint8_t> data(file_size);
    file.read_bytes(data);
    auto parent = parse_block(data);
    ansiedit_block_t child;
    image_data_t image_data;
    palette_t ega_palette;

    for(size_t i = 0; i < parent.block_length; i += child.block_length + 9) {
        std::vector<uint8_t> child_data(parent.data.begin() + i, parent.data.end());
        child = parse_block(child_data);
        if(child.id == "DISP") {
            image_data = decode_display(child, options);
        } else if(child.id == "PALE") {
            ega_palette = decode_ega_palette(child);
        } else if(child.id == "META") {
            decode_metadata(child, title, author, group);
        }
    }

    if(ega_palette.length == 0) {
        options.palette_type = palette_type_t::binary_text;
    } else {
        image_data.palette = std::move(ega_palette);
        options.palette_type = palette_type_t::custom;
    }

    return std::move(image_data);
}

ansiedit_t::ansiedit_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_ansiedit_file(file, sauce.file_size, options, title, author, group);
    type = textmode_type_t::ansiedit;
}
