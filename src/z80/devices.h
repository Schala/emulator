#ifndef _Z80_DEVICES_H
#define _Z80_DEVICES_H

#include <array>
#include <functional>
#include <map>
#include <string_view>

#include "../core/devices.h"

class Z80;

struct Z80Opcode
{
	uint8_t Cycles;
	std::function<uint8_t(Z80 *)> Operation;
	std::string_view Label;
};

// Disassembly cache
struct Z80Disassembly
{
	uint16_t Address;
	std::string_view Line;
};

class Z80 : public Device
{
private:
	static const std::array<Z80Opcode, 256> BitOps;
	static const std::map<uint8_t, Z80Opcode> ExtOps;
	static const std::array<Z80Opcode, 256> IXBitOps;
	static const std::map<uint8_t, Z80Opcode> IXOps;
	static const std::array<Z80Opcode, 256> IYBitOps;
	static const std::map<uint8_t, Z80Opcode> IYOps;
	static const std::array<Z80Opcode, 256> MainOps;

	struct Registers
	{
		struct State
		{
			bool
				s : 1, // sign
				z : 1, // zero
				_2 : 1,
				h : 1, // half carry
				_4 : 1,
				v : 1, // overflow
				n : 1, // add/subtract
				c : 1; // carry
		} p;

		uint8_t a; // accumulator
		uint8_t sp; // stack pointer

		// general purpse, either 8 bit or paired to be 16 bit
		uint8_t b, c,
				d, e,
				h, l;

		uint8_t i; // interrupt page address
		uint8_t r; // memory refresh
		uint16_t ix, iy; // index registers
		uint16_t pc; // counter
	} m_regs;

	std::vector<Z80Disassembly> m_disasm;

	// None of these should be inlined, as we need them to be addressable.
	// All return the number of additional cycles possibly needed.

	// --- arithmetic ---

	// add with carry
	uint8_t ADC();

	uint8_t ADD();

	// adjust accumulator for add and subtact operations
	uint8_t DAA();

	uint8_t DEC();

	uint8_t INC();

	uint8_t NEG();

	uint8_t SBC();

	uint8_t SUB();


	// --- bitwise ---

	uint8_t AND();

	uint8_t CPL();

	uint8_t OR();

	uint8_t RES();

	uint8_t RL();

	uint8_t RLA();

	uint8_t RLCA();

	uint8_t RLC();

	uint8_t RLD();

	uint8_t RR();

	uint8_t RRA();

	uint8_t RRC();

	uint8_t RRCA();

	uint8_t RRD();

	uint8_t SET();

	uint8_t SLA();

	uint8_t SRA();

	uint8_t SRL();

	uint8_t XOR();


	// --- comparison ---

	uint8_t BIT();

	uint8_t CP();

	// compare and decrement
	uint8_t CPD();

	// compare and decrement and repeat
	uint8_t CPDR();

	// compare and increment
	uint8_t CPI();

	// compare and increment and repeat
	uint8_t CPIR();


	// --- state control ---

	uint8_t CCF();

	uint8_t DI();

	uint8_t EI();

	uint8_t SCF();


	// --- jumps ---

	uint8_t CALL();

	// decrement and jump if not zero
	uint8_t DJNZ();

	uint8_t GoTo();

	uint8_t JP();

	uint8_t JR();

	uint8_t RET();


	// --- storage control ---

	uint8_t EX();

	uint8_t EXX();

	uint8_t IN();

	// place on input and decrement
	uint8_t IND();

	// place on input and decrement and repeat
	uint8_t INDR();

	// place on input and increment
	uint8_t INI();

	// place on input and increment and repeat
	uint8_t INIR();

	uint8_t LD();

	// load and decrement
	uint8_t LDD();

	// load and decrement and repeat
	uint8_t LDDR();

	// load and increment
	uint8_t LDI();

	// load and increment and repeat
	uint8_t LDIR();

	// place on output and decrement and repeat
	uint8_t OTDR();

	// place on output and increment and repeat
	uint8_t OTIR();

	uint8_t OUT();

	// place on output and decrement
	uint8_t OUTD();

	// place on output and increment
	uint8_t OUTI();


	// --- interrupts ---

	// halt until interrupt
	uint8_t HALT();

	uint8_t IM();

	// return from interrupt
	uint8_t RETI();

	uint8_t RETN();


	// --- stack control ---

	uint8_t POP();

	uint8_t PUSH();


	// --- misc ---
	uint8_t NOP();
};

#endif // _Z80_DEVICES_H
