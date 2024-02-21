#ifndef _6500_DEVICES_H
#define _6500_DEVICES_H

#include "../generic/bus_le16.h"

class MOS6500;

typedef uint8_t (MOS6500::*Instruction6500)();

// Metadata for the CPU's various operations
struct Opcode6500
{
	// Constructor needed because otherwise we'll get an array error
	Opcode6500(uint8_t, Instruction6500, Instruction6500, const char *);

	uint8_t Cycles;
	Instruction6500 AddressMode;
	Instruction6500 Operation;
	const char *Mnemonic;
};


// The CPU processes data available via its bus
class MOS6500 : public CPU
{
	//friend Debugger6500;
	static const std::array<Opcode6500, 256> OPS;
public:
	// Allocate a new CPU, given a parent bus, and start and end addresses in RAM
	MOS6500(BusLE16 *, uint16_t, uint16_t);

	// CPU clock operation (execute one instruction)
	void Clock() override;

	size_t Cycles() const;

	// Disassemble from the specified address for the specified length
	Disassembly Disassemble(size_t) override;

	// Read address from RAM
	size_t FetchAddress() override;

	// Fetch and cache a byte from the cached absolute address
	uint8_t FetchByte() override;

	// Returns a string containing info of the CPU stack
	std::string FrameInfo();

	Disassembly & LastDisassembly();

	// Read address from ROM
	size_t ReadROMAddress() override;

	// Reset CPU state
	void Reset() override;

	std::array<uint8_t, 256> StackFrame();

	// Read address from stack
	size_t StackReadAddress() override;

	// Write address to stack
	void StackWriteAddress(size_t) override;

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
	uint8_t BCC();

	// Branch if carry bit set
	uint8_t BCS();

	// Branch if equal
	uint8_t BEQ();

	// Branch if negative
	uint8_t BMI();

	// Branch if not equal
	uint8_t BNE();

	// Branch if positive
	uint8_t BPL();

	// Branch if overflow bit clear
	uint8_t BVC();

	// Branch if overflow bit set
	uint8_t BVS();


	// --- state manipulation ---

	// Clear carry bit
	uint8_t CLC();

	// Clear decimal mode bit
	uint8_t CLD();

	// Clear disable interrupts bit
	uint8_t CLI();

	// Clear overflow bit
	uint8_t CLV();

	// Set carry bit
	uint8_t SEC();

	// Set decimal mode bit
	uint8_t SED();

	// Set disable interrupts bit
	uint8_t SEI();


	// --- interrupts ---

	// Program-sourced interrupt
	uint8_t BRK();

	// Interrupt request
	uint8_t IRQ();

	// Non-maskable interrupt
	uint8_t NMI();

	// Return from interrupt
	uint8_t RTI();

	// Return from subroutine
	uint8_t RTS();


	// --- stack manipulation ---

	// Push accumulator to stack
	uint8_t PHA();

	// Push state to stack
	uint8_t PHP();

	// Pop accumulator from stack
	uint8_t PLA();

	// Pop state from stack
	uint8_t PLP();


	// --- arithmetic ---

	// Add with carry
	uint8_t ADC();

	// Decrement value at location
	uint8_t DEC();

	// Decrement X register
	uint8_t DEX();

	// Decrement Y register
	uint8_t DEY();

	// Increment value at location
	uint8_t INC();

	// Increment X register
	uint8_t INX();

	// Increment Y register
	uint8_t INY();

	// Subtract with carry
	uint8_t SBC();


	// --- bitwise --

	// Bitwise and
	uint8_t AND();

	// Arithmetical shift left
	uint8_t ASL();

	// Exclusive or
	uint8_t EOR();

	// Logical shift right
	uint8_t LSR();

	// Bitwise or
	uint8_t ORA();

	// Rotate left
	uint8_t ROL();

	// Rotate right
	uint8_t ROR();


	// --- comparison ---

	// Bit test
	uint8_t BIT();

	// Compare accumulator
	uint8_t CMP();

	// Compare X register
	uint8_t CPX();

	// Compare Y register
	uint8_t CPY();


	// --- jumps ---

	// Jump to location
	uint8_t JMP();

	// Jump to subroutine
	uint8_t JSR();


	// --- load/store ---

	// Load accumulator
	uint8_t LDA();

	// Load X register
	uint8_t LDX();

	// Load Y register
	uint8_t LDY();

	// Store accumulator
	uint8_t STA();

	// Store X register
	uint8_t STX();

	// Store Y register
	uint8_t STY();


	// --- transfers ---

	// Transfer accumulator to X register
	uint8_t TAX();

	// Transfer accumulator to Y register
	uint8_t TAY();

	// Transfer stack pointer to X register
	uint8_t TSX();

	// Transfer X register to accumulator
	uint8_t TXA();

	// Transfer X register to stack pointer
	uint8_t TXS();

	// Transfer Y register to accumulator
	uint8_t TYA();


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
	uint8_t m_cache; // last read byte
	uint8_t m_cycles; // remaining cycles for current operation
	uint8_t m_lastOp;

	struct Registers
	{
		uint8_t a; // accumulator
		uint8_t x;
		uint8_t y;

		union
		{
			struct
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

			uint8_t state;
		};
	} m_regs;

	Disassembly m_lastDisasm;

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

#endif // _6500_DEVICES_H
