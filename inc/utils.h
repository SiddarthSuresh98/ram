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
void get_cache_fields(int address, int *tag, int *index, int *offset);

/**
 * Parse an instruction into a type, opcode, and fields. If the type is invalid,
 * only the type field will be set.
 * @param the resulting first field, which varies per type. To call this
 * function properly, this field must contain the full instruction bytes on
 * function entry.
 * @param the resulting second field, which varies per type.
 * @param the resulting third field, which varies per type.
 * @param the resulting type.
 * @param the resulting opcode.
 */
void get_instr_fields(
	signed int &s1,
	signed int &s2,
	signed int &s3,
	unsigned int &type,
	unsigned int &opcode);

/**
 * Formats a string using snprintf.
 * @param an object that represents the format string
 * @param arguments to be formatted
 * @return a string object holding the formatted result
 */
const std::string string_format(const char *const zcFormat, ...);

/**
 * Given `address`, returns an address that is within the current memory size
 * using a clean wrap.
 * @param an address
 * @return an address guaranteed to be within range.
 */
int wrap_address(int address);

/**
 * Given `address`, returns the line and word it is in.
 * @param an address
 * @param the line (row) `address` is in
 * @param the word (column) `address` corresponds to
 */
void get_memory_index(int address, int &line, int &word);

#endif /* UTILS_H_INCLUDED */
