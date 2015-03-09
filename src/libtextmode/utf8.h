#ifndef UTF8_H
#define UTF8_H

#include <vector>

std::string cp_437_code_to_string(const uint8_t& cp_437);
std::string cp_437_to_utf8_string(std::vector<uint8_t>& vector);

#endif
