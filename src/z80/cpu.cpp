#include <array>

#include "cpu.h"

static const std::array<Z80Opcode, 256> OPS =
{
	// 0x
	Z80Opcode(4, &Z80::Implied, &Z80::NOP, "NOP"),
	Z80Opcode(7, &Z80::Register, &Z80::LD, "LD"),

	// Ax
	Z80Opcode(4, &Z80::Register, &Z80::XOR, "XOR"),

	// Cx
	Z80Opcode(10, &Z80::Extended, &Z80::JP, "JP")
};

// Z80

Z80::Z80(BusLE16 *bus, uint16_t startAddr, uint16_t endAddr):
	CPU(bus, startAddr, endAddr,
{
}

void Z80::Clock()
{
	if (cycles == 0)
	{
		lastOp = ReadROMByte();

		cycles = OPS[lastOp].Cycles;
		cycles += OPS[lastOp].AddressMode();
		cycles += OPS[lastOp].Operation();
	}

	cycles--;
}

uint8_t Z80::FetchByte()
{
	if (OPS[lastOp].AddressMode != &Z80::Implied)
		cache = ReadByteFromLastAddress();
	return cache;
}
