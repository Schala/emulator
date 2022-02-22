#ifndef _GB_CPU_H
#define _GB_CPU_H

#include "../cpu.h

class LR35902 : public Z80
{
public:
	LR35902();

	// CPU clock operation (execute one instruction)
	void Clock() override;

	// Disassemble from the specified address for the specified length
	void Disassemble(size_t) override;


	// None of these should be inlined, as we need them to be addressable.
	// All return the number of additional cycles possibly needed.

	uint8_t STOP();

	uint8_t SWAP();
};

#endif // _Z80_CPU_H
