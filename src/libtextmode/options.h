#ifndef OPTIONS_H
#define OPTIONS_H

#include <fstream>

enum class data_type_t
{
    character,
    bitmap,
    vector,
    audio,
    binary_text,
    xbin,
    archive,
    executable,
    invalid,
    undefined
};

enum class file_type_t
{
    ascii,
    ansi,
    ansimation,
    rip_script,
    pcboard,
    avatar,
    html,
    source,
    tundradraw,
    gif,
    pcx,
    lbm_iff,
    tga,
    fli,
    flc,
    bmp,
    gl,
    dl,
    wpg,
    png,
    jpg,
    mpg,
    avi,
    dxf,
    dwg,
    t_3ds,
    mod,
    t_699,
    stm,
    s3m,
    mtm,
    far,
    ult,
    amf,
    dmf,
    okt,
    rol,
    cmf,
    mid,
    sadt,
    voc,
    wav,
    smp8,
    smp8s,
    smp16,
    smp16s,
    patch8,
    patch16,
    xm,
    hsc,
    it,
    zip,
    arj,
    lzh,
    arc,
    tar,
    zoo,
    rar,
    uc2,
    pak,
    sqz,
    invalid,
    undefined
};

enum class non_blink_t
{
    off,
    on,
    undefined
};

enum class letter_space_t
{
    no_preference,
    eight_pixels,
    nine_pixels,
    invalid,
    undefined
};

enum class aspect_ratio_t
{
    no_preference,
    legacy_aspect,
    modern_aspect,
    invalid,
    undefined
};

enum class font_type_t
{
    custom,
    ibm_vga,
    ibm_vga50,
    ibm_vga25g,
    ibm_ega,
    ibm_ega43,
    ibm_vga_437,
    ibm_vga_720,
    ibm_vga_737,
    ibm_vga_775,
    ibm_vga_819,
    ibm_vga_850,
    ibm_vga_852,
    ibm_vga_855,
    ibm_vga_857,
    ibm_vga_858,
    ibm_vga_860,
    ibm_vga_861,
    ibm_vga_862,
    ibm_vga_863,
    ibm_vga_864,
    ibm_vga_865,
    ibm_vga_866,
    ibm_vga_869,
    ibm_vga_872,
    ibm_vga_kam,
    ibm_vga_maz,
    ibm_vga_mik,
    ibm_vga50_437,
    ibm_vga50_720,
    ibm_vga50_737,
    ibm_vga50_775,
    ibm_vga50_819,
    ibm_vga50_850,
    ibm_vga50_852,
    ibm_vga50_855,
    ibm_vga50_857,
    ibm_vga50_858,
    ibm_vga50_860,
    ibm_vga50_861,
    ibm_vga50_862,
    ibm_vga50_863,
    ibm_vga50_864,
    ibm_vga50_865,
    ibm_vga50_866,
    ibm_vga50_869,
    ibm_vga50_872,
    ibm_vga50_kam,
    ibm_vga50_maz,
    ibm_vga50_mik,
    ibm_vga25g_437,
    ibm_vga25g_720,
    ibm_vga25g_737,
    ibm_vga25g_775,
    ibm_vga25g_819,
    ibm_vga25g_850,
    ibm_vga25g_852,
    ibm_vga25g_855,
    ibm_vga25g_857,
    ibm_vga25g_858,
    ibm_vga25g_860,
    ibm_vga25g_861,
    ibm_vga25g_862,
    ibm_vga25g_863,
    ibm_vga25g_864,
    ibm_vga25g_865,
    ibm_vga25g_866,
    ibm_vga25g_869,
    ibm_vga25g_872,
    ibm_vga25g_kam,
    ibm_vga25g_maz,
    ibm_vga25g_mik,
    ibm_ega_437,
    ibm_ega_720,
    ibm_ega_737,
    ibm_ega_775,
    ibm_ega_819,
    ibm_ega_850,
    ibm_ega_852,
    ibm_ega_855,
    ibm_ega_857,
    ibm_ega_858,
    ibm_ega_860,
    ibm_ega_861,
    ibm_ega_862,
    ibm_ega_863,
    ibm_ega_864,
    ibm_ega_865,
    ibm_ega_866,
    ibm_ega_869,
    ibm_ega_872,
    ibm_ega_kam,
    ibm_ega_maz,
    ibm_ega_mik,
    ibm_ega43_437,
    ibm_ega43_720,
    ibm_ega43_737,
    ibm_ega43_775,
    ibm_ega43_819,
    ibm_ega43_850,
    ibm_ega43_852,
    ibm_ega43_855,
    ibm_ega43_857,
    ibm_ega43_858,
    ibm_ega43_860,
    ibm_ega43_861,
    ibm_ega43_862,
    ibm_ega43_863,
    ibm_ega43_864,
    ibm_ega43_865,
    ibm_ega43_866,
    ibm_ega43_869,
    ibm_ega43_872,
    ibm_ega43_kam,
    ibm_ega43_maz,
    ibm_ega43_mik,
    amiga_topaz_1,
    amiga_topaz_1_plus,
    amiga_topaz_2,
    amiga_topaz_2_plus,
    amiga_p0t_noodle,
    amiga_microknight,
    amiga_microknight_plus,
    amiga_mosoul,
    c64_petscii_unshifted,
    c64_petscii_shifted,
    atari_atascii,
    invalid,
    undefined
};

enum class palette_type_t
{
    binary_text,
    ansi,
    ansi_with_truecolor,
    custom,
    truecolor,
    none
};

class options_t
{
public:
    non_blink_t non_blink = non_blink_t::undefined;
    letter_space_t letter_space = letter_space_t::undefined;
    aspect_ratio_t aspect_ratio = aspect_ratio_t::undefined;
    font_type_t font_type = font_type_t::undefined;
    palette_type_t palette_type = palette_type_t::none;
};

std::ostream& operator<<(std::ostream&, const data_type_t&);

std::ostream& operator<<(std::ostream&, const file_type_t&);

std::ostream& operator<<(std::ostream&, const non_blink_t&);

std::ostream& operator<<(std::ostream&, const letter_space_t&);

std::ostream& operator<<(std::ostream&, const aspect_ratio_t&);

std::ostream& operator<<(std::ostream&, const font_type_t&);

std::ostream& operator<<(std::ostream&, const options_t&);

std::ostream& operator<<(std::ostream&, const palette_type_t&);

#endif
