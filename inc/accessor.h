#ifndef ACCESSOR_H
#define ACCESSOR_H
#include <iostream>

enum Accessor {
	IDLE,
	WRITE,
	MEM,
	EXEC,
	DCDE,
	FETCH,
	L1CACHE,
	SIDE,
};

std::ostream &operator<<(std::ostream &os, Accessor a);

#endif /* ACCESSOR_H_INCLUDED */
