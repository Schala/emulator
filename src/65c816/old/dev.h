#ifndef _65C816_DEV_H
#define _65C816_DEV_H

#include <stdint.h>

typedef struct _STATE_65C816
{
	uint8_t
		n : 1, // negative
		v : 1, // overflow
		m : 1, // accumulator register size (0 = 16 bit, 1 = 8 bit)
		x : 1, // index register size (0 = 16 bit, 1 = 8 bit)
		d : 1, // decimal
		i : 1, // interrupt disable
		z : 1, // zero
		c : 1; // carry
} STATE_65C816;

typedef struct _REGS_65C816
{
	STATE_65C816 p; // processor state
	uint16_t a; // accumulator
	uint16_t b; // accumulator
	uint16_t x; // index
	uint16_t y; // index
	uint32_t dp : 24; // direct page
	uint32_t s : 24; // stack pointer
	uint32_t db : 24; // data bank
	uint32_t pb : 24; // program bank
	uint32_t pc : 24; // counter
} REGS_65C816;

typedef struct _CPU_65C816
	REGS_65C816 regs;
} CPU_65C816;

#endif // _65C816_DEV_H
