#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>

enum Response {
	OK,
	WAIT,
	BLOCKED,
	STALLED,
};

std::ostream &operator<<(std::ostream &os, Response r);

#endif /* RESPONSE_H_INCLUDED */
