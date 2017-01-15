#include "../textmode.h"
#include "ansi.h"
#include "screen.h"
#include "../utf8.h"
#include "palettes.h"

class ansi_escape_sequence_t
{
private:
    bool no_value = true;
public:
    enum class type_t {up, down, right, left, move, erase_display, erase_line, sgr, save_pos, true_color, restore_pos, unknown};

    std::vector<size_t> values;
    type_t type;

    void append_numeric(const uint8_t&);
    bool has_no_value();
    void set_defaults(const uint8_t& default_value, const uint8_t& limit_size);
    void set_default(const uint8_t&);
    void clear_values();
    void push();
    size_t size();
};

void ansi_escape_sequence_t::append_numeric(const uint8_t& ascii_code)
{
    if(no_value) {
        values.push_back(size_t(0));
        no_value = false;
    }
    size_t& value = values.back();
    value *= 10;
    value += ascii_code - '0';

}

bool ansi_escape_sequence_t::has_no_value()
{
    return no_value;
}

void ansi_escape_sequence_t::set_defaults(const uint8_t& default_value, const uint8_t& size_limit)
{
    size_t length = values.size();
    if(length < size_limit) {
        do {
            values.push_back(default_value);
        } while(++length < size_limit);
    } else if(length > size_limit) {
        values.resize(size_limit);
    }
}

void ansi_escape_sequence_t::set_default(const uint8_t& default_value)
{
    if(values.empty()) {
        values.push_back(default_value);
    }
}

void ansi_escape_sequence_t::clear_values()
{
    values.clear();
}

void ansi_escape_sequence_t::push()
{
    no_value = true;
}

size_t ansi_escape_sequence_t::size()
{
    return values.size();
}

class ansi_tokens_t
{
public:
    enum class type_t {ansi_escape_sequence, literal};

    std::vector<type_t> types;
    std::vector<ansi_escape_sequence_t> ansi_escape_sequences;
    std::vector<uint8_t> literals;

    void push(const ansi_escape_sequence_t&);
    void push(const uint8_t&);
};

inline void ansi_tokens_t::push(const ansi_escape_sequence_t& ansi_escape_sequence)
{
    types.push_back(type_t::ansi_escape_sequence);
    ansi_escape_sequences.push_back(ansi_escape_sequence);
}

inline void ansi_tokens_t::push(const uint8_t& ascii_code)
{
    types.push_back(type_t::literal);
    literals.push_back(ascii_code);
}

inline bool is_numeric(const uint8_t& ascii_code)
{
    return ascii_code >= '0' && ascii_code <= '9';
}

inline bool is_escape_code_terminator(const uint8_t& ascii_code)
{
    return ascii_code >= '@' && ascii_code <= '~';
}

ansi_tokens_t tokenize_ansi_file(file_t& file, const size_t& file_size)
{
    ansi_tokens_t ansi_tokens;
    uint8_t byte;
    ansi_escape_sequence_t ansi_escape_sequence;
    bool pre_escape_mode = false;
    bool escape_mode = false;
    for(size_t i = 0; i < file_size; ++i) {
        byte = file.read_byte();
        if(escape_mode) {
            if(is_numeric(byte)) {
                ansi_escape_sequence.append_numeric(byte);
            } else if(byte == ';' || byte == ':') {
                if(ansi_escape_sequence.has_no_value()) {
                    ansi_escape_sequence.values.push_back(1);
                }
                ansi_escape_sequence.push();
            } else if(is_escape_code_terminator(byte)) {
                switch(byte) {
                case 'A':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::up;
                    ansi_escape_sequence.set_defaults(1, 1);
                    break;
                case 'B':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::down;
                    ansi_escape_sequence.set_defaults(1, 1);
                    break;
                case 'C':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::right;
                    ansi_escape_sequence.set_defaults(1, 1);
                    break;
                case 'D':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::left;
                    ansi_escape_sequence.set_defaults(1, 1);
                    break;
                case 'H':
                case 'f':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::move;
                    ansi_escape_sequence.set_defaults(1, 2);
                    break;
                case 'J':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::erase_display;
                    ansi_escape_sequence.set_defaults(0, 1);
                    break;
                case 'K':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::erase_line;
                    ansi_escape_sequence.set_defaults(0, 1);
                    break;
                case 'm':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::sgr;
                    ansi_escape_sequence.set_default(0);
                    break;
                case 's':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::save_pos;
                    ansi_escape_sequence.clear_values();
                    break;
                case 't':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::true_color;
                    break;
                case 'u':
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::restore_pos;
                    ansi_escape_sequence.clear_values();
                    break;
                default:
                    ansi_escape_sequence.type = ansi_escape_sequence_t::type_t::unknown;
                    ansi_escape_sequence.clear_values();
                }
                escape_mode = false;
                ansi_tokens.push(ansi_escape_sequence);
                ansi_escape_sequence = ansi_escape_sequence_t();
            }
        } else {
            if(byte == '\e' && !pre_escape_mode) {
                pre_escape_mode = true;
            } else if(byte == '[' && pre_escape_mode) {
                pre_escape_mode = false;
                escape_mode = true;
            } else {
                pre_escape_mode = false;
                ansi_tokens.push(byte);
            }
        }
    }
    return ansi_tokens;
}

inline void copy_rgb_values(rgb_t& rgb, const std::vector<size_t>& values)
{
    rgb.red = values[1];
    rgb.green = values[2];
    rgb.blue = values[3];
}

bool has_24_bit_colors(image_data_t& image_data)
{
    for(size_t y = 0, i = 0; y < image_data.rows; ++y) {
        for(size_t x = 0; x < image_data.columns; ++x, ++i) {
            block_t& block = image_data.data[i];
            if(block.attr.fg_rgb_mode || block.attr.bg_rgb_mode) {
                return true;
            }
        }
    }
    return false;
}

image_data_t read_ansi_file(file_t& file, const size_t& file_size, const size_t& columns, palette_type_t& palette_type)
{
    ansi_screen_t screen(columns == 0 ? 80 : columns);
    auto ansi_tokens = tokenize_ansi_file(file, file_size);
    size_t lit_pos = 0;
    size_t esc_pos = 0;
    for(const auto& type:ansi_tokens.types) {
        if(type == ansi_tokens_t::type_t::literal) {
            const auto& literal = ansi_tokens.literals[lit_pos++];
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
            auto& ansi_escape_sequence = ansi_tokens.ansi_escape_sequences[esc_pos++];
            switch(ansi_escape_sequence.type) {
            case ansi_escape_sequence_t::type_t::up:
                screen.up(ansi_escape_sequence.values[0]);
                break;
            case ansi_escape_sequence_t::type_t::down:
                screen.down(ansi_escape_sequence.values[0]);
                break;
            case ansi_escape_sequence_t::type_t::right:
                screen.right(ansi_escape_sequence.values[0]);
                break;
            case ansi_escape_sequence_t::type_t::left:
                screen.left(ansi_escape_sequence.values[0]);
                break;
            case ansi_escape_sequence_t::type_t::move:
                screen.move(ansi_escape_sequence.values[1], ansi_escape_sequence.values[0]);
                break;
            case ansi_escape_sequence_t::type_t::erase_display:
                switch(ansi_escape_sequence.values[0]) {
                case 0:
                    screen.clear_until_end_of_screen();
                    break;
                case 1:
                    screen.clear_from_start_of_screen();
                    break;
                case 2:
                    screen.clear_screen();
                    break;
                }
                break;
            case ansi_escape_sequence_t::type_t::erase_line:
                switch(ansi_escape_sequence.values[0]) {
                case 0:
                    screen.clear_until_end_of_line();
                    break;
                case 1:
                    screen.clear_from_start_of_line();
                    break;
                case 2:
                    screen.clear_line();
                    break;
                }
                break;
            case ansi_escape_sequence_t::type_t::sgr:
                for(const auto& value:ansi_escape_sequence.values) {
                    if(value >= 30 && value <= 37) {
                        screen.fg = value - 30;
                        screen.fg_rgb_mode = false;
                    } else if(value >= 40 && value <= 47) {
                        screen.bg = value - 40;
                        screen.bg_rgb_mode = false;
                    } else {
                        switch(value) {
                        case 0:
                            screen.reset_attributes();
                            break;
                        case 1:
                            screen.bold = true;
                            screen.fg_rgb_mode = false;
                            break;
                        case 5:
                            screen.blink = true;
                            break;
                        case 7:
                            screen.inverse = true;
                            break;
                        case 22:
                            screen.bold = false;
                            break;
                        case 25:
                            screen.blink = false;
                            break;
                        case 27:
                            screen.inverse = false;
                            break;
                        }
                    }
                }
                break;
            case ansi_escape_sequence_t::type_t::save_pos:
                screen.save_position();
                break;
            case ansi_escape_sequence_t::type_t::true_color:
                if(ansi_escape_sequence.size() >= 4) {
                    switch(ansi_escape_sequence.values[0]) {
                    case 0:
                        screen.bg_rgb_mode = true;
                        copy_rgb_values(screen.bg_rgb, ansi_escape_sequence.values);
                        break;
                    case 1:
                        screen.fg_rgb_mode = true;
                        copy_rgb_values(screen.fg_rgb, ansi_escape_sequence.values);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case ansi_escape_sequence_t::type_t::restore_pos:
                screen.restore_position();
                break;
            case ansi_escape_sequence_t::type_t::unknown:
                break;
            }
        }
    }

    {
        image_data_t image_data = screen.get_image_data();
        image_data.palette = create_ansi_palette();
        if(has_24_bit_colors(image_data)) {
            palette_type = palette_type_t::ansi_with_truecolor;
        } else {
            palette_type = palette_type_t::ansi;
        }
        return image_data;
    }
}

ansi_t::ansi_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_ansi_file(file, sauce.file_size, size_t(sauce.columns), options.palette_type);
    type = textmode_type_t::ansi;
}
