#ifndef _MIPS_DEV_H
#define _MIPS_DEV_H

#include <stdint.h>

typedef struct _MIPS_REGS
{
	uint8_t ll : 1; // load/link bit
	float ir; // implementation/revision
	float cs; // control/tatus
	const uint64_t zero; // 1st GPR always 0 and read only
	uint64_t gp[30]; // general purpose
	uint64_t l; // link
	double fp[32]; // floating point

	// multiply and divide
	uint64_t hi;
	uint64_t lo;

	uint64_t pc; // counter
} MIPS_REGS;

typedef struct _MIPS_CP0_REGS
{
	uint64_t i; // TLB array pointer
	uint64_t r; // pseudorandom pointer into TLB array (read only)
	uint64_t el[2]; // low half of TLB entry for even and odd virtual address (VPN)
	uint64_t ctx; // pointer to kernel virtual page table entry in 32 bit mode
	uint64_t pm; // page size spec
	uint64_t w; // number of wired TLB entries
	uint64_t bva; // virtual address of last error
	uint64_t cnt; // timer count
	uint64_t eh; // high half of TLB entry
	uint64_t cmp; // timer compare value
	uint64_t st; // operation status
	uint64_t why; // cause of last exception
	uint64_t epc; // exception program counter
	uint64_t prid; // processor revision identifier
	uint64_t cfg; // memory system mode
	uint64_t lla; // load linked instruction address
	uint64_t wlo; // memory reference trap address lo bits
	uint64_t whi; // memory reference trap address hi bits
	uint64_t xc; // pointer to kernel virtual PTE table in 64 bit mode
	uint64_t par; // cache parity bits
	uint64_t ce; // cache error and status register
	uint64_t tlo; // cache tag register lo
	uint64_t thi; // cache tag register hi
	uint64_t eepc; // error exception program counter
} MIPS_CP0_REGS;

typedef struct _MIPS_CPU {
	MIPS_REGS regs;
} MIPS_CPU;

MIPS_CPU * mips_cpu_alloc();
void mips_cpu_free(MIPS_CPU *);

#endif // _MIPS_DEV_H
