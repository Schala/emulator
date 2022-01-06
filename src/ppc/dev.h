#ifndef _PPC_DEV_H
#define _PPC_DEV_H

#include <stdint.h>

typedef struct _PPC_REGS
{
	uint32_t sp; // stack pointer
	uint32_t toc; // table of contents pointer
	uint32_t r[32]; // general purpose
	double f[32]; // floating point
} PPC_REGS;

typedef struct _PPC_CPU
{
	PPC_REGS regs;
} PPC_CPU;

#endif // _PPC_DEV_H
