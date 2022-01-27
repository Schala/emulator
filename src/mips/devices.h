#ifndef _MIPS_DEVICES_H
#define _MIPS_DEVICES_H

#include <array>

#include "../core/devices.h"

struct MIPSInstruction
{
	union
	{
		struct
		{
			uint32_t
				op : 6, // opcode
				rs : 5, // source register #
				rt : 5, // target register # or branch condition
				imm : 16; // immediate value, branch displacement or address displacement
		} Immediate;

		struct
		{
			uint32_t
				op : 6, // opcode
				t : 26; // target address
		} Jump;

		struct
		{
			uint32_t
				op : 6, // opcode
				rs : 5, // source register #
				rt : 5, // target register # or branch condition
				rd : 5, // destination register #
				sa : 5, // shift amount
				f : 6; // function
		} Register;
	};

	void (MIPS::*Operation)();
};

class MIPS : public Device
{
public:
	MIPS(Bus *, uint64_t, uint64_t);

	void Clock();

	void Disassemble(uint64_t, uint64_t);
private:
	struct Registers
	{
		uint8_t ll : 1; // load/link bit
		float ir; // implementation/revision
		float cs; // control/tatus
		const uint64_t zero; // 1st GPR always 0 and read only
		std::array<uint64_t, 30> gp; // general purpose
		uint64_t l; // link
		std::array<double, 32> fp; // floating point

		// multiply and divide
		uint64_t hi;
		uint64_t lo;

		uint64_t pc; // counter
	} m_regs;

	struct CP0Registers
	{
		uint64_t i; // TLB array pointer
		uint64_t r; // pseudorandom pointer into TLB array (read only)
		std::array<uint64_t, 2> el; // low half of TLB entry for even and odd virtual address (VPN)
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
	} m_cp0Regs;


	// --- arithmetic ---

	void Add();

	void AddImmediate();

	void AddImmediateUnsigned();

	void AddUnsigned();

	void Divide();

	void DivideUnsigned();

	void DWordAdd();

	void DWordAddImmediate();

	void DWordAddImmediateUnsigned();

	void DWordAddUnsigned();

	void DWordDivide();

	void DWordDivideUnsigned();

	void DWordMultiply();

	void DWordMultiplyUnsigned();

	void DWordSubtract();

	void DWordSubtractUnsigned();

	void Multiply();

	void MultiplyUnsigned();

	void Subtract();

	void SubtractUnsigned();


	// -- bitwise ---

	void And();

	void AndImmediate();

	void DWordShiftLeft();

	void DWordShiftLeft32();

	void DWordShiftLeftVariable();

	void DWordShiftRightA();

	void DWordShiftRightA32();

	void DWordShiftRightAVariable();

	void DWordShiftRightL();

	void DWordShiftRightL32();

	void DWordShiftRightLVariable();

	void Nor();

	void Or();

	void OrImmediate();

	void ShiftLeft();

	void ShiftLeftVariable();

	void ShiftRightA();

	void ShiftRightAVariable();

	void ShiftRightL();

	void ShiftRightLVariable();

	void Xor();

	void XorImmediate();


	// --- branching ---

	void BranchEqual();

	void BranchEqualLikely();

	void BranchGreater0();

	void BranchGreater0Likely();

	void BranchGreaterEqual0();

	void BranchGreaterEqual0LinkLikely();

	void BranchLess0();

	void BranchLess0Likely();

	void BranchLess0Link();

	void BranchLess0LinkLikely();

	void BranchLessEqual0();

	void BranchLessEqual0Likely();

	void BranchNotEqual();

	void BranchNotEqualLikely();


	// --- interrupts ---

	void Break();


	// --- moving ---

	void DWordMoveFromC0();

	void DWordMoveToC0();

	void MoveFromC0();

	void MoveFromHi();

	void MoveFromLo();

	void MoveToC0();

	void MoveToHi();

	void MoveToLo();


	// --- jump ---

	void ExceptionReturn();

	void Jump();

	void JumpLink();

	void JumpLinkRegister();

	void JumpRegister();

	void SystemCall();


	// --- load/store ---

	void LoadByte();

	void LoadByteUnsigned();

	void LoadDWord();

	void LoadDWordLeft();

	void LoadDWordRight();

	void LoadHWord();

	void LoadHWordUnsigned();

	void LoadLinked();

	void LoadLinkedDWord();

	void LoadUpperImmediate();

	void LoadWord();

	void LoadWordUnsigned();

	void StoreByte();

	void StoreConditional();

	void StoreConditionalDWord();

	void StoreDWord();

	void StoreDWordLeft();

	void StoreDWordRight();

	void StoreHWord();

	void StoreWord();

	void StoreWordLeft();

	void StoreWordRight();


	// --- state control ---

	void SetLessThan();

	void SetLessThanImmediate();

	void SetLessThanImmediateUnsigned();

	void SetLessThanUnsigned();


	// --- trapping ---

	void TrapEqual();

	void TrapEqualImmediate();

	void TrapGreaterEqual();

	void TrapGreaterEqualImmediate();

	void TrapGreaterEqualImmediateUnsigned();

	void TrapGreaterEqualUnsigned();

	void TrapLessThan();

	void TrapLessThanImmediate();

	void TrapLessThanImmediateUnsigned();

	void TrapLessThanUnsigned();

	void TrapNotEqual();

	void TrapNotEqualImmediate();


	// --- TLB ---

	void TLBProbe();

	void TLBRead();

	void TLBWriteIndexed();

	void TLBWriteRandom();


	// --- misc ---

	void Sync();
};

#endif // _MIPS_DEVICES_H
