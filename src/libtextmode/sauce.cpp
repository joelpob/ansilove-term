#include "sauce.h"
#include "utf8.h"
#include <iomanip>
#include <cstring>
#include <algorithm>
#include "file.h"

class sauce_data_t
{
public:
    sauce_data_t();
    sauce_data_t(const std::string&);

    bool exists;
    size_t actual_file_size;
    std::vector<uint8_t> version, title, author, group, year, month, day;
    uint32_t file_size;
    uint8_t data_type, file_type;
    uint16_t t_info_1, t_info_2, t_info_3, t_info_4;
    uint8_t comment_lines, t_flags;
    std::vector<uint8_t> t_info_s;
    std::vector<std::vector<uint8_t>> comments;
};

bool has_sauce_header(file_t& file)
{
    file.seek_from_end(128);
    return file.read_string(5) == "SAUCE";
}

bool has_comment_header(file_t& file, const uint8_t& comment_lines)
{
    file.seek_from_end(128 + 5 + 64 * comment_lines);
    return file.read_string(5) == "COMNT";
}

size_t get_file_size(file_t& file)
{
    file.seek_from_end(0);
    return file.tell();
}

size_t get_file_size(file_t& file, const uint8_t& comment_lines)
{
    size_t sauce_size = (comment_lines > 0) ? 5 + 64 * comment_lines + 129 : 129;
    file.seek_from_end(sauce_size);
    return file.tell();
}

sauce_data_t::sauce_data_t()
    : version(2), title(35), author(20), group(20), year(4), month(2), day(2), t_info_s(22)
{
    exists = false;
}

sauce_data_t::sauce_data_t(const std::string& filename)
    : sauce_data_t()
{
    file_t file(filename);
    try {
        exists = has_sauce_header(file);
        if(!exists) {
            actual_file_size = get_file_size(file);
        } else {
            file.read_bytes(version);
            file.read_bytes(title);
            file.read_bytes(author);
            file.read_bytes(group);
            file.read_bytes(year);
            file.read_bytes(month);
            file.read_bytes(day);
            file_size = file.read_32_bit_word();
            data_type = file.read_byte();
            file_type = file.read_byte();
            t_info_1 = file.read_16_bit_word();
            t_info_2 = file.read_16_bit_word();
            t_info_3 = file.read_16_bit_word();
            t_info_4 = file.read_16_bit_word();
            comment_lines = file.read_byte();
            t_flags = file.read_byte();
            file.read_bytes(t_info_s);
            if(comment_lines > 0 && has_comment_header(file, comment_lines)) {
                for(size_t i = 0; i < comment_lines; ++i) {
                    std::vector<uint8_t> comment(64);
                    file.read_bytes(comment);
                    comments.push_back(std::move(comment));
                }
            }
            actual_file_size = get_file_size(file, comment_lines);
        }
    } catch(std::exception e) {
        exists = false;
        actual_file_size = get_file_size(file);
    }
}

std::string ascii_to_string(const std::vector<uint8_t>& vector)
{
    std::string string;
    for(auto& i:vector) {
        string += char(i);
    }
    return string;
}

data_type_t get_data_type(const uint8_t& data_type_value)
{
    switch(data_type_value) {
    case 0:
        return data_type_t::undefined;
    case 1:
        return data_type_t::character;
    case 2:
        return data_type_t::bitmap;
    case 3:
        return data_type_t::vector;
    case 4:
        return data_type_t::audio;
    case 5:
        return data_type_t::binary_text;
    case 6:
        return data_type_t::xbin;
    case 7:
        return data_type_t::archive;
    case 8:
        return data_type_t::executable;
    default:
        return data_type_t::invalid;
    }
}

file_type_t get_file_type(const data_type_t& data_type, const uint8_t& file_type_value)
{
    switch(data_type) {
    case data_type_t::undefined:
        return file_type_t::undefined;
    case data_type_t::character:
        switch(file_type_value) {
        case 0:
            return file_type_t::ascii;
        case 1:
            return file_type_t::ansi;
        case 2:
            return file_type_t::ansimation;
        case 3:
            return file_type_t::rip_script;
        case 4:
            return file_type_t::pcboard;
        case 5:
            return file_type_t::avatar;
        case 6:
            return file_type_t::html;
        case 7:
            return file_type_t::source;
        case 8:
            return file_type_t::tundradraw;
        default:
            return file_type_t::invalid;
        }
    case data_type_t::bitmap:
        switch(file_type_value) {
        case 0:
            return file_type_t::gif;
        case 1:
            return file_type_t::pcx;
        case 2:
            return file_type_t::lbm_iff;
        case 3:
            return file_type_t::tga;
        case 4:
            return file_type_t::fli;
        case 5:
            return file_type_t::flc;
        case 6:
            return file_type_t::bmp;
        case 7:
            return file_type_t::gl;
        case 8:
            return file_type_t::dl;
        case 9:
            return file_type_t::wpg;
        case 10:
            return file_type_t::png;
        case 11:
            return file_type_t::jpg;
        case 12:
            return file_type_t::mpg;
        case 13:
            return file_type_t::avi;
        default:
            return file_type_t::invalid;
        }
    case data_type_t::vector:
        switch(file_type_value) {
        case 0:
            return file_type_t::dxf;
        case 1:
            return file_type_t::dwg;
        case 2:
            return file_type_t::wpg;
        case 3:
            return file_type_t::t_3ds;
        default:
            return file_type_t::invalid;
        }
    case data_type_t::audio:
        switch(file_type_value) {
        case 0:
            return file_type_t::mod;
        case 1:
            return file_type_t::t_699;
        case 2:
            return file_type_t::stm;
        case 3:
            return file_type_t::s3m;
        case 4:
            return file_type_t::mtm;
        case 5:
            return file_type_t::far;
        case 6:
            return file_type_t::ult;
        case 7:
            return file_type_t::amf;
        case 8:
            return file_type_t::dmf;
        case 9:
            return file_type_t::okt;
        case 10:
            return file_type_t::rol;
        case 11:
            return file_type_t::cmf;
        case 12:
            return file_type_t::mid;
        case 13:
            return file_type_t::sadt;
        case 14:
            return file_type_t::voc;
        case 15:
            return file_type_t::wav;
        case 16:
            return file_type_t::smp8;
        case 17:
            return file_type_t::smp8s;
        case 18:
            return file_type_t::smp16;
        case 19:
            return file_type_t::smp16s;
        case 20:
            return file_type_t::patch8;
        case 21:
            return file_type_t::patch16;
        case 22:
            return file_type_t::xm;
        case 23:
            return file_type_t::hsc;
        case 24:
            return file_type_t::it;
        default:
            return file_type_t::invalid;
        }
    case data_type_t::binary_text:
        return file_type_t::undefined;
    case data_type_t::xbin:
        return file_type_t::undefined;
    case data_type_t::archive:
        switch(file_type_value) {
        case 0:
            return file_type_t::zip;
        case 1:
            return file_type_t::arj;
        case 2:
            return file_type_t::lzh;
        case 3:
            return file_type_t::arc;
        case 4:
            return file_type_t::tar;
        case 5:
            return file_type_t::zoo;
        case 6:
            return file_type_t::rar;
        case 7:
            return file_type_t::uc2;
        case 8:
            return file_type_t::pak;
        case 9:
            return file_type_t::sqz;
        default:
            return file_type_t::invalid;
        }
    case data_type_t::executable:
        return file_type_t::undefined;
    case data_type_t::invalid:
        return file_type_t::invalid;
    }
}

inline bool font_test(std::string& t_info_s, const char* preset_name)
{
    return strcasecmp(t_info_s.c_str(), preset_name) == 0;
}

font_type_t get_font_type(std::string& font_name)
{
    if(font_test(font_name, "")) {
        return font_type_t::undefined;
    } else if(font_test(font_name, "IBM VGA")) {
        font_name = "IBM VGA";
        return font_type_t::ibm_vga;
    } else if(font_test(font_name, "IBM VGA50")) {
        font_name = "IBM VGA50";
        return font_type_t::ibm_vga50;
    } else if(font_test(font_name, "IBM VGA25G")) {
        font_name = "IBM VGA25G";
        return font_type_t::ibm_vga25g;
    } else if(font_test(font_name, "IBM EGA")) {
        font_name = "IBM EGA";
        return font_type_t::ibm_ega;
    } else if(font_test(font_name, "IBM EGA43")) {
        font_name = "IBM EGA43";
        return font_type_t::ibm_ega43;
    } else if(font_test(font_name, "IBM VGA 437")) {
        font_name = "IBM VGA 437";
        return font_type_t::ibm_vga_437;
    } else if(font_test(font_name, "IBM VGA 720")) {
        font_name = "IBM VGA 720";
        return font_type_t::ibm_vga_720;
    } else if(font_test(font_name, "IBM VGA 737")) {
        font_name = "IBM VGA 737";
        return font_type_t::ibm_vga_737;
    } else if(font_test(font_name, "IBM VGA 775")) {
        font_name = "IBM VGA 775";
        return font_type_t::ibm_vga_775;
    } else if(font_test(font_name, "IBM VGA 819")) {
        font_name = "IBM VGA 819";
        return font_type_t::ibm_vga_819;
    } else if(font_test(font_name, "IBM VGA 850")) {
        font_name = "IBM VGA 850";
        return font_type_t::ibm_vga_850;
    } else if(font_test(font_name, "IBM VGA 852")) {
        font_name = "IBM VGA 852";
        return font_type_t::ibm_vga_852;
    } else if(font_test(font_name, "IBM VGA 855")) {
        font_name = "IBM VGA 855";
        return font_type_t::ibm_vga_855;
    } else if(font_test(font_name, "IBM VGA 857")) {
        font_name = "IBM VGA 857";
        return font_type_t::ibm_vga_857;
    } else if(font_test(font_name, "IBM VGA 858")) {
        font_name = "IBM VGA 858";
        return font_type_t::ibm_vga_858;
    } else if(font_test(font_name, "IBM VGA 860")) {
        font_name = "IBM VGA 860";
        return font_type_t::ibm_vga_860;
    } else if(font_test(font_name, "IBM VGA 861")) {
        font_name = "IBM VGA 861";
        return font_type_t::ibm_vga_861;
    } else if(font_test(font_name, "IBM VGA 862")) {
        font_name = "IBM VGA 862";
        return font_type_t::ibm_vga_862;
    } else if(font_test(font_name, "IBM VGA 863")) {
        font_name = "IBM VGA 863";
        return font_type_t::ibm_vga_863;
    } else if(font_test(font_name, "IBM VGA 864")) {
        font_name = "IBM VGA 864";
        return font_type_t::ibm_vga_864;
    } else if(font_test(font_name, "IBM VGA 865")) {
        font_name = "IBM VGA 865";
        return font_type_t::ibm_vga_865;
    } else if(font_test(font_name, "IBM VGA 866")) {
        font_name = "IBM VGA 866";
        return font_type_t::ibm_vga_866;
    } else if(font_test(font_name, "IBM VGA 869")) {
        font_name = "IBM VGA 869";
        return font_type_t::ibm_vga_869;
    } else if(font_test(font_name, "IBM VGA 872")) {
        font_name = "IBM VGA 872";
        return font_type_t::ibm_vga_872;
    } else if(font_test(font_name, "IBM VGA KAM")) {
        font_name = "IBM VGA KAM";
        return font_type_t::ibm_vga_kam;
    } else if(font_test(font_name, "IBM VGA MAZ")) {
        font_name = "IBM VGA MAZ";
        return font_type_t::ibm_vga_maz;
    } else if(font_test(font_name, "IBM VGA MIK")) {
        font_name = "IBM VGA MIK";
        return font_type_t::ibm_vga_mik;
    } else if(font_test(font_name, "IBM VGA50 437")) {
        font_name = "IBM VGA50 437";
        return font_type_t::ibm_vga50_437;
    } else if(font_test(font_name, "IBM VGA50 720")) {
        font_name = "IBM VGA50 720";
        return font_type_t::ibm_vga50_720;
    } else if(font_test(font_name, "IBM VGA50 737")) {
        font_name = "IBM VGA50 737";
        return font_type_t::ibm_vga50_737;
    } else if(font_test(font_name, "IBM VGA50 775")) {
        font_name = "IBM VGA50 775";
        return font_type_t::ibm_vga50_775;
    } else if(font_test(font_name, "IBM VGA50 819")) {
        font_name = "IBM VGA50 819";
        return font_type_t::ibm_vga50_819;
    } else if(font_test(font_name, "IBM VGA50 850")) {
        font_name = "IBM VGA50 850";
        return font_type_t::ibm_vga50_850;
    } else if(font_test(font_name, "IBM VGA50 852")) {
        font_name = "IBM VGA50 852";
        return font_type_t::ibm_vga50_852;
    } else if(font_test(font_name, "IBM VGA50 855")) {
        font_name = "IBM VGA50 855";
        return font_type_t::ibm_vga50_855;
    } else if(font_test(font_name, "IBM VGA50 857")) {
        font_name = "IBM VGA50 857";
        return font_type_t::ibm_vga50_857;
    } else if(font_test(font_name, "IBM VGA50 858")) {
        font_name = "IBM VGA50 858";
        return font_type_t::ibm_vga50_858;
    } else if(font_test(font_name, "IBM VGA50 860")) {
        font_name = "IBM VGA50 860";
        return font_type_t::ibm_vga50_860;
    } else if(font_test(font_name, "IBM VGA50 861")) {
        font_name = "IBM VGA50 861";
        return font_type_t::ibm_vga50_861;
    } else if(font_test(font_name, "IBM VGA50 862")) {
        font_name = "IBM VGA50 862";
        return font_type_t::ibm_vga50_862;
    } else if(font_test(font_name, "IBM VGA50 863")) {
        font_name = "IBM VGA50 863";
        return font_type_t::ibm_vga50_863;
    } else if(font_test(font_name, "IBM VGA50 864")) {
        font_name = "IBM VGA50 864";
        return font_type_t::ibm_vga50_864;
    } else if(font_test(font_name, "IBM VGA50 865")) {
        font_name = "IBM VGA50 865";
        return font_type_t::ibm_vga50_865;
    } else if(font_test(font_name, "IBM VGA50 866")) {
        font_name = "IBM VGA50 866";
        return font_type_t::ibm_vga50_866;
    } else if(font_test(font_name, "IBM VGA50 869")) {
        font_name = "IBM VGA50 869";
        return font_type_t::ibm_vga50_869;
    } else if(font_test(font_name, "IBM VGA50 872")) {
        font_name = "IBM VGA50 872";
        return font_type_t::ibm_vga50_872;
    } else if(font_test(font_name, "IBM VGA50 KAM")) {
        font_name = "IBM VGA50 KAM";
        return font_type_t::ibm_vga50_kam;
    } else if(font_test(font_name, "IBM VGA50 MAZ")) {
        font_name = "IBM VGA50 MAZ";
        return font_type_t::ibm_vga50_maz;
    } else if(font_test(font_name, "IBM VGA50 MIK")) {
        font_name = "IBM VGA50 MIK";
        return font_type_t::ibm_vga50_mik;
    } else if(font_test(font_name, "IBM VGA25G 437")) {
        font_name = "IBM VGA25G 437";
        return font_type_t::ibm_vga25g_437;
    } else if(font_test(font_name, "IBM VGA25G 720")) {
        font_name = "IBM VGA25G 720";
        return font_type_t::ibm_vga25g_720;
    } else if(font_test(font_name, "IBM VGA25G 737")) {
        font_name = "IBM VGA25G 737";
        return font_type_t::ibm_vga25g_737;
    } else if(font_test(font_name, "IBM VGA25G 775")) {
        font_name = "IBM VGA25G 775";
        return font_type_t::ibm_vga25g_775;
    } else if(font_test(font_name, "IBM VGA25G 819")) {
        font_name = "IBM VGA25G 819";
        return font_type_t::ibm_vga25g_819;
    } else if(font_test(font_name, "IBM VGA25G 850")) {
        font_name = "IBM VGA25G 850";
        return font_type_t::ibm_vga25g_850;
    } else if(font_test(font_name, "IBM VGA25G 852")) {
        font_name = "IBM VGA25G 852";
        return font_type_t::ibm_vga25g_852;
    } else if(font_test(font_name, "IBM VGA25G 855")) {
        font_name = "IBM VGA25G 855";
        return font_type_t::ibm_vga25g_855;
    } else if(font_test(font_name, "IBM VGA25G 857")) {
        font_name = "IBM VGA25G 857";
        return font_type_t::ibm_vga25g_857;
    } else if(font_test(font_name, "IBM VGA25G 858")) {
        font_name = "IBM VGA25G 858";
        return font_type_t::ibm_vga25g_858;
    } else if(font_test(font_name, "IBM VGA25G 860")) {
        font_name = "IBM VGA25G 860";
        return font_type_t::ibm_vga25g_860;
    } else if(font_test(font_name, "IBM VGA25G 861")) {
        font_name = "IBM VGA25G 861";
        return font_type_t::ibm_vga25g_861;
    } else if(font_test(font_name, "IBM VGA25G 862")) {
        font_name = "IBM VGA25G 862";
        return font_type_t::ibm_vga25g_862;
    } else if(font_test(font_name, "IBM VGA25G 863")) {
        font_name = "IBM VGA25G 863";
        return font_type_t::ibm_vga25g_863;
    } else if(font_test(font_name, "IBM VGA25G 864")) {
        font_name = "IBM VGA25G 864";
        return font_type_t::ibm_vga25g_864;
    } else if(font_test(font_name, "IBM VGA25G 865")) {
        font_name = "IBM VGA25G 865";
        return font_type_t::ibm_vga25g_865;
    } else if(font_test(font_name, "IBM VGA25G 866")) {
        font_name = "IBM VGA25G 866";
        return font_type_t::ibm_vga25g_866;
    } else if(font_test(font_name, "IBM VGA25G 869")) {
        font_name = "IBM VGA25G 869";
        return font_type_t::ibm_vga25g_869;
    } else if(font_test(font_name, "IBM VGA25G 872")) {
        font_name = "IBM VGA25G 872";
        return font_type_t::ibm_vga25g_872;
    } else if(font_test(font_name, "IBM VGA25G KAM")) {
        font_name = "IBM VGA25G KAM";
        return font_type_t::ibm_vga25g_kam;
    } else if(font_test(font_name, "IBM VGA25G MAZ")) {
        font_name = "IBM VGA25G MAZ";
        return font_type_t::ibm_vga25g_maz;
    } else if(font_test(font_name, "IBM VGA25G MIK")) {
        font_name = "IBM VGA25G MIK";
        return font_type_t::ibm_vga25g_mik;
    } else if(font_test(font_name, "IBM EGA 437")) {
        font_name = "IBM EGA 437";
        return font_type_t::ibm_ega_437;
    } else if(font_test(font_name, "IBM EGA 720")) {
        font_name = "IBM EGA 720";
        return font_type_t::ibm_ega_720;
    } else if(font_test(font_name, "IBM EGA 737")) {
        font_name = "IBM EGA 737";
        return font_type_t::ibm_ega_737;
    } else if(font_test(font_name, "IBM EGA 775")) {
        font_name = "IBM EGA 775";
        return font_type_t::ibm_ega_775;
    } else if(font_test(font_name, "IBM EGA 819")) {
        font_name = "IBM EGA 819";
        return font_type_t::ibm_ega_819;
    } else if(font_test(font_name, "IBM EGA 850")) {
        font_name = "IBM EGA 850";
        return font_type_t::ibm_ega_850;
    } else if(font_test(font_name, "IBM EGA 852")) {
        font_name = "IBM EGA 852";
        return font_type_t::ibm_ega_852;
    } else if(font_test(font_name, "IBM EGA 855")) {
        font_name = "IBM EGA 855";
        return font_type_t::ibm_ega_855;
    } else if(font_test(font_name, "IBM EGA 857")) {
        font_name = "IBM EGA 857";
        return font_type_t::ibm_ega_857;
    } else if(font_test(font_name, "IBM EGA 858")) {
        font_name = "IBM EGA 858";
        return font_type_t::ibm_ega_858;
    } else if(font_test(font_name, "IBM EGA 860")) {
        font_name = "IBM EGA 860";
        return font_type_t::ibm_ega_860;
    } else if(font_test(font_name, "IBM EGA 861")) {
        font_name = "IBM EGA 861";
        return font_type_t::ibm_ega_861;
    } else if(font_test(font_name, "IBM EGA 862")) {
        font_name = "IBM EGA 862";
        return font_type_t::ibm_ega_862;
    } else if(font_test(font_name, "IBM EGA 863")) {
        font_name = "IBM EGA 863";
        return font_type_t::ibm_ega_863;
    } else if(font_test(font_name, "IBM EGA 864")) {
        font_name = "IBM EGA 864";
        return font_type_t::ibm_ega_864;
    } else if(font_test(font_name, "IBM EGA 865")) {
        font_name = "IBM EGA 865";
        return font_type_t::ibm_ega_865;
    } else if(font_test(font_name, "IBM EGA 866")) {
        font_name = "IBM EGA 866";
        return font_type_t::ibm_ega_866;
    } else if(font_test(font_name, "IBM EGA 869")) {
        font_name = "IBM EGA 869";
        return font_type_t::ibm_ega_869;
    } else if(font_test(font_name, "IBM EGA 872")) {
        font_name = "IBM EGA 872";
        return font_type_t::ibm_ega_872;
    } else if(font_test(font_name, "IBM EGA KAM")) {
        font_name = "IBM EGA KAM";
        return font_type_t::ibm_ega_kam;
    } else if(font_test(font_name, "IBM EGA MAZ")) {
        font_name = "IBM EGA MAZ";
        return font_type_t::ibm_ega_maz;
    } else if(font_test(font_name, "IBM EGA MIK")) {
        font_name = "IBM EGA MIK";
        return font_type_t::ibm_ega_mik;
    } else if(font_test(font_name, "IBM EGA43 437")) {
        font_name = "IBM EGA43 437";
        return font_type_t::ibm_ega43_437;
    } else if(font_test(font_name, "IBM EGA43 720")) {
        font_name = "IBM EGA43 720";
        return font_type_t::ibm_ega43_720;
    } else if(font_test(font_name, "IBM EGA43 737")) {
        font_name = "IBM EGA43 737";
        return font_type_t::ibm_ega43_737;
    } else if(font_test(font_name, "IBM EGA43 775")) {
        font_name = "IBM EGA43 775";
        return font_type_t::ibm_ega43_775;
    } else if(font_test(font_name, "IBM EGA43 819")) {
        font_name = "IBM EGA43 819";
        return font_type_t::ibm_ega43_819;
    } else if(font_test(font_name, "IBM EGA43 850")) {
        font_name = "IBM EGA43 850";
        return font_type_t::ibm_ega43_850;
    } else if(font_test(font_name, "IBM EGA43 852")) {
        font_name = "IBM EGA43 852";
        return font_type_t::ibm_ega43_852;
    } else if(font_test(font_name, "IBM EGA43 855")) {
        font_name = "IBM EGA43 855";
        return font_type_t::ibm_ega43_855;
    } else if(font_test(font_name, "IBM EGA43 857")) {
        font_name = "IBM EGA43 857";
        return font_type_t::ibm_ega43_857;
    } else if(font_test(font_name, "IBM EGA43 858")) {
        font_name = "IBM EGA43 858";
        return font_type_t::ibm_ega43_858;
    } else if(font_test(font_name, "IBM EGA43 860")) {
        font_name = "IBM EGA43 860";
        return font_type_t::ibm_ega43_860;
    } else if(font_test(font_name, "IBM EGA43 861")) {
        font_name = "IBM EGA43 861";
        return font_type_t::ibm_ega43_861;
    } else if(font_test(font_name, "IBM EGA43 862")) {
        font_name = "IBM EGA43 862";
        return font_type_t::ibm_ega43_862;
    } else if(font_test(font_name, "IBM EGA43 863")) {
        font_name = "IBM EGA43 863";
        return font_type_t::ibm_ega43_863;
    } else if(font_test(font_name, "IBM EGA43 864")) {
        font_name = "IBM EGA43 864";
        return font_type_t::ibm_ega43_864;
    } else if(font_test(font_name, "IBM EGA43 865")) {
        font_name = "IBM EGA43 865";
        return font_type_t::ibm_ega43_865;
    } else if(font_test(font_name, "IBM EGA43 866")) {
        font_name = "IBM EGA43 866";
        return font_type_t::ibm_ega43_866;
    } else if(font_test(font_name, "IBM EGA43 869")) {
        font_name = "IBM EGA43 869";
        return font_type_t::ibm_ega43_869;
    } else if(font_test(font_name, "IBM EGA43 872")) {
        font_name = "IBM EGA43 872";
        return font_type_t::ibm_ega43_872;
    } else if(font_test(font_name, "IBM EGA43 KAM")) {
        font_name = "IBM EGA43 KAM";
        return font_type_t::ibm_ega43_kam;
    } else if(font_test(font_name, "IBM EGA43 MAZ")) {
        font_name = "IBM EGA43 MAZ";
        return font_type_t::ibm_ega43_maz;
    } else if(font_test(font_name, "IBM EGA43 MIK")) {
        font_name = "IBM EGA43 MIK";
        return font_type_t::ibm_ega43_mik;
    } else if(font_test(font_name, "Amiga Topaz 1")) {
        font_name = "Amiga Topaz 1";
        return font_type_t::amiga_topaz_1;
    } else if(font_test(font_name, "Amiga Topaz 1+")) {
        font_name = "Amiga Topaz 1+";
        return font_type_t::amiga_topaz_1_plus;
    } else if(font_test(font_name, "Amiga Topaz 2")) {
        font_name = "Amiga Topaz 2";
        return font_type_t::amiga_topaz_2;
    } else if(font_test(font_name, "Amiga Topaz 2+")) {
        font_name = "Amiga Topaz 2+";
        return font_type_t::amiga_topaz_2_plus;
    } else if(font_test(font_name, "Amiga P0T-NOoDLE")) {
        font_name = "Amiga P0T-NOoDLE";
        return font_type_t::amiga_p0t_noodle;
    } else if(font_test(font_name, "Amiga MicroKnight")) {
        font_name = "Amiga MicroKnight";
        return font_type_t::amiga_microknight;
    } else if(font_test(font_name, "Amiga MicroKnight+")) {
        font_name = "Amiga MicroKnight+";
        return font_type_t::amiga_microknight_plus;
    } else if(font_test(font_name, "Amiga mOsOul")) {
        font_name = "Amiga mOsOul";
        return font_type_t::amiga_mosoul;
    } else if(font_test(font_name, "C64 PETSCII unshifted")) {
        font_name = "C64 PETSCII unshifted";
        return font_type_t::c64_petscii_unshifted;
    } else if(font_test(font_name, "C64 PETSCII shifted")) {
        font_name = "C64 PETSCII shifted";
        return font_type_t::c64_petscii_shifted;
    } else if(font_test(font_name, "Atari ATASCII")) {
        font_name = "Atari ATASCII";
        return font_type_t::atari_atascii;
    }
    return font_type_t::invalid;
}

uint8_t is_not_a_space(const uint8_t& uint8)
{
    return uint8 != ' ';
}

std::vector<uint8_t>& trim(std::vector<uint8_t>& vector)
{
    vector.erase(vector.begin(), std::find_if(vector.begin(), vector.end(), is_not_a_space));
    vector.erase(std::find_if(vector.rbegin(), vector.rend(), is_not_a_space).base(), vector.end());
    return vector;
}

sauce_t::sauce_t(const std::string& filename)
{
    sauce_data_t data(filename);
    exists = data.exists;
    file_size = data.actual_file_size;
    if(exists) {
        version = cp_437_to_utf8_string(data.version);
        title = cp_437_to_utf8_string(trim(data.title));
        author = cp_437_to_utf8_string(trim(data.author));
        group = cp_437_to_utf8_string(trim(data.group));
        year = cp_437_to_utf8_string(data.year);
        month = cp_437_to_utf8_string(data.month);
        day = cp_437_to_utf8_string(data.day);
        data_type = get_data_type(data.data_type);
        if(data_type == data_type_t::binary_text) {
            columns = (data.file_type > 0) ? data.file_type * 2 : 160;
            file_type = file_type_t::undefined;
        } else {
            file_type = get_file_type(data_type, data.file_type);
        }
        switch(file_type) {
        case file_type_t::ascii:
        case file_type_t::ansi:
        case file_type_t::ansimation:
        case file_type_t::pcboard:
        case file_type_t::avatar:
        case file_type_t::tundradraw:
            columns = (data.t_info_1 > 0) ? data.t_info_1 : 80;
            if(file_type == file_type_t::ansimation && data.t_info_1 == 0) {
                rows = 25;
            } else {
                rows = data.t_info_2;
            }
            break;
        default:
            if(data_type != data_type_t::binary_text) {
                columns = 0;
            }
            rows = 0;
            break;
        }
        switch(file_type) {
        case file_type_t::rip_script:
        case file_type_t::gif:
        case file_type_t::pcx:
        case file_type_t::lbm_iff:
        case file_type_t::tga:
        case file_type_t::fli:
        case file_type_t::flc:
        case file_type_t::bmp:
        case file_type_t::gl:
        case file_type_t::dl:
        case file_type_t::wpg:
        case file_type_t::png:
        case file_type_t::jpg:
        case file_type_t::mpg:
        case file_type_t::avi:
            width = data.t_info_1;
            height = data.t_info_2;
            if(file_type == file_type_t::rip_script) {
                colors = depth = data.t_info_3;
                depth = 0;
            } else {
                depth = data.t_info_3;
                colors = 0;
            }
            break;
        default:
            width = 0;
            height = 0;
            colors = 0;
            depth = 0;
            break;
        }
        switch(file_type) {
        case file_type_t::smp8:
        case file_type_t::smp8s:
        case file_type_t::smp16:
        case file_type_t::smp16s:
            sample_rate = data.t_info_1;
            break;
        default:
            sample_rate = 0;
            break;
        }
        if(file_type == file_type_t::ascii || file_type == file_type_t::ansi || file_type == file_type_t::ansimation || data_type == data_type_t::binary_text) {
            switch((data.t_flags >> 0) & 1) {
            case 0:
                non_blink = non_blink_t::off;
                break;
            case 1:
                non_blink = non_blink_t::on;
                break;
            }
            switch((data.t_flags >> 1) & 3) {
            case 0:
                letter_space = letter_space_t::no_preference;
                break;
            case 1:
                letter_space = letter_space_t::eight_pixels;
                break;
            case 2:
                letter_space = letter_space_t::nine_pixels;
                break;
            default:
                letter_space = letter_space_t::invalid;
                break;
            }
            switch((data.t_flags >> 3) & 3) {
            case 0:
                aspect_ratio = aspect_ratio_t::no_preference;
                break;
            case 1:
                aspect_ratio = aspect_ratio_t::legacy_aspect;
                break;
            case 2:
                aspect_ratio = aspect_ratio_t::modern_aspect;
                break;
            default:
                aspect_ratio = aspect_ratio_t::invalid;
                break;
            }
        } else {
            non_blink = non_blink_t::undefined;
            letter_space = letter_space_t::undefined;
            aspect_ratio = aspect_ratio_t::undefined;
        }
        font_name = ascii_to_string(data.t_info_s);
        font_type = get_font_type(font_name);
        for(auto i:data.comments) {
            comments.push_back(cp_437_to_utf8_string(i));
        }
    }
}

template <typename T>
inline void remove_invalid_option(T& option)
{
    if(option == T::invalid) {
        option = T::undefined;
    }
}

template <typename T>
inline void set_default(T& option, const T& default_option)
{
    if(option == T::undefined) {
        option = default_option;
    }
}

options_t sauce_t::get_options()
{
    options_t options;
    options.non_blink = non_blink;
    options.letter_space = letter_space;
    options.aspect_ratio = aspect_ratio;
    options.font_type = font_type;
    remove_invalid_option(options.letter_space);
    remove_invalid_option(options.aspect_ratio);
    remove_invalid_option(options.font_type);
    set_default(options.non_blink, non_blink_t::off);
    set_default(options.letter_space, letter_space_t::eight_pixels);
    set_default(options.aspect_ratio, aspect_ratio_t::modern_aspect);
    set_default(options.font_type, font_type_t::ibm_vga);
    return options;
}

std::ostream& operator<<(std::ostream& ostream, const uint8_t& uint8)
{
    ostream << uint16_t(uint8);
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const std::vector<uint8_t>& vector)
{
    ostream << ascii_to_string(vector);
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const sauce_data_t& data)
{
    if(data.exists) {
        ostream << "    Version: " << data.version << std::endl;
        ostream << "      Title: " << data.title << std::endl;
        ostream << "     Author: " << data.author << std::endl;
        ostream << "      Group: " << data.group << std::endl;
        ostream << "       Date: " << data.year << data.month << data.day << std::endl;
        ostream << "   FileSize: " << data.file_size << std::endl;
        ostream << "   DataType: " << data.data_type << std::endl;
        ostream << "   FileType: " << data.file_type << std::endl;
        ostream << "     TInfo1: " << data.t_info_1 << std::endl;
        ostream << "     TInfo2: " << data.t_info_2 << std::endl;
        ostream << "     TInfo3: " << data.t_info_3 << std::endl;
        ostream << "     TInfo4: " << data.t_info_4 << std::endl;
        ostream << "   Comments: " << data.comment_lines << std::endl;
        ostream << "     TFlags: " << data.t_flags << std::endl;
        ostream << "     TInfoS: " << data.t_info_s << std::endl;
        if(data.comment_lines == data.comments.size()) {
            for(size_t i = 0; i < data.comment_lines; ++i) {
                ostream << "Comment ";
                ostream << std::right << std::setw(3) << std::setfill('0') << size_t(i + 1) << ": ";
                ostream << data.comments[i] << std::endl;
            }
        }
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const sauce_t& sauce)
{
    if(!sauce.exists) {
        ostream << "No SAUCE Record Found" << std::endl;
    } else {
        ostream << "     Version: " << sauce.version << std::endl;
        ostream << "       Title: " << sauce.title << std::endl;
        ostream << "      Author: " << sauce.author << std::endl;
        ostream << "       Group: " << sauce.group << std::endl;
        ostream << "        Date: ";
        ostream << sauce.year << "-" << sauce.month << "-" << sauce.day << std::endl;
        ostream << "   File Size: " << sauce.file_size << " bytes" << std::endl;
        ostream << "   Data Type: " << sauce.data_type << std::endl;
        switch(sauce.data_type) {
            case data_type_t::character:
            case data_type_t::bitmap:
            case data_type_t::vector:
            case data_type_t::audio:
            case data_type_t::archive:
                ostream << "   File Type: " << sauce.file_type << std::endl;
                break;
            default:
                break;
        }
        if(sauce.columns > 0) {
            ostream << "     Columns: " << sauce.columns << std::endl;
        }
        if(sauce.rows > 0) {
            ostream << "        Rows: " << sauce.rows << std::endl;
        }
        if(sauce.width > 0) {
            ostream << "       Width: " << sauce.width << std::endl;
        }
        if(sauce.height > 0) {
            ostream << "      Height: " << sauce.height << std::endl;
        }
        if(sauce.colors > 0) {
            ostream << "      Colors: " << sauce.colors << std::endl;
        }
        if(sauce.depth > 0) {
            ostream << "       Depth: " << sauce.depth << std::endl;
        }
        if(sauce.sample_rate > 0) {
            ostream << " Sample Rate: " << sauce.sample_rate << std::endl;
        }
        if(sauce.file_type == file_type_t::ascii || sauce.file_type == file_type_t::ansi || sauce.file_type == file_type_t::ansimation || sauce.data_type == data_type_t::binary_text) {
            ostream << "   Non-Blink: " << sauce.non_blink << std::endl;
            ostream << "Letter Space: " << sauce.letter_space << std::endl;
            ostream << "Aspect Ratio: " << sauce.aspect_ratio << std::endl;
            if(sauce.font_type == font_type_t::invalid) {
                ostream << "   Font Name: " << sauce.font_type;
                // ostream << " (" << sauce.font_name << ")" << std::endl;
            } else {
                ostream << "   Font Name: " << sauce.font_type << std::endl;
            }
        }
        {
            size_t comment_length = sauce.comments.size();
            for(size_t i = 0; i < comment_length; ++i) {
                ostream << " Comment ";
                ostream << std::right << std::setw(3) << std::setfill('0') << size_t(i + 1) << ": ";
                ostream << sauce.comments[i] << std::endl;
            }
        }
    }
    return ostream;
}
