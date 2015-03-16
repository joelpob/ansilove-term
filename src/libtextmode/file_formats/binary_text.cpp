#include "../textmode.h"
#include "binary_text.h"
#include "screen.h"
#include "palettes.h"

image_data_t read_binary_text_file(file_t& file, const size_t& file_size, const size_t& columns)
{
    screen_t screen(columns == 0 ? 160 : columns);

    {
        uint8_t code, attr;
        for(size_t i = 0; i < file_size; i += 2) {
            code = file.read_byte();
            attr = file.read_byte();
            screen.use_packed_attribute(attr);
            screen.putc(code);
        }
    }

    return screen.get_image_data();
}

binary_text_t::binary_text_t(const std::string& filename)
    : textmode_t(filename)
{
    file_t file(filename);
    image_data = read_binary_text_file(file, sauce.file_size, size_t(sauce.columns));
    image_data.palette = create_binary_text_palette();
    options.palette_type = palette_type_t::binary_text;
    type = textmode_type_t::binary_text;
}
