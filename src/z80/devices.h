#ifndef _Z80_DEVICES_H
#define _Z80_DEVICES_H

#include <stdint.h>

typedef struct _Z80_STATUS
{
	uint8_t
		s : 1, // sign
		z : 1, // zero
		_2 : 1,
		h : 1, // half carry
		_4 : 1,
		v : 1, // overflow
		n : 1, // add/subtract
		c : 1; // carry
} Z80_STATUS;

typedef struct _Z80_REGS
{
	uint8_t a; // accumulator
	Z80_STATUS f;

	// general purpse, either 8 bit or paired to be 16 bit
	uint8_t b, c,
			d, e,
			h, l;

	uint8_t i; // interrupt page address
	uint8_t r; // memory refresh
	uint16_t ix, iy; // index registers
	uint16_t pc; // counter
	uint16_t sp; // stack pointer
} Z80_REGS;

typedef struct _Z80_CPU
{
	Z80_REGS regs;
} Z80_CPU;

#endif // _Z80_DEVICES_H
