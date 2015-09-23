#include "textmode.h"
#include "file_formats/ansi.h"
#include "file_formats/ansiedit.h"
#include "file_formats/artworx.h"
#include "file_formats/ascii.h"
#include "file_formats/binary_text.h"
#include "file_formats/ice_draw.h"
#include "file_formats/pc_board.h"
#include "file_formats/tundra_draw.h"
#include "file_formats/xbin.h"
#include <cstring>

textmode_t::textmode_t(const std::string& filename)
    : sauce(filename)
{
    this->filename = filename;
    options = sauce.get_options();

    if(!sauce.title.empty()) {
        title = sauce.title;
    }

    if(!sauce.author.empty()) {
        author = sauce.author;
    }

    if(!sauce.group.empty()) {
        group = sauce.group;
    }
}

inline bool extension_test(std::string& extension, const char* extension_suffix)
{
    return strcasecmp(extension.c_str(), extension_suffix) == 0;
}

textmode_type_t check_extension(const std::string& filename)
{
    size_t pos = filename.rfind(".");
    if(pos == std::string::npos) {
        return textmode_type_t::undefined;
    }
    std::string extension = filename.substr(pos + 1);
    if(extension_test(extension, "ans")) {
        return textmode_type_t::ansi;
    } else if(extension_test(extension, "ansiedit")) {
        return textmode_type_t::ansiedit;
    } else if(extension_test(extension, "adf")) {
        return textmode_type_t::artworx;
    } else if(extension_test(extension, "asc") || 
                extension_test(extension, "diz") ||
                    extension_test(extension, "nfo") ||
                        extension_test(extension, "txt")) {
        return textmode_type_t::ascii;
    } else if(extension_test(extension, "bin")) {
        return textmode_type_t::binary_text;
    } else if(extension_test(extension, "idf")) {
        return textmode_type_t::ice_draw;
    } else if(extension_test(extension, "pcb")) {
        return textmode_type_t::pc_board;
    } else if(extension_test(extension, "tnd")) {
        return textmode_type_t::tundra_draw;
    } else if(extension_test(extension, "xb")) {
        return textmode_type_t::xbin;
    }
    return textmode_type_t::undefined;
}

textmode_t load_artwork(std::string filename)
{
    auto textmode_type = check_extension(filename);
    switch(textmode_type) {
    case textmode_type_t::ansiedit:
        return ansiedit_t(filename);
    case textmode_type_t::artworx:
        return artworx_t(filename);
    case textmode_type_t::binary_text:
        return binary_text_t(filename);
    case textmode_type_t::ice_draw:
        return ice_draw_t(filename);
    case textmode_type_t::pc_board:
        return pc_board_t(filename);
    case textmode_type_t::tundra_draw:
        return tundra_draw_t(filename);
    case textmode_type_t::xbin:
        return xbin_t(filename);
    case textmode_type_t::ansi:
        return ansi_t(filename);
    case textmode_type_t::ascii:
        return ascii_t(filename);
    case textmode_type_t::undefined:
    default:
        throw file_format_not_recognized_t();
    }
}

std::ostream& operator<<(std::ostream& ostream, const textmode_type_t& type)
{
    switch(type) {
    case textmode_type_t::ansi:
        ostream << "ANSi";
        break;
    case textmode_type_t::ansiedit:
        ostream << "AnsiEdit";
        break;
    case textmode_type_t::artworx:
        ostream << "Artworx";
        break;
    case textmode_type_t::ascii:
        ostream << "ASCii (8-Bit)";
        break;
    case textmode_type_t::binary_text:
        ostream << "Binary Text";
        break;
    case textmode_type_t::ice_draw:
        ostream << "Ice Draw";
        break;
    case textmode_type_t::pc_board:
        ostream << "PC Board";
        break;
    case textmode_type_t::tundra_draw:
        ostream << "Tundra Draw";
        break;
    case textmode_type_t::xbin:
        ostream << "XBIN";
        break;
    case textmode_type_t::undefined:
        ostream << "Undefined";
        break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, textmode_t& artwork)
{
    ostream << "     Columns: " << artwork.image_data.columns << std::endl;
    ostream << "        Rows: " << artwork.image_data.rows << std::endl;
    ostream << "        Type: " << artwork.type << std::endl;
    ostream << "       Title: " << artwork.title << std::endl;
    ostream << "      Author: " << artwork.author << std::endl;
    ostream << "       Group: " << artwork.group << std::endl;
    return ostream;
}
