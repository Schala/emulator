#ifndef _68K_DEV_H
#define _68K_DEV_H

#include <stdint.h>

typedef struct _M68K_STATE
{
	uint16_t
		t : 2,
		s : 1,
		m : 1,
		_4 : 1,
		i : 3, // interrupt
		_8 : 3,
		x : 1, // extend
		n : 1, // negative
		z : 1, // zero
		v : 1, // overflow
		c : 1; // carry
} M68K_STATE;

typedef struct _M68K_REGS
{
	M68K_STATE flags;
	uint32_t d[8];
	uint32_t a[7];
	uint32_t sp;
	uint32_t pc;
} M68K_REGS;

typedef struct _M68K_CPU
{
	M68K_REGS regs;
} M68K_CPU;

#endif // _68K_DEV_H
