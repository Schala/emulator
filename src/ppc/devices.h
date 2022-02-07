#ifndef _PPC_DEVICES_H
#define _PPC_DEVICES_H

#include <array>

#include "../core/devices.h"

class PPC
{
public:
	// instruction set

	// --- load/store ---

	void LBZ();

	void LBZU();

	void LBZX();

	void LHA();

	void LHAU();

	void LHAX();

	void LHZ();

	void LHZU();

	void LHZX();

	void LMW();

	void LWZ();

	void STB();

	void STBU();

	void STBX();

	void STH();

	void STHU();

	void STHX();

	void STMW();

	void STW();

	void STWU();

	void STWX();


	// --- arithmetic ---

	void ADD();

	void ADDC();

	void ADDCO();

	void ADDE();

	void ADDEO();

	void ADDI();

	void ADDIC();

	void ADDIS();

	void ADDME();

	void ADDMEO();

	void ADDO();

	void ADDZE();

	void ADDZEO();

	void DIVW();

	void DIVWO();

	void DIVWU();

	void DIVWUO();

	void MULHW();

	void MULLI();

	void MULLW();

	void NEG();

	void NEGO();

	void SUBF();

	void SUBFC();

	void SUBFCO();

	void SUBFE();

	void SUBFEO();

	void SUBFIC();

	void SUBFME();

	void SUBFMEO();

	void SUBFZE();

	void SUBFZEO();


	// --- bitwise ---

	void AND();

	void ANDC();

	void ANDI();

	void ANDIS();

	void CNTLZW();

	void CRAND();

	void CRANDC();

	void CRCLR();

	void CREQV();

	void CRMOVE();

	void CRNAND();

	void CRNOR();

	void CRNOT();

	void CROR();

	void CRORC();

	void CRSET();

	void CRXOR();

	void EQV();

	void EXTSB();

	void EXTSH();

	void NAND();

	void NOR();

	void OR();

	void ORI();

	void ORIS();

	void RLWIMI();

	void RLWINM();

	void RLWNM();

	void SLW();

	void SRAW();

	void SRAWI();

	void SRW();

	void XOR();

	void XORI();

	void XORIS();


	// --- comparison ---

	void CMP();

	void CMPI();

	void CMPL();

	void CMPLI();


	// --- move ---

	void MCRF();

	void MCRXR();

	void MFCR();

	void MFSPR();

	void MTCR();

	void MTCRF();

	void MTSPR();


	// --- branching ---

	void B();

	void BA();

	void BC();

	void BCA();

	void BCCTR();

	void BCCTRL();

	void BCL();

	void BCLR();

	void BCLRL();

	void BEQ();

	void BEQA();

	void BEQL();

	void BGE();

	void BGEA();

	void BGEL();

	void BGT();

	void BGTA();

	void BGTL();

	void BL();

	void BLE();

	void BLEA();

	void BLEL();

	void BLT();

	void BLTA();

	void BLTL();

	void BNE();

	void BNEA();

	void BNEL();

	void BNG();

	void BNGA();

	void BNGL();

	void BNL();

	void BNLA();

	void BNLL();

	void BNS();

	void BNSA();

	void BNSL();

	void BSO();

	void BSOA();

	void BSOL();


	// --- traps ---

	void SC();

	void TW();

	void TWI();
private:
	struct Registers
	{
		uint32_t cr; // condition
		uint32_t lr; // link
		uint32_t ctr; // count
		std::array<uint32_t, 32> r; // general
		uint32_t xer; // float exception
		uint32_t fpscr;
		std::array<double, 32> f; // floating point
	} m_regs;
};

#endif // _PPC_DEVICES_H
