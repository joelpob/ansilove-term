#include "options.h"

std::ostream& operator<<(std::ostream& ostream, const data_type_t& data_type)
{
    switch(data_type) {
        case data_type_t::character:
            ostream << "Character";
            break;
        case data_type_t::bitmap:
            ostream << "Bitmap";
            break;
        case data_type_t::vector:
            ostream << "Vector";
            break;
        case data_type_t::audio:
            ostream << "Audio";
            break;
        case data_type_t::binary_text:
            ostream << "Binary Text";
            break;
        case data_type_t::xbin:
            ostream << "XBin";
            break;
        case data_type_t::archive:
            ostream << "Archive";
            break;
        case data_type_t::executable:
            ostream << "Executable";
            break;
        case data_type_t::invalid:
            ostream << "Invalid";
            break;
        case data_type_t::undefined:
            ostream << "Undefined";
            break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const file_type_t& file_type)
{
    switch(file_type) {
        case file_type_t::ascii:
            ostream << "ASCII";
            break;
        case file_type_t::ansi:
            ostream << "ANSi";
            break;
        case file_type_t::ansimation:
            ostream << "ANSiMation";
            break;
        case file_type_t::rip_script:
            ostream << "RIP script";
            break;
        case file_type_t::pcboard:
            ostream << "PCBoard";
            break;
        case file_type_t::avatar:
            ostream << "Avatar";
            break;
        case file_type_t::html:
            ostream << "HTML";
            break;
        case file_type_t::source:
            ostream << "Source";
            break;
        case file_type_t::tundradraw:
            ostream << "TundraDraw";
            break;
        case file_type_t::gif:
            ostream << "GIF";
            break;
        case file_type_t::pcx:
            ostream << "PCX";
            break;
        case file_type_t::lbm_iff:
            ostream << "LBM/IFF";
            break;
        case file_type_t::tga:
            ostream << "TGA";
            break;
        case file_type_t::fli:
            ostream << "FLI";
            break;
        case file_type_t::flc:
            ostream << "FLC";
            break;
        case file_type_t::bmp:
            ostream << "BMP";
            break;
        case file_type_t::gl:
            ostream << "GL";
            break;
        case file_type_t::dl:
            ostream << "DL";
            break;
        case file_type_t::wpg:
            ostream << "WPG";
            break;
        case file_type_t::png:
            ostream << "PNG";
            break;
        case file_type_t::jpg:
            ostream << "JPG/JPeg";
            break;
        case file_type_t::mpg:
            ostream << "MPG";
            break;
        case file_type_t::avi:
            ostream << "AVI";
            break;
        case file_type_t::dxf:
            ostream << "DXF";
            break;
        case file_type_t::dwg:
            ostream << "DWG";
            break;
        case file_type_t::t_3ds:
            ostream << "3DS";
            break;
        case file_type_t::mod:
            ostream << "MOD";
            break;
        case file_type_t::t_699:
            ostream << "699";
            break;
        case file_type_t::stm:
            ostream << "STM";
            break;
        case file_type_t::s3m:
            ostream << "S3M";
            break;
        case file_type_t::mtm:
            ostream << "MTM";
            break;
        case file_type_t::far:
            ostream << "FAR";
            break;
        case file_type_t::ult:
            ostream << "ULT";
            break;
        case file_type_t::amf:
            ostream << "AMF";
            break;
        case file_type_t::dmf:
            ostream << "DMF";
            break;
        case file_type_t::okt:
            ostream << "OKT";
            break;
        case file_type_t::rol:
            ostream << "ROL";
            break;
        case file_type_t::cmf:
            ostream << "CMF";
            break;
        case file_type_t::mid:
            ostream << "MID";
            break;
        case file_type_t::sadt:
            ostream << "SADT";
            break;
        case file_type_t::voc:
            ostream << "VOC";
            break;
        case file_type_t::wav:
            ostream << "WAV";
            break;
        case file_type_t::smp8:
            ostream << "SMP8";
            break;
        case file_type_t::smp8s:
            ostream << "SMP8S";
            break;
        case file_type_t::smp16:
            ostream << "SMP16";
            break;
        case file_type_t::smp16s:
            ostream << "SMP16S";
            break;
        case file_type_t::patch8:
            ostream << "PATCH8";
            break;
        case file_type_t::patch16:
            ostream << "PATCH16";
            break;
        case file_type_t::xm:
            ostream << "XM";
            break;
        case file_type_t::hsc:
            ostream << "HSC";
            break;
        case file_type_t::it:
            ostream << "IT";
            break;
        case file_type_t::zip:
            ostream << "ZIP";
            break;
        case file_type_t::arj:
            ostream << "ARJ";
            break;
        case file_type_t::lzh:
            ostream << "LZH";
            break;
        case file_type_t::arc:
            ostream << "ARC";
            break;
        case file_type_t::tar:
            ostream << "TAR";
            break;
        case file_type_t::zoo:
            ostream << "ZOO";
            break;
        case file_type_t::rar:
            ostream << "RAR";
            break;
        case file_type_t::uc2:
            ostream << "UC2";
            break;
        case file_type_t::pak:
            ostream << "PAK";
            break;
        case file_type_t::sqz:
            ostream << "SQZ";
            break;
        case file_type_t::invalid:
            ostream << "Invalid";
            break;
        case file_type_t::undefined:
            ostream << "Undefined";
            break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const non_blink_t& non_blink)
{
    switch(non_blink) {
        case non_blink_t::off:
            ostream << "Off";
            break;
        case non_blink_t::on:
            ostream << "On";
            break;
        case non_blink_t::undefined:
            ostream << "Undefined";
            break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const letter_space_t& letter_space)
{
    switch(letter_space) {
        case letter_space_t::no_preference:
            ostream << "No Preference";
            break;
        case letter_space_t::eight_pixels:
            ostream << "Eight Pixels";
            break;
        case letter_space_t::nine_pixels:
            ostream << "Nine Pixels";
            break;
        case letter_space_t::invalid:
            ostream << "Invalid";
            break;
        case letter_space_t::undefined:
            ostream << "Undefined";
            break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const aspect_ratio_t& aspect_ratio)
{
    switch(aspect_ratio) {
        case aspect_ratio_t::no_preference:
            ostream << "No Preference";
            break;
        case aspect_ratio_t::legacy_aspect:
            ostream << "Legacy Aspect";
            break;
        case aspect_ratio_t::modern_aspect:
            ostream << "Modern Aspect";
            break;
        case aspect_ratio_t::invalid:
            ostream << "Invalid";
            break;
        case aspect_ratio_t::undefined:
            ostream << "Undefined";
            break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const font_type_t& font_type)
{
    switch(font_type) {
    case font_type_t::custom:
        ostream << "Custom";
        break;
    case font_type_t::ibm_vga:
        ostream << "IBM VGA";
        break;
    case font_type_t::ibm_vga50:
        ostream << "IBM VGA50";
        break;
    case font_type_t::ibm_vga25g:
        ostream << "IBM VGA25G";
        break;
    case font_type_t::ibm_ega:
        ostream << "IBM EGA";
        break;
    case font_type_t::ibm_ega43:
        ostream << "IBM EGA43";
        break;
    case font_type_t::ibm_vga_437:
        ostream << "IBM VGA 437";
        break;
    case font_type_t::ibm_vga_720:
        ostream << "IBM VGA 720";
        break;
    case font_type_t::ibm_vga_737:
        ostream << "IBM VGA 737";
        break;
    case font_type_t::ibm_vga_775:
        ostream << "IBM VGA 775";
        break;
    case font_type_t::ibm_vga_819:
        ostream << "IBM VGA 819";
        break;
    case font_type_t::ibm_vga_850:
        ostream << "IBM VGA 850";
        break;
    case font_type_t::ibm_vga_852:
        ostream << "IBM VGA 852";
        break;
    case font_type_t::ibm_vga_855:
        ostream << "IBM VGA 855";
        break;
    case font_type_t::ibm_vga_857:
        ostream << "IBM VGA 857";
        break;
    case font_type_t::ibm_vga_858:
        ostream << "IBM VGA 858";
        break;
    case font_type_t::ibm_vga_860:
        ostream << "IBM VGA 860";
        break;
    case font_type_t::ibm_vga_861:
        ostream << "IBM VGA 861";
        break;
    case font_type_t::ibm_vga_862:
        ostream << "IBM VGA 862";
        break;
    case font_type_t::ibm_vga_863:
        ostream << "IBM VGA 863";
        break;
    case font_type_t::ibm_vga_864:
        ostream << "IBM VGA 864";
        break;
    case font_type_t::ibm_vga_865:
        ostream << "IBM VGA 865";
        break;
    case font_type_t::ibm_vga_866:
        ostream << "IBM VGA 866";
        break;
    case font_type_t::ibm_vga_869:
        ostream << "IBM VGA 869";
        break;
    case font_type_t::ibm_vga_872:
        ostream << "IBM VGA 872";
        break;
    case font_type_t::ibm_vga_kam:
        ostream << "IBM VGA KAM";
        break;
    case font_type_t::ibm_vga_maz:
        ostream << "IBM VGA MAZ";
        break;
    case font_type_t::ibm_vga_mik:
        ostream << "IBM VGA MIK";
        break;
    case font_type_t::ibm_vga50_437:
        ostream << "IBM VGA50 437";
        break;
    case font_type_t::ibm_vga50_720:
        ostream << "IBM VGA50 720";
        break;
    case font_type_t::ibm_vga50_737:
        ostream << "IBM VGA50 737";
        break;
    case font_type_t::ibm_vga50_775:
        ostream << "IBM VGA50 775";
        break;
    case font_type_t::ibm_vga50_819:
        ostream << "IBM VGA50 819";
        break;
    case font_type_t::ibm_vga50_850:
        ostream << "IBM VGA50 850";
        break;
    case font_type_t::ibm_vga50_852:
        ostream << "IBM VGA50 852";
        break;
    case font_type_t::ibm_vga50_855:
        ostream << "IBM VGA50 855";
        break;
    case font_type_t::ibm_vga50_857:
        ostream << "IBM VGA50 857";
        break;
    case font_type_t::ibm_vga50_858:
        ostream << "IBM VGA50 858";
        break;
    case font_type_t::ibm_vga50_860:
        ostream << "IBM VGA50 860";
        break;
    case font_type_t::ibm_vga50_861:
        ostream << "IBM VGA50 861";
        break;
    case font_type_t::ibm_vga50_862:
        ostream << "IBM VGA50 862";
        break;
    case font_type_t::ibm_vga50_863:
        ostream << "IBM VGA50 863";
        break;
    case font_type_t::ibm_vga50_864:
        ostream << "IBM VGA50 864";
        break;
    case font_type_t::ibm_vga50_865:
        ostream << "IBM VGA50 865";
        break;
    case font_type_t::ibm_vga50_866:
        ostream << "IBM VGA50 866";
        break;
    case font_type_t::ibm_vga50_869:
        ostream << "IBM VGA50 869";
        break;
    case font_type_t::ibm_vga50_872:
        ostream << "IBM VGA50 872";
        break;
    case font_type_t::ibm_vga50_kam:
        ostream << "IBM VGA50 KAM";
        break;
    case font_type_t::ibm_vga50_maz:
        ostream << "IBM VGA50 MAZ";
        break;
    case font_type_t::ibm_vga50_mik:
        ostream << "IBM VGA50 MIK";
        break;
    case font_type_t::ibm_vga25g_437:
        ostream << "IBM VGA25G 437";
        break;
    case font_type_t::ibm_vga25g_720:
        ostream << "IBM VGA25G 720";
        break;
    case font_type_t::ibm_vga25g_737:
        ostream << "IBM VGA25G 737";
        break;
    case font_type_t::ibm_vga25g_775:
        ostream << "IBM VGA25G 775";
        break;
    case font_type_t::ibm_vga25g_819:
        ostream << "IBM VGA25G 819";
        break;
    case font_type_t::ibm_vga25g_850:
        ostream << "IBM VGA25G 850";
        break;
    case font_type_t::ibm_vga25g_852:
        ostream << "IBM VGA25G 852";
        break;
    case font_type_t::ibm_vga25g_855:
        ostream << "IBM VGA25G 855";
        break;
    case font_type_t::ibm_vga25g_857:
        ostream << "IBM VGA25G 857";
        break;
    case font_type_t::ibm_vga25g_858:
        ostream << "IBM VGA25G 858";
        break;
    case font_type_t::ibm_vga25g_860:
        ostream << "IBM VGA25G 860";
        break;
    case font_type_t::ibm_vga25g_861:
        ostream << "IBM VGA25G 861";
        break;
    case font_type_t::ibm_vga25g_862:
        ostream << "IBM VGA25G 862";
        break;
    case font_type_t::ibm_vga25g_863:
        ostream << "IBM VGA25G 863";
        break;
    case font_type_t::ibm_vga25g_864:
        ostream << "IBM VGA25G 864";
        break;
    case font_type_t::ibm_vga25g_865:
        ostream << "IBM VGA25G 865";
        break;
    case font_type_t::ibm_vga25g_866:
        ostream << "IBM VGA25G 866";
        break;
    case font_type_t::ibm_vga25g_869:
        ostream << "IBM VGA25G 869";
        break;
    case font_type_t::ibm_vga25g_872:
        ostream << "IBM VGA25G 872";
        break;
    case font_type_t::ibm_vga25g_kam:
        ostream << "IBM VGA25G KAM";
        break;
    case font_type_t::ibm_vga25g_maz:
        ostream << "IBM VGA25G MAZ";
        break;
    case font_type_t::ibm_vga25g_mik:
        ostream << "IBM VGA25G MIK";
        break;
    case font_type_t::ibm_ega_437:
        ostream << "IBM EGA 437";
        break;
    case font_type_t::ibm_ega_720:
        ostream << "IBM EGA 720";
        break;
    case font_type_t::ibm_ega_737:
        ostream << "IBM EGA 737";
        break;
    case font_type_t::ibm_ega_775:
        ostream << "IBM EGA 775";
        break;
    case font_type_t::ibm_ega_819:
        ostream << "IBM EGA 819";
        break;
    case font_type_t::ibm_ega_850:
        ostream << "IBM EGA 850";
        break;
    case font_type_t::ibm_ega_852:
        ostream << "IBM EGA 852";
        break;
    case font_type_t::ibm_ega_855:
        ostream << "IBM EGA 855";
        break;
    case font_type_t::ibm_ega_857:
        ostream << "IBM EGA 857";
        break;
    case font_type_t::ibm_ega_858:
        ostream << "IBM EGA 858";
        break;
    case font_type_t::ibm_ega_860:
        ostream << "IBM EGA 860";
        break;
    case font_type_t::ibm_ega_861:
        ostream << "IBM EGA 861";
        break;
    case font_type_t::ibm_ega_862:
        ostream << "IBM EGA 862";
        break;
    case font_type_t::ibm_ega_863:
        ostream << "IBM EGA 863";
        break;
    case font_type_t::ibm_ega_864:
        ostream << "IBM EGA 864";
        break;
    case font_type_t::ibm_ega_865:
        ostream << "IBM EGA 865";
        break;
    case font_type_t::ibm_ega_866:
        ostream << "IBM EGA 866";
        break;
    case font_type_t::ibm_ega_869:
        ostream << "IBM EGA 869";
        break;
    case font_type_t::ibm_ega_872:
        ostream << "IBM EGA 872";
        break;
    case font_type_t::ibm_ega_kam:
        ostream << "IBM EGA KAM";
        break;
    case font_type_t::ibm_ega_maz:
        ostream << "IBM EGA MAZ";
        break;
    case font_type_t::ibm_ega_mik:
        ostream << "IBM EGA MIK";
        break;
    case font_type_t::ibm_ega43_437:
        ostream << "IBM EGA43 437";
        break;
    case font_type_t::ibm_ega43_720:
        ostream << "IBM EGA43 720";
        break;
    case font_type_t::ibm_ega43_737:
        ostream << "IBM EGA43 737";
        break;
    case font_type_t::ibm_ega43_775:
        ostream << "IBM EGA43 775";
        break;
    case font_type_t::ibm_ega43_819:
        ostream << "IBM EGA43 819";
        break;
    case font_type_t::ibm_ega43_850:
        ostream << "IBM EGA43 850";
        break;
    case font_type_t::ibm_ega43_852:
        ostream << "IBM EGA43 852";
        break;
    case font_type_t::ibm_ega43_855:
        ostream << "IBM EGA43 855";
        break;
    case font_type_t::ibm_ega43_857:
        ostream << "IBM EGA43 857";
        break;
    case font_type_t::ibm_ega43_858:
        ostream << "IBM EGA43 858";
        break;
    case font_type_t::ibm_ega43_860:
        ostream << "IBM EGA43 860";
        break;
    case font_type_t::ibm_ega43_861:
        ostream << "IBM EGA43 861";
        break;
    case font_type_t::ibm_ega43_862:
        ostream << "IBM EGA43 862";
        break;
    case font_type_t::ibm_ega43_863:
        ostream << "IBM EGA43 863";
        break;
    case font_type_t::ibm_ega43_864:
        ostream << "IBM EGA43 864";
        break;
    case font_type_t::ibm_ega43_865:
        ostream << "IBM EGA43 865";
        break;
    case font_type_t::ibm_ega43_866:
        ostream << "IBM EGA43 866";
        break;
    case font_type_t::ibm_ega43_869:
        ostream << "IBM EGA43 869";
        break;
    case font_type_t::ibm_ega43_872:
        ostream << "IBM EGA43 872";
        break;
    case font_type_t::ibm_ega43_kam:
        ostream << "IBM EGA43 KAM";
        break;
    case font_type_t::ibm_ega43_maz:
        ostream << "IBM EGA43 MAZ";
        break;
    case font_type_t::ibm_ega43_mik:
        ostream << "IBM EGA43 MIK";
        break;
    case font_type_t::amiga_topaz_1:
        ostream << "Amiga Topaz 1";
        break;
    case font_type_t::amiga_topaz_1_plus:
        ostream << "Amiga Topaz 1+";
        break;
    case font_type_t::amiga_topaz_2:
        ostream << "Amiga Topaz 2";
        break;
    case font_type_t::amiga_topaz_2_plus:
        ostream << "Amiga Topaz 2+";
        break;
    case font_type_t::amiga_p0t_noodle:
        ostream << "Amiga P0T-NOoDLE";
        break;
    case font_type_t::amiga_microknight:
        ostream << "Amiga MicroKnight";
        break;
    case font_type_t::amiga_microknight_plus:
        ostream << "Amiga MicroKnight+";
        break;
    case font_type_t::amiga_mosoul:
        ostream << "Amiga mOsOul";
        break;
    case font_type_t::c64_petscii_unshifted:
        ostream << "C64 PETSCII unshifted";
        break;
    case font_type_t::c64_petscii_shifted:
        ostream << "C64 PETSCII shifted";
        break;
    case font_type_t::atari_atascii:
        ostream << "Atari ATASCII";
        break;
    case font_type_t::invalid:
        ostream << "Invalid";
        break;
    case font_type_t::undefined:
        ostream << "Undefined";
        break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const palette_type_t& palette_type)
{
    switch(palette_type) {
    case palette_type_t::binary_text:
        ostream << "Binary Text";
        break;
    case palette_type_t::ansi:
        ostream << "ANSi";
        break;
    case palette_type_t::ansi_with_truecolor:
        ostream << "ANSi With Truecolor";
        break;
    case palette_type_t::custom:
        ostream << "Custom";
        break;
    case palette_type_t::truecolor:
        ostream << "Truecolor";
        break;
    case palette_type_t::none:
        ostream << "None";
        break;
    }
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const options_t& options)
{
    ostream << "   Non-Blink: " << options.non_blink << std::endl;
    ostream << "Letter Space: " << options.letter_space << std::endl;
    ostream << "Aspect Ratio: " << options.aspect_ratio << std::endl;
    ostream << "   Font Name: " << options.font_type << std::endl;
    ostream << "     Palette: " << options.palette_type << std::endl;
    return ostream;
}
