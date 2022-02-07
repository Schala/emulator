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

	void ADD();

	void ADDI();

	void ADDIU();

	void ADDU();

	void DADD();

	void DADDI();

	void DADDIU();

	void DADDU();

	void DDIV();

	void DDIVU();

	void DIV();

	void DIVU();

	void DMULT();

	void DMULTU();

	void DSLL();

	void DSLL32();

	void DSLLV();

	void DSRA();

	void DSRA32();

	void DSRAV();

	void DSRL();

	void DSRL32();

	void DSRLV();

	void DSUB();

	void DSUBU();

	void MUL();

	void MULT();

	void MULTU();

	void SUB();

	void SUBU();


	// -- bitwise ---

	void AND();

	void ANDI();

	void NOR();

	void OR();

	void ORI();

	void SLL();

	void SLLV();

	void SRA();

	void SRAV();

	void SRL();

	void SRLV();

	void XOR();

	void XORI();


	// --- branching ---

	void BEQ();

	void BEQL();

	void BGEZ();

	void BGEZAL();

	void BGEZALL();

	void BGEZL();

	void BGTZ();

	void BGTZL();

	void BLE();

	void BLEZ();

	void BLEZL();

	void BLTZ();

	void BLTZAL();

	void BLTZALL();

	void BLTZL();

	void BNE();

	void BNEL();


	// --- interrupts ---

	void BREAK();

	// --- moving ---

	void MFHI();

	void MFLO();

	void MOVN();

	void MOVZ();

	void MTHI();

	void MTLO();


	// --- jump ---

	void J();

	void JAL();

	void JALR();

	void JR();

	void SYSCALL();


	// --- load/store ---

	void LA();

	void LB();

	void LBU();

	void LD();

	void LDL();

	void LDR();

	void LDXC1();

	void LH();

	void LHU();

	void LI();

	void LL();

	void LLD();

	void LUI();

	void LW();

	void LWL();

	void LWR();

	void LWU();

	void LWXC1();

	void SB();

	void SC();

	void SCD();

	void SD();

	void SDL();

	void SDR();

	void SDXC1();

	void SH();

	void SW();

	void SWL();

	void SWR();

	void SWXC1();


	// --- state control ---

	void SLT();

	void SLTI();

	void SLTIU();


	// --- trapping ---

	void TEQ();

	void TEQI();

	void TGE();

	void TGEI();

	void TGEIU();

	void TGEU();

	void TLT();

	void TLTI();

	void TLTIU();

	void TLTU();

	void TNE();

	void TNEI();


	// --- serialisation ---

	void SYNC();


	// --- prefetch ---

	void PREF();

	void PREFX();
};

#endif // _MIPS_DEVICES_H
