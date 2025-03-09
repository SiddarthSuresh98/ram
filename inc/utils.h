#ifndef UTILS_H
#define UTILS_H

/**
 * Parse an address into a tag, index into the cache table, and a line
 * offset.
 * @param the address to be parsed
 * @param the resulting tag
 * @param the resulting index
 * @param the resulting offset
 */
void get_bit_fields(int address, int *tag, int *index, int *offset);

#endif /* UTILS_H_INCLUDED */
