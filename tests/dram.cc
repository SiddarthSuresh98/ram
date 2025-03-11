#include "dram.h"
#include "definitions.h"
#include <array>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Construct singleton dram", "[dram]")
{
	Dram *d = new Dram(1, 1);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0th element in zero cycles", "[dram]")
{
	Dram *d = new Dram(1, 0);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	Response r = d->write_word(MEM, w, 0x00000000);
	CHECK(r == OK);

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0th element in three cycles", "[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay; ++i) {
		r = d->write_word(MEM, w, 0x00000000);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_word(MEM, w, 0x00000000);
	CHECK(r == OK);
	d->resolve();

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0, 1th element in three cycles no "
	"conflict",
	"[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay; ++i) {
		r = d->write_word(MEM, w, 0x00000000);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_word(MEM, w, 0x00000000);
	REQUIRE(r == OK);
	// clock cycle did NOT resolve yet!
	// this fetch should not make progress
	r = d->write_word(FETCH, w, 0x00000001);
	CHECK(r == WAIT);

	actual = d->view(0, 1)[0];
	CHECK(r == WAIT);
	d->resolve();

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	for (i = 0; i < delay; ++i) {
		r = d->write_word(FETCH, w, 0x00000001);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_word(FETCH, w, 0x00000001);
	actual = d->view(0, 1)[0];
	CHECK(r == OK);

	expected.at(1) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0, 1th element in three cycles much "
	"conflict",
	"[dram]")
{
	int delay = 2;
	Dram *d = new Dram(1, 2);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay; ++i) {
		r = d->write_word(MEM, w, 0x00000000);
		CHECK(r == WAIT);

		r = d->write_word(FETCH, w, 0x00000001);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_word(MEM, w, 0x00000000);
	CHECK(r == OK);
	r = d->write_word(FETCH, w, 0x00000001);
	CHECK(r == WAIT);
	d->resolve();

	actual = d->view(0, 1)[0];
	expected.at(0) = w;
	REQUIRE(expected == actual);

	for (i = 0; i < delay; ++i) {
		r = d->write_word(FETCH, w, 0x00000001);
		CHECK(r == WAIT);

		r = d->write_word(MEM, w, 0x00000003);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_word(FETCH, w, 0x00000001);
	actual = d->view(0, 1)[0];
	CHECK(r == OK);
	r = d->write_word(MEM, w, 0x00000003);
	CHECK(r == WAIT);

	expected.at(1) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store line in zero cycles", "[dram]")
{
	Dram *d = new Dram(1, 0);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;
	expected = {w, w+1, w+2, w+3};

	Response r = d->write_line(MEM, expected, 0x00000000);
	CHECK(r == OK);

	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store line in three cycles", "[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;
	std::array<signed int, LINE_SIZE> written_line = {w, w+1, w+2, w+3};

	int i;
	Response r;
	for (i = 0; i < delay; ++i) {
		r = d->write_line(MEM, written_line, 0x00000000);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_line(MEM, written_line, 0x00000000);
	CHECK(r == OK);
	d->resolve();

	expected = written_line;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store line in three cycles no "
	"conflict",
	"[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;
	std::array<signed int, LINE_SIZE> written_line = {w, w+1, w+2, w+3};

	int i;
	Response r;
	for (i = 0; i < delay; ++i) {
		r = d->write_line(MEM, written_line, 0x00000000);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_line(MEM, written_line, 0x00000000);
	REQUIRE(r == OK);
	// clock cycle did NOT resolve yet!
	// this fetch should not make progress
	r = d->write_line(FETCH, written_line, 0x00000001);
	CHECK(r == WAIT);

	actual = d->view(0, 1)[0];
	CHECK(r == WAIT);
	d->resolve();

	expected = written_line;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	written_line = {w+4, w+5, w+6, w+7};

	for (i = 0; i < delay; ++i) {
		r = d->write_line(FETCH, written_line, 0x00000001);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_line(FETCH, written_line, 0x00000001);
	actual = d->view(0, 1)[0];
	CHECK(r == OK);

	expected = written_line;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store line in three cycles much "
	"conflict",
	"[dram]")
{
	int delay = 2;
	Dram *d = new Dram(1, 2);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;
	std::array<signed int, LINE_SIZE> written_line = {w, w+1, w+2, w+3};

	int i;
	Response r;
	for (i = 0; i < delay; ++i) {
		r = d->write_line(MEM, written_line, 0x00000000);
		CHECK(r == WAIT);

		r = d->write_line(FETCH, written_line, 0x00000001);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_line(MEM, written_line, 0x00000000);
	CHECK(r == OK);
	r = d->write_line(FETCH, written_line, 0x00000001);
	CHECK(r == WAIT);
	d->resolve();

	actual = d->view(0, 1)[0];
	expected = written_line;
	REQUIRE(expected == actual);

	written_line = {w+4, w+5, w+6, w+7};
	for (i = 0; i < delay; ++i) {
		r = d->write_line(FETCH, written_line, 0x00000001);
		CHECK(r == WAIT);

		r = d->write_line(MEM, written_line, 0x00000003);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_line(FETCH, written_line, 0x00000001);
	actual = d->view(0, 1)[0];
	CHECK(r == OK);
	r = d->write_line(MEM, written_line, 0x00000003);
	CHECK(r == WAIT);

	expected = written_line;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE("Construct singleton dram, write a line to an address in 0 cycles, read in 0 cycles", "[dram]")
{
	Dram *d = new Dram(1, 0);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	expected = {w, w+1, w+2, w+3};
	int addr = 0x00000000;
	d->write_line(MEM, expected, addr);

	Response r = d->read_line(MEM, 0x00000000, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	r = d->read_line(MEM, 0x00000001, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	r = d->read_line(MEM, 0x00000002, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	r = d->read_line(MEM, 0x00000003, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE("Construct singleton dram, write a line to an address in three cycles, read it in three cycles", "[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	expected = {w, w+1, w+2, w+3};
	int addr = 0x00000000;
	
	int i;
	Response r;
	
	for(i=0; i<delay; ++i) {
		r = d->write_line(MEM, expected, addr);
		d->resolve();
	}
	r = d->write_line(MEM, expected, addr);
	d->resolve();

	for (i = 0; i < delay; ++i) {
		r = d->read_line(MEM, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(MEM, 0x00000000, actual);
	CHECK(r == OK);
	d->resolve();
	REQUIRE(expected == actual);
	delete d;
}

TEST_CASE(
	"Construct singleton dram, store line in 3 cycles, read line in 3 cycles with no conflict","[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	expected = {w, w+1, w+2, w+3};
	int addr = 0x00000000;
	
	int i;
	Response r;
	for(int j=0; j<delay; ++j) {
		r = d->write_line(MEM, expected, addr);
		d->resolve();
	}
	r = d->write_line(MEM, expected, addr++);
	d->resolve();

	for (i = 0; i < delay; ++i) {
		r = d->read_line(MEM, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(MEM, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(FETCH, 0x00000003, actual);
	CHECK(r == WAIT); 
	d->resolve();
	REQUIRE(expected == actual);

	actual = {0,0,0,0};
	for (i = 0; i < delay; ++i) {
		r = d->read_line(FETCH, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(FETCH, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(MEM, 0x00000002, actual);
	CHECK(r == WAIT);
	d->resolve();
	REQUIRE(expected == actual);
	
	delete d;

}

TEST_CASE(
	"Construct singleton dram, store line in 3 cycles, read line in 3 cycles with much conflict","[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	expected = {w, w+1, w+2, w+3};
	int addr = 0x00000000;
	
	int i;
	Response r;
	for(int j=0; j<delay; ++j) {
		r = d->write_line(MEM, expected, addr);
		d->resolve();
	}
	r = d->write_line(MEM, expected, addr++);
	d->resolve();


	for (i = 0; i < delay; ++i) {
		r = d->read_line(MEM, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		r = d->read_line(FETCH, 0x00000002, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(MEM, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(FETCH, 0x00000003, actual);
	CHECK(r == WAIT); 
	d->resolve();
	REQUIRE(expected == actual);

	actual = {0,0,0,0};
	for (i = 0; i < delay; ++i) {
		r = d->read_line(FETCH, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		r = d->read_line(MEM, 0x00000002, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(FETCH, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(MEM, 0x00000002, actual);
	CHECK(r == WAIT);
	d->resolve();
	REQUIRE(expected == actual);
	
	delete d;

}


TEST_CASE("Construct singleton dram, write a line to an address one element at a time, read it in zero cycles", "[dram]")
{
	Dram *d = new Dram(1, 0);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	int addr = 0x00000000;
	for(int i=0; i<LINE_SIZE; ++i) {
		Response r = d->write_word(MEM, w, addr++);
		CHECK(r == OK);
		expected.at(i) = w++;
	}

	Response r = d->read_line(MEM, 0x00000000, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	r = d->read_line(MEM, 0x00000001, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	r = d->read_line(MEM, 0x00000002, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	r = d->read_line(MEM, 0x00000003, actual);
	CHECK(r == OK);
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE("Construct singleton dram, write a line to an address one element at a time in 12 cycles, read it in three cycles", "[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	int addr = 0x00000000;
	int i;
	Response r;
	for(i=0; i<LINE_SIZE; ++i) {
		for(int j=0; j<delay; ++j) {
			r = d->write_word(MEM, w, addr);
			d->resolve();
		}
		r = d->write_word(MEM, w, addr++);
		d->resolve();
		expected.at(i) = w++;
	}

	for (i = 0; i < delay; ++i) {
		r = d->read_line(MEM, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(MEM, 0x00000000, actual);
	CHECK(r == OK);
	d->resolve();
	REQUIRE(expected == actual);
	delete d;
}

TEST_CASE(
	"Construct singleton dram, store line one element at a time in 12 cycles, read line in 3 cycles with no conflict","[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	int addr = 0x00000000;
	int i;
	Response r;
	for(i=0; i<LINE_SIZE; ++i) {
		for(int j=0; j<delay; ++j) {
			r = d->write_word(MEM, w, addr);
			d->resolve();
		}
		r = d->write_word(MEM, w, addr++);
		d->resolve();
		expected.at(i) = w++;
	}

	for (i = 0; i < delay; ++i) {
		r = d->read_line(MEM, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(MEM, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(FETCH, 0x00000003, actual);
	CHECK(r == WAIT); 
	d->resolve();
	REQUIRE(expected == actual);

	actual = {0,0,0,0};
	for (i = 0; i < delay; ++i) {
		r = d->read_line(FETCH, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(FETCH, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(MEM, 0x00000002, actual);
	CHECK(r == WAIT);
	d->resolve();
	REQUIRE(expected == actual);
	
	delete d;

}

TEST_CASE(
	"Construct singleton dram, store line one element at a time in 12 cycles, read line in 3 cycles with much conflict","[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223311;
	int addr = 0x00000000;
	int i;
	Response r;
	for(i=0; i<LINE_SIZE; ++i) {
		for(int j=0; j<delay; ++j) {
			r = d->write_word(MEM, w, addr);
			d->resolve();
		}
		r = d->write_word(MEM, w, addr++);
		d->resolve();
		expected.at(i) = w++;
	}

	for (i = 0; i < delay; ++i) {
		r = d->read_line(MEM, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		r = d->read_line(FETCH, 0x00000002, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(MEM, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(FETCH, 0x00000003, actual);
	CHECK(r == WAIT); 
	d->resolve();
	REQUIRE(expected == actual);

	actual = {0,0,0,0};
	for (i = 0; i < delay; ++i) {
		r = d->read_line(FETCH, 0x00000000, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		r = d->read_line(MEM, 0x00000002, actual);
		CHECK(r == WAIT);
		REQUIRE(expected != actual);
		d->resolve();
	}

	r = d->read_line(FETCH, 0x00000000, actual);
	REQUIRE(r == OK);
	r = d->read_line(MEM, 0x00000002, actual);
	CHECK(r == WAIT);
	d->resolve();
	REQUIRE(expected == actual);
	
	delete d;

}


TEST_CASE("Sidedoor bypasses delay", "[dram]")
{
	int delay = 3;
	Dram *d = new Dram(1, delay);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	int i;
	Response r;
	for (i = 0; i < delay - 1; ++i) {
		r = d->write_word(MEM, w, 0x00000000);
		CHECK(r == WAIT);

		actual = d->view(0, 1)[0];
		REQUIRE(expected == actual);
		d->resolve();
	}

	r = d->write_word(MEM, w, 0x00000000);
	CHECK(r == WAIT);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	r = d->write_word(SIDE, w, 0x00000001);
	actual = d->view(0, 1)[0];
	CHECK(r == OK);

	expected.at(1) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}
