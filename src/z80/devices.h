#ifndef _Z80_DEVICES_H
#define _Z80_DEVICES_H

#include <array>
#include <functional>
#include <map>
#include <string_view>

#include "../devices.h"

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

	struct
	{
		struct
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
		} state;

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

	uint8_t Add();


	// add with carry
	uint8_t AddCarry();

	// adjust accumulator for add and subtact operations
	uint8_t AdjustAddSubtract();

	uint8_t ArithmeticShiftLeft();

	uint8_t ArithmeticShiftRight();

	uint8_t Decrement();

	uint8_t Increment();

	uint8_t Negate();

	uint8_t Subtract();

	uint8_t SubtractCarry();


	// --- bitwise ---

	uint8_t And();

	uint8_t BitShiftRight();

	uint8_t Compliment();

	uint8_t Or();

	uint8_t ResetBit();

	uint8_t RotateAccumulatorLeft();

	uint8_t RotateAccumulatorLeftCarry();

	uint8_t RotateAccumulatorRight();

	uint8_t RotateAccumulatorRightCarry();

	uint8_t RotateLeft();

	uint8_t RotateLeftCarry();

	uint8_t RotateRight();

	uint8_t RotateRightCarry();

	uint8_t SetBit();

	uint8_t SwapHiLo();

	uint8_t SwapLoHi();

	uint8_t Xor();


	// --- comparison ---

	uint8_t Compare();

	// compare and decrement
	uint8_t CompareDecrement();

	// compare and decrement and repeat
	uint8_t CompareDecrementLoop();

	// compare and increment
	uint8_t CompareIncrement();

	// compare and increment and repeat
	uint8_t CompareIncrementLoop();

	uint8_t TestBit();


	// --- state control ---

	uint8_t DisableMaskableInterrupts();

	uint8_t EnableInterrupts();

	uint8_t InvertCarry();


	// --- jumps ---

	uint8_t Call();

	uint8_t ConditionalJump();

	// decrement and jump if not zero
	uint8_t DecrementJumpNonZero();

	uint8_t GoTo();

	uint8_t Jump();

	uint8_t Return();


	// --- storage control ---

	uint8_t Exchange8();

	uint8_t Exchange16();

	uint8_t Load();

	// load and decrement
	uint8_t LoadDecrement();

	// load and decrement and repeat
	uint8_t LoadDecrementLoop();

	// load and increment
	uint8_t LoadIncrement();

	// load and increment and repeat
	uint8_t LoadIncrementLoop();

	uint8_t PutInput();

	// place on input and decrement
	uint8_t PutInputDecrement();

	// place on input and decrement and repeat
	uint8_t PutInputDecrementLoop();

	// place on input and increment
	uint8_t PutInputIncrement();

	// place on input and increment and repeat
	uint8_t PutInputIncrementLoop();

	uoutt8_t PutOutput();

	// place on output and decrement
	uoutt8_t PutOutputDecrement();

	// place on output and decrement and repeat
	uoutt8_t PutOutputDecrementLoop();

	// place on output and outcrement
	uoutt8_t PutOutputOutcrement();

	// place on output and outcrement and repeat
	uoutt8_t PutOutputOutcrementLoop();


	// --- interrupts ---

	// halt until interrupt
	uint8_t Halt();

	// return from interrupt
	uint8_t InterruptReturn();

	uint8_t NonMaskableInterruptReturn();

	uint8_t SetInterruptMode();


	// --- stack control ---

	uint8_t Pop();

	uint8_t Push();


	// --- misc ---
	uint8_t NoOperation();
};

#endif // _Z80_DEVICES_H
