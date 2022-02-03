#ifndef _6502_DEVICES_H
#define _6502_DEVICES_H

#include <array>
#include <string_view>

#include "../core/devices.h"

// Extends the base Bus class with suitable functions for the 6502
class Bus6502 : public Bus
{
public:
	Bus6502(size_t);

	// Read byte from RAM address
	uint8_t Read(uint16_t) const;

	// Write byte to RAM address
	void Write(uint16_t, uint8_t);
};

class MOS6502;

typedef uint8_t (MOS6502::*Instruction6502)();

// Metadata for the CPU's various operations
struct Opcode6502
{
	// Constructor needed because otherwise we'll get an array error
	Opcode6502(uint8_t, Instruction6502, Instruction6502, std::string_view);

	uint8_t Cycles;
	Instruction6502 AddressMode;
	Instruction6502 Operation;
	std::string_view Mneumonic;
};

// Disassembly cache
struct Disassembly6502
{
	uint16_t Address;
	std::string_view Line;
};

// The CPU processes data available via its bus
class MOS6502 : public Device
{
public:
	// Allocate a new CPU, given a parent bus, and start and end addresses in RAM
	MOS6502(Bus6502 *, uint16_t, uint16_t);

	// CPU clock operation (execute one instruction)
	void Clock();

	// Disassemble from the specified address for the specified length
	void Disassemble(uint16_t, uint16_t);

	// Fetch and cache a byte from the cached absolute address
	uint8_t Fetch();

	// Read address from RAM
	uint16_t FetchAddress();

	// Returns a string containing info of the CPU stack
	std::string FrameInfo() const;

	// Read byte from RAM address
	uint8_t Read(uint16_t) const;

	// Read address from RAM address
	uint16_t ReadAddress(uint16_t) const;

	// Read byte from last used address
	uint8_t ReadFromLastAddress() const;

	// Read byte from ROM
	uint8_t ReadROM();

	// Read address from ROM
	uint16_t ReadROMAddress();

	// Reset CPU state
	void Reset();

	void SetResetVector(uint16_t);

	// Read byte from stack
	uint8_t StackRead();

	// Read address from stack
	uint16_t StackReadAddress();

	// Write byte to stack
	void StackWrite(uint8_t);

	// Write address to stack
	void StackWriteAddress(uint16_t);

	// Return state register as a byte
	uint8_t StateByte() const;

	// Write byte to RAM address
	void Write(uint16_t, uint8_t);

	// Write address to RAM
	void WriteAddress(uint16_t, uint16_t);

	// Fetch an address, write to it, and return the address
	uint16_t WriteToFetchedAddress(uint8_t);

	// Write byte to last used address
	void WriteToLastAddress(uint8_t);

	// None of these should be inlined, as we need them to be addressable.
	// All return the number of additional cycles possibly needed.

	// --- addressing modes ---

	// Absolute address mode
	uint8_t Absolute();

	// Absolute address mode with X register offset
	uint8_t AbsoluteX();

	// Absolute address mode with Y register offset
	uint8_t AbsoluteY();

	// Immediate address mode
	uint8_t Immediate();

	// Implied address mode
	uint8_t Implied();

	// Indirect address mode (pointer access)
	uint8_t Indirect();

	// Indirect address mode of zero-page with X register offset
	uint8_t IndirectX();

	// Indirect address mode of zero-page with X register offset
	uint8_t IndirectY();

	// Relative address mode (branching)
	uint8_t Relative();

	// Zero-page address mode
	uint8_t ZeroPage();

	// Zero-page address mode with X register offset
	uint8_t ZeroPageX();

	// Zero-page address mode with X register offset
	uint8_t ZeroPageY();


	/// --- branching ---

	// Branch if carry bit clear
	uint8_t BranchCarryClear();

	// Branch if carry bit set
	uint8_t BranchCarrySet();

	// Branch if equal
	uint8_t BranchEqual();

	// Branch if negative
	uint8_t BranchNegative();

	// Branch if not equal
	uint8_t BranchNotEqual();

	// Branch if overflow bit clear
	uint8_t BranchOverflowClear();

	// Branch if overflow bit set
	uint8_t BranchOverflowSet();

	// Branch if positive
	uint8_t BranchPositive();


	// --- state manipulation ---

	// Clear carry bit
	uint8_t ClearCarry();

	// Clear decimal mode bit
	uint8_t ClearDecimal();

	// Clear disable interrupts bit
	uint8_t ClearDisableInterrupts();

	// Clear overflow bit
	uint8_t ClearOverflow();

	// Set carry bit
	uint8_t SetCarry();

	// Set decimal mode bit
	uint8_t SetDecimal();

	// Set disable interrupts bit
	uint8_t SetDisableInterrupts();


	// --- interrupts ---

	// Program-sourced interrupt
	uint8_t Break();

	// Interrupt request
	uint8_t InterruptRequest();

	// Return from interrupt
	uint8_t InterruptReturn();

	// Non-maskable interrupt
	uint8_t NonMaskableInterrupt();

	// Return from subroutine
	uint8_t SubroutineReturn();


	// --- stack manipulation ---

	// Pop accumulator from stack
	uint8_t PopAccumulator();

	// Pop state from stack
	uint8_t PopState();

	// Push accumulator to stack
	uint8_t PushAccumulator();

	// Push state to stack
	uint8_t PushState();


	// --- arithmetic ---

	// Add with carry
	uint8_t Add();

	// Decrement value at location
	uint8_t Decrement();

	// Decrement X register
	uint8_t DecrementX();

	// Decrement Y register
	uint8_t DecrementY();

	// Increment value at location
	uint8_t Increment();

	// Increment X register
	uint8_t IncrementX();

	// Increment Y register
	uint8_t IncrementY();

	// Subtract with carry
	uint8_t Subtract();


	// --- bitwise --

	// Bitwise and
	uint8_t And();

	// Bitwise or
	uint8_t Or();

	// Rotate left
	uint8_t RotateLeft();

	// Rotate right
	uint8_t RotateRight();

	// Arithmetical shift left
	uint8_t ShiftLeft();

	// Logical shift right
	uint8_t ShiftRight();

	// Exclusive or
	uint8_t Xor();


	// --- comparison ---

	// Compare accumulator
	uint8_t Compare();

	// Compare X register
	uint8_t CompareX();

	// Compare Y register
	uint8_t CompareY();

	// Bit test
	uint8_t TestBit();


	// --- jumps ---

	// Jump to location
	uint8_t Jump();

	// Jump to subroutine
	uint8_t SubroutineJump();


	// --- load/store ---

	// Load accumulator
	uint8_t LoadAccumulator();

	// Load X register
	uint8_t LoadX();

	// Load Y register
	uint8_t LoadY();

	// Store accumulator
	uint8_t StoreAccumulator();

	// Store X register
	uint8_t StoreX();

	// Store Y register
	uint8_t StoreY();


	// --- transfers ---

	// Transfer accumulator to X register
	uint8_t AccumulatorToX();

	// Transfer accumulator to Y register
	uint8_t AccumulatorToY();

	// Transfer stack pointer to X register
	uint8_t StackPtrToX();

	// Transfer X register to accumulator
	uint8_t XToAccumulator();

	// Transfer X register to stack pointer
	uint8_t XToStackPtr();

	// Transfer Y register to accumulator
	uint8_t YToAccumulator();


	// --- illegals ---

	// AND + LSR
	uint8_t ALR();

	// AND + SEC
	uint8_t ANC();

	// Magic AND X AND operand
	uint8_t ANE();

	// AND + ROR
	uint8_t ARR();

	// DEC + CMP
	uint8_t DCP();

	// INC + SBC
	uint8_t ISC();

	// Jams the CPU into a freeze, requiring a reset
	uint8_t JAM();

	// LDA + TSX
	uint8_t LAS();

	// LDA + LDX
	uint8_t LAX();

	// Store Magic AND operand in accumulator and X
	uint8_t LXA();

	// No operation
	uint8_t NOP();

	// ROL + AND
	uint8_t RLA();

	// ROR + ADC
	uint8_t RRA();

	// Store accumulator AND X
	uint8_t SAX();

	// Accumulator AND X + SBC
	uint8_t SBX();

	// Store accumulator AND X AND high address + 1
	uint8_t SHA();

	// Store X register AND high address + 1
	uint8_t SHX();

	// Store Y register AND high address + 1
	uint8_t SHY();

	// Store ASL + ORA
	uint8_t SLO();

	// Store LSR + EOR
	uint8_t SRE();

	// Transfer accumulator AND X to stack pointer
	uint8_t TAS();

	// SBC + NOP
	uint8_t USBC();
private:
	static constexpr uint16_t InterruptRequestAddress = 0xFFFE;

	// for some unstable illegal opcodes
	static constexpr uint8_t MagicValue = 255;

	static constexpr uint16_t NonMaskableInterruptAddress = 0xFFFA;

	// Reset address
	static constexpr uint16_t ResetAddress = 0xFFFC;

	// Reset vector address
	static constexpr uint16_t ResetVectorAddress = 0xFFFD;

	// stack offset in RAM
	static constexpr uint16_t StackBaseAddress = 256;

	// stack pointer initial offset
	static constexpr uint16_t StackPtrInitAddress = 253;

	static const std::array<Opcode6502, 256> Ops;

	uint8_t m_cache; // last read byte
	uint8_t m_cycles; // remaining cycles for current operation
	uint8_t m_lastOp;
	uint8_t m_lastRelAddr;
	uint16_t m_lastAbsAddr;

	struct Registers
	{
		uint8_t a; // accumulator
		uint8_t x;
		uint8_t y;
		uint8_t sp; // stack pointer

		struct State
		{
			bool
				c : 1, // carry
				z : 1, // zero
				i : 1, // no interrupts
				d : 1, // decimal mode
				b : 1, // break
				u : 1, // unused
				v : 1, // overflow
				n : 1; // negative
		} p;

		uint16_t pc; // program counter
	} m_regs;

	std::vector<Disassembly6502> m_disasm;

	// Common functionality for branch instructions
	void Branch();

	// Check the address mode, either assigning to accumulator, or writing to cached address
	void CheckAddressMode(uint16_t);

	// (Re)initialise flags register
	void InitializeState();

	// Common functionality for interrupt operations
	void Interrupt(uint16_t, uint8_t);

	// Unstable magic value equation for some illegal opcodes
	uint8_t Magic() const;

	// Set carry, negative, and/or zero bits of flags register, given a 16-bit value
	void SetCarryNegativeZero(uint16_t);

	// Set negative and/or zero bits of flags register, given a value
	void SetNegativeZero(uint16_t);
};

#endif // _6502_DEVICES_H
