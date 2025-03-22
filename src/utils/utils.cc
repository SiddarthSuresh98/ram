#include "utils.h"
#include "definitions.h"
#include <cstdarg>
#include <string>
#include <vector>

void get_bit_fields(int address, int *tag, int *index, int *offset)
{
	*tag = GET_MID_BITS(
		address, L1_CACHE_LINE_SPEC + LINE_SPEC, MEM_WORD_SPEC);
	*index = GET_MID_BITS(address, LINE_SPEC, L1_CACHE_LINE_SPEC + LINE_SPEC);
	*offset = GET_LS_BITS(address, LINE_SPEC);
}

const std::string string_format(const char *const zcFormat, ...)
{
	va_list vaArgs;
	va_start(vaArgs, zcFormat);

	va_list vaArgsCopy;
	va_copy(vaArgsCopy, vaArgs);
	const int iLen = std::vsnprintf(NULL, 0, zcFormat, vaArgsCopy);
	va_end(vaArgsCopy);

	std::vector<char> zc(iLen + 1);
	std::vsnprintf(zc.data(), zc.size(), zcFormat, vaArgs);
	va_end(vaArgs);
	return std::string(zc.data(), iLen);
}

int wrap_address(int address) {
	if (address < 0){
		return ((address % MEM_WORDS) + MEM_WORDS) % MEM_WORDS;
	}
	return address % MEM_WORDS;
}

void get_memory_index(int address, int &line, int &word)
{
	line = wrap_address(address) / LINE_SIZE;
	word = address % LINE_SIZE;
}
