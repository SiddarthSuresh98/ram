#ifndef RESPONSE_H
#define RESPONSE_H

enum Status {
	OK,
	WAIT,
	BLOCKED,
};

struct Response {
	Status status;
	int *line;
	int val;
};

#endif /* RESPONSE_H_INCLUDED */
