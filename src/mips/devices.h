#ifndef _MIPS_DEVICES_H
#define _MIPS_DEVICES_H

#include <stdint.h>

typedef struct _MIPS_REGS {
	// following 31 registers are in order according to their index

	const uint32_t zero; // always 0, read only, should be initialised by calloc on CPU
	uint32_t v0, v1; // expression evals and function result values
	uint32_t a0, a1, a2, a3; // first 4 args for subroutine
	uint32_t t0, t1, t2, t3, t4, t5, t6, t7; // temp vars
	uint32_t s0, s1, s2, s3, s4, s5, s6, s7; // saved values for final computed results
	uint32_t t8, t9; // more temp vars
	uint32_t ra; // return address

	// registers capable of holding floats or doubles
	double f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;

	uint32_t hi; // special high byte register
	uint32_t lo; // special low byte register
	uint32_t pc; // counter
} MIPS_REGS;

typedef struct _R3000 {
	MIPS_REGS regs;
} R3000;

R3000 * r3000_alloc();
void r3000_free(R3000 *);

#endif // _MIPS_DEVICES_H
