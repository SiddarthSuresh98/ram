#include "utils.h"
#include "definitions.h"

void get_bit_fields(int address, int *tag, int *index, int *offset)
{
	*tag =
		GET_MID_BITS(address, LINE_SPEC + L1_CACHE_SPEC,
			MEM_SPEC + LINE_SPEC + L1_CACHE_SPEC);
	*index = GET_MID_BITS(address, LINE_SPEC, L1_CACHE_SPEC + LINE_SPEC);
	*offset = GET_LS_BITS(address, LINE_SPEC);
}
