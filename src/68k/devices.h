#ifndef _M68K_DEVICES_H
#define _M68K_DEVICES_H

#include <stdint.h>

#define SWAP16(x) ((((x) & 0xFF00) >> 8) | (((x) & 255) << 8))
#define SWAP32(x) \
	((((x) & 0xFF000000) >> 24) | \
	(((x) & 0xFF0000) >> 16) | \
	(((x) & 0xFF00) << 16) | \
	(((x) & 255) << 24))

typedef struct _M68K_STATUS
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
} M68K_STATUS;

typedef struct _M68K_REGS
{
	M68K_STATUS flags;
	uint32_t d[8];
	uint32_t a[7];
	uint32_t sp;
	uint32_t pc;
} M68K_REGS;

typedef struct _M68000
{
	M68K_REGS regs;
} M68000;

#endif // _M68K_DEVICES_H
