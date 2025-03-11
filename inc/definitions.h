#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include "logger.h"
#include <cmath>

/**
 * The number of bits to specify a word in a line
 */
#define LINE_SPEC 2
/**
 * The total number of words in a line
 */
#define LINE_SIZE static_cast<int>(pow(2, 2))

/**
 * The number of bits to specify a memory word
 * The number of bits to specify a memory line
 * The total number of lines in memory
 */
#define MEM_WORD_SPEC 10
#define MEM_LINE_SPEC static_cast<unsigned int>(MEM_WORD_SPEC - LINE_SPEC)
#define MEM_WORDS static_cast<int>(pow(2, MEM_WORD_SPEC))
#define MEM_LINES static_cast<int>(pow(2, MEM_LINE_SPEC))

/**
 * The number of bits to specify a l1 cache word
 * The number of bits to specify a l1 cache line
 * The total number of lines in l1 cache
 */
#define L1_CACHE_WORD_SPEC 7
#define L1_CACHE_LINE_SPEC static_cast<unsigned int>(L1_CACHE_WORD_SPEC - LINE_SPEC)
#define L1_CACHE_LINES static_cast<int>(pow(2, L1_CACHE_LINE_SPEC))

/**
 * The total number of cycles a memory access takes.
 */
#define MEM_DELAY 3

/**
 * The total number of cycles a level one cache access takes
 */
#define L1_CACHE_DELAY 1

/**
 * Return the N least-significant bits from integer K using a bit mask
 * @param the integer to be parsed
 * @param the number of bits to be parsed
 * @return the N least-significant bits from K
 */
#define GET_LS_BITS(k, n) ((k) & ((1 << (n)) - 1))
/**
 * Return the bits from integer K starting at N and ending at M using a bit
 * mask
 * @param the integer to be parsed
 * @param the index of the starting bit to be parsed
 * @param the index of the ending bit to be parsed
 * @return a section of bits from K
 */
#define GET_MID_BITS(k, m, n) GET_LS_BITS((k) >> (m), ((n) - (m)))

#endif /* DEFINITIONS_H_INCLUDED */
