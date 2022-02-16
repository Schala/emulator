#ifndef _65C816_DEVICES_H
#define _65C816_DEVICES_H

#include "../core/devices.h"

class WDC65C816 : public Processor
{
public:
	WDC65C816(Bus65C816 *, uint16_t, uint16_t);

	void Clock();
protected:
	struct
	{
		// processor state
		struct
		{
			bool
				c : 1, // carry
				z : 1, // zero
				i : 1, // interrupt disable
				d : 1, // decimal
				x : 1, // index register size (0 = 16 bit, 1 = 8 bit)
				m : 1, // accumulator register size (0 = 16 bit, 1 = 8 bit)
				v : 1, // overflow
				n : 1; // negative
		} p;

		uint16_t a; // accumulator
		uint16_t x; // index
		uint16_t y; // index
		uint32_t dp : 24; // direct page
		uint32_t db : 24; // data bank
		uint32_t pb : 24; // program bank
	} m_regs;

	// --- address modes ---

	uint8_t Absolute();

	uint8_t AbsoluteIndexedIndirect();

	uint8_t AbsoluteIndexedLong();

	uint8_t AbsoluteIndexedX();

	uint8_t AbsoluteIndexedY();

	uint8_t AbsoluteIndirect();

	uint8_t AbsoluteIndirectLong();

	uint8_t AbsoluteLong();

	uint8_t Direct();

	uint8_t DirectIndexedIndirect();

	uint8_t DirectIndexedX();

	uint8_t DirectIndexedY();

	uint8_t DirectIndirect();

	uint8_t DirectIndirectIndexed();

	uint8_t DirectIndirectIndexedLong();

	uint8_t DirectIndirectLong();

	uint8_t Immediate();

	uint8_t Implied();

	uint8_t Move();

	uint8_t Relative();

	uint8_t RelativeLong();

	uint8_t StackRelative();

	uint8_t StackRelativeIndirectIndexed();


	// --- arithmetic ---

	uint8_t ADC();

	uint8_t DEC();

	uint8_t DEX();

	uint8_t DEY();

	uint8_t INC();

	uint8_t INX();

	uint8_t INY();

	uint8_t SBC();


	// -- bitwise ---

	uint8_t AND();

	uint8_t ASL();

	uint8_t EOR();

	uint8_t LSR();

	uint8_t ORA();

	uint8_t ROL();

	uint8_t ROR();


	// --- branching ---

	uint8_t BCC();

	uint8_t BCS();

	uint8_t BEQ();

	uint8_t BMI();

	uint8_t BNE();

	uint8_t BPL();

	uint8_t BRA();

	uint8_t BRL();

	uint8_t BVC();

	uint8_t BVS();


	// --- comparison ---

	uint8_t BIT();

	uint8_t CMP();

	uint8_t CPX();

	uint8_t CPY();

	uint8_t TRB();

	uint8_t TSB();


	// --- interrupts ---

	uint8_t BRK();

	uint8_t RTI();

	uint8_t WAI();


	// --- state control ---

	uint8_t CLC();

	uint8_t CLD();

	uint8_t CLI();

	uint8_t CLV();

	uint8_t REP();

	uint8_t SEC();

	uint8_t SED();

	uint8_t SEI();

	uint8_t SEP();

	uint8_t XCE();


	// --- jumping ---

	uint8_t JMP();

	uint8_t JSR();

	uint8_t RTS();



	// --- load/store ---

	uint8_t LDA();

	uint8_t LDX();

	uint8_t LDY();

	uint8_t STA();

	uint8_t STP();

	uint8_t STX();

	uint8_t STY();

	uint8_t STZ();


	// --- moving ---

	uint8_t MVN();

	uint8_t MVP();


	// --- stack control ---

	uint8_t PEA();

	uint8_t PHA();

	uint8_t PHB();

	uint8_t PHD();

	uint8_t PEI();

	uint8_t PHK();

	uint8_t PER();

	uint8_t PHP();

	uint8_t PHX();

	uint8_t PHY();

	uint8_t PLA();

	uint8_t PLB();

	uint8_t PLD();

	uint8_t PLP();

	uint8_t PLX();

	uint8_t PLY();


	// --- transfers ---

	uint8_t TAX();

	uint8_t TAY();

	uint8_t TCD();

	uint8_t TCS();

	uint8_t TDC();

	uint8_t TSC();

	uint8_t TSX();

	uint8_t TXA();

	uint8_t TXS();

	uint8_t TXY();

	uint8_t TYA();

	uint8_t TYX();

	uint8_t XBA();


	// --- misc ---

	uint8_t COP();

	uint8_t NOP();
};

#endif // _65C816_DEVICES_H
