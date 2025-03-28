#include "utils.h"
#include "definitions.h"
#include <cstdarg>
#include <string>
#include <vector>

static Logger *global_log = Logger::getInstance();

void get_cache_fields(int address, int *tag, int *index, int *offset)
{
	*tag = GET_MID_BITS(address, L1_CACHE_LINE_SPEC + LINE_SPEC, MEM_WORD_SPEC);
	*index = GET_MID_BITS(address, LINE_SPEC, L1_CACHE_LINE_SPEC + LINE_SPEC);
	*offset = GET_LS_BITS(address, LINE_SPEC);
}

void get_instr_fields(
	signed int &s1,
	signed int &s2,
	signed int &s3,
	unsigned int &type,
	unsigned int &opcode)
{
	type = GET_LS_BITS(s1, TYPE_SIZE);
	switch (type) {
	case 0:
		// R-TYPE
		opcode = GET_MID_BITS(s1, TYPE_SIZE, TYPE_SIZE + R_OPCODE_SIZE);
		s3 = GET_MID_BITS(
			s1, TYPE_SIZE + R_OPCODE_SIZE + (REG_SIZE * 2),
			TYPE_SIZE + R_OPCODE_SIZE + (REG_SIZE * 3));
		s2 = GET_MID_BITS(
			s1, TYPE_SIZE + R_OPCODE_SIZE + REG_SIZE,
			TYPE_SIZE + R_OPCODE_SIZE + (REG_SIZE * 2));
		s1 = GET_MID_BITS(
			s1, TYPE_SIZE + R_OPCODE_SIZE,
			TYPE_SIZE + R_OPCODE_SIZE + REG_SIZE);
		break;
	case 1:
		// I-TYPE
		opcode = GET_MID_BITS(s1, TYPE_SIZE, TYPE_SIZE + OPCODE_SIZE);
		s3 = GET_MID_BITS(
			s1, TYPE_SIZE + OPCODE_SIZE + (REG_SIZE * 2), WORD_SPEC);
		s2 = GET_MID_BITS(
			s1, TYPE_SIZE + OPCODE_SIZE + REG_SIZE,
			TYPE_SIZE + OPCODE_SIZE + (REG_SIZE * 2));
		s1 = GET_MID_BITS(
			s1, TYPE_SIZE + OPCODE_SIZE, TYPE_SIZE + OPCODE_SIZE + REG_SIZE);
		break;
	case 2:
		// J-TYPE
		opcode = GET_MID_BITS(s1, TYPE_SIZE, TYPE_SIZE + OPCODE_SIZE);
		s2 = GET_MID_BITS(s1, TYPE_SIZE + OPCODE_SIZE + REG_SIZE, WORD_SPEC);
		s1 = GET_MID_BITS(
			s1, TYPE_SIZE + OPCODE_SIZE, TYPE_SIZE + OPCODE_SIZE + REG_SIZE);
		break;
	default:
		global_log->log(
			DEBUG,
			string_format("%s returning invalid type: %d", __FUNCTION__, type));
	}
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

int wrap_address(int address)
{
	if (address < 0) {
		return ((address % MEM_WORDS) + MEM_WORDS) % MEM_WORDS;
	}
	return address % MEM_WORDS;
}

void get_memory_index(int address, int &line, int &word)
{
	line = wrap_address(address) / LINE_SIZE;
	word = address % LINE_SIZE;
}
