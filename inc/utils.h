#ifndef UTILS_H
#define UTILS_H
#include <string>

/**
 * Parse an address into a tag, index into the cache table, and a line
 * offset.
 * @param the address to be parsed
 * @param the resulting tag
 * @param the resulting index
 * @param the resulting offset
 */
void get_bit_fields(int address, int *tag, int *index, int *offset);

/**
 * Formats a string using snprintf.
 * @param an object that represents the format string
 * @param arguments to be formatted
 * @return a string object holding the formatted result
 */
const std::string string_format(const char *const zcFormat, ...);

int wrap_address(int address);

#endif /* UTILS_H_INCLUDED */
