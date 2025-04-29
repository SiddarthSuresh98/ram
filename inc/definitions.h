// Memory subsystem for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef DEFINITIONS_H
#define DEFINITIONS_H
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
 * Number of bits in a word
 */
#define WORD_SPEC 32

/**
 * The number of bits to specify a memory word
 * The number of bits to specify a memory line
 * The total number of lines in memory
 */
#define MEM_WORD_SPEC 14
#define MEM_LINE_SPEC static_cast<unsigned int>(MEM_WORD_SPEC - LINE_SPEC)
#define MEM_WORDS static_cast<int>(pow(2, MEM_WORD_SPEC))
#define MEM_LINES static_cast<int>(pow(2, MEM_LINE_SPEC))

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
