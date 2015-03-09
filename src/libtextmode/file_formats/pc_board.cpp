#include <iostream>
#include "../textmode.h"
#include "pc_board.h"
#include "screen.h"
#include "palettes.h"

class pc_board_tokens_t
{
public:
    enum class type_t {pc_board_sequence, literal};

    std::vector<type_t> types;
    std::vector<std::string> pc_board_sequences;
    std::vector<uint8_t> literals;

    void push(const std::string&);
    void push(const uint8_t&);
};

void pc_board_tokens_t::push(const std::string& pc_board_sequence)
{
    types.push_back(type_t::pc_board_sequence);
    pc_board_sequences.push_back(pc_board_sequence);
}

void pc_board_tokens_t::push(const uint8_t& literal)
{
    types.push_back(type_t::literal);
    literals.push_back(literal);
}

inline void read(std::ifstream& ifs, uint8_t& data)
{
    ifs.read(reinterpret_cast<char*>(&data), 1);
    if(ifs.fail()) {
        ifs.clear();
        throw std::exception();
    }
}

pc_board_tokens_t tokenize_pc_board_file(std::ifstream& ifs, const size_t& file_size)
{
    pc_board_tokens_t pc_board_tokens;
    uint8_t byte;
    std::string string;
    bool escape_mode = false;
    for(size_t i = 0; i < file_size; ++i) {
        read(ifs, byte);
        if(byte == 0x1a) {
            break;
        }
        switch(byte) {
        case '@':
            if(escape_mode) {
                pc_board_tokens.push(string);
                string.clear();

                escape_mode = false;
            } else {
                escape_mode = true;
            }
            break;
        case 'X':
            if(escape_mode && string.empty()) {
                string += byte;

                read(ifs, byte);
                string += byte;

                read(ifs, byte);
                string += byte;

                pc_board_tokens.push(string);
                string.clear();

                escape_mode = false;
                i += 2;
            }
            break;
        default:
            if(escape_mode) {
                string += byte;
            } else {
                pc_board_tokens.push(byte);
            }
        }
    }

    if(escape_mode) {
        throw std::exception();
    }

    return std::move(pc_board_tokens);
}

inline bool is_hex(const char& character)
{
    return (character >= '0' && character <= '9') || (character >= 'A' && character <= 'F');
}

inline uint8_t from_hex(const char& character)
{
    if(character >= '0' && character <= '9') {
        return character - '0';
    } else {
        return character - 'A' + 10;
    }
}

image_data_t read_pc_board_file(std::ifstream& ifs, const size_t& file_size, const size_t& columns)
{
    ansi_screen_t screen(columns == 0 ? 80 : columns);
    auto pc_board_tokens = tokenize_pc_board_file(ifs, file_size);
    size_t lit_pos = 0;
    size_t seq_pos = 0;
    for(const auto& type:pc_board_tokens.types) {
        if(type == pc_board_tokens_t::type_t::literal) {
            const auto& literal = pc_board_tokens.literals[lit_pos++];
            switch(literal) {
            case '\n':
                screen.new_line();
                break;
            case '\r':
                break;
            default:
                screen.putc(literal);
                break;
            }
        } else {
            auto& string = pc_board_tokens.pc_board_sequences[seq_pos++];
            if((string.size() == 3) && (string[0] == 'X') && is_hex(string[1]) && is_hex(string[2])) {
                screen.bg = from_hex(string[1]);
                screen.fg = from_hex(string[2]);
            } else if(string == "CLS") {
                screen.clear_screen();
            }
        }
    }

    return screen.get_image_data();
}

pc_board_t::pc_board_t(std::ifstream& ifs)
    : textmode_t(ifs)
{
    image_data = read_pc_board_file(ifs, sauce.file_size, size_t(sauce.columns));
    image_data.palette = create_binary_text_palette();
    options.palette_type = palette_type_t::binary_text;
    type = textmode_type_t::pc_board;
}
