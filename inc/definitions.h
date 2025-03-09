#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <cmath>

/* The number of bits to specify a word in a line
 */
#define LINE_SPEC 2
#define LINE_SIZE (int)pow(2, 2)

/* The number of bits to specify a memory line
 * (/ (expt 2 15) 4)
 */
#define MEM_SPEC 13
#define MEM_SIZE (int)pow(2, MEM_SPEC)

/* The number of bits to specify a l1 cache line
 */
#define L1_CACHE_SPEC 5
#define L1_CACHE_SIZE (int)pow(2, L1_CACHE_SPEC)

/* Parses some bits.
 */
#define LAST(k, n) ((k) & ((1 << (n)) - 1))
#define MID(k, m, n) LAST((k) >> (m), ((n) - (m)))

#endif /* DEFINITIONS_H_INCLUDED */
