#include "definitions.h"
#include "utils.h"

void get_bit_fields(int address, int *tag, int *index, int *offset)
{
	*tag =
		MID(address, LINE_SPEC + L1_CACHE_SPEC,
			MEM_SPEC + LINE_SPEC + L1_CACHE_SPEC);
	*index = MID(address, LINE_SPEC, L1_CACHE_SPEC + LINE_SPEC);
	*offset = LAST(address, LINE_SPEC);
}
