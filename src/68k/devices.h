#ifndef _68K_DEVICES_H
#define _68K_DEVICES_H

#include <array>

#include "../core/devices.h"

class M68K : public Device
{
private:
	struct Registers
	{
		struct State
		{
			uint16_t
				t : 2,
				s : 1,
				m : 1,
				_4 : 1,
				i : 3, // interrupt
				_8 : 3,
				x : 1, // extend
				n : 1, // negative
				z : 1, // zero
				v : 1, // overflow
				c : 1; // carry
		} p;

		std::array<uint32_t, 8> d;
		std::array<uint32_t, 7> a;
		uint32_t sp;
		uint32_t pc;
	} m_regs;

	// Instructions

	// --- arithmetic ---

	void ABCD();

	void ADD();

	void ADDA();

	void ADDI();

	void ADDQ();

	void ADDX();

	void DIVS();

	void DIVU();

	void MULS();

	void MULU();

	void NBCD();

	void NEG();

	void NEGX();

	void SBCD();

	void SUB();

	void SUBA();

	void SUBI();

	void SUBQ();

	void SUBX();


	// -- bitwise ---

	void AND();

	void ANDI();

	void ASL();

	void ASR();

	void EOR();

	void EORI();

	void LSL();

	void LSR();

	void NOT();

	void OR();

	void ORI();

	void ROL();

	void ROR();

	void ROXL();

	void ROXR();


	// -- branching ---

	void BCC();

	void BCS();

	void BEQ();

	void BGE();

	void BGT();

	void BHI();

	void BHS();

	void BLE();

	void BLO();

	void BLS();

	void BLT();

	void BMI();

	void BNE();

	void BPL();

	void BRA();

	void BSR();

	void BVC();

	void BVS();

	void DBCC();

	void DBEQ();

	void DBF();

	void DBRA();

	void DBT();


	// --- bit manipulation ---

	void BCHG();

	void BCLR();

	void BSET();


	// --- comparison ---

	void BTST();

	void CHK();

	void CMP();

	void CMPA();

	void CMPI();

	void CMPM();

	void TAS();

	void TST();


	// --- register control ---

	void EXG();

	void EXT();

	void SWAP();


	// --- jumping ---

	void JMP();

	void JSR();

	void RTE();

	void RTR();

	void RTS();


	// --- load/store ---

	void LEA();

	void PEA();


	// --- move ---

	void MOVE();

	void MOVEA();

	void MOVEM();

	void MOVEP();

	void MOVEQ();


	// --- set conditionals ---

	void SCC();

	void SCS();

	void SEQ();

	void SF();

	void SGE();

	void SGT();

	void SHI();

	void SLE();

	void SLS();

	void SLT();

	void SMI();

	void SNE();

	void SPL();

	void ST();

	void SVC();

	void SVS();


	// --- misc ---

	void CLR();

	void ILLEGAL();

	void LINK();

	void NOP();

	void RESET();

	void STOP();

	void TRAP();

	void TRAPV();

	void UNLK();
};

#endif // _68K_DEVICES_H
