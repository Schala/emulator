#ifndef _65C816_DEVICES_H
#define _65C816_DEVICES_H

#include "../core/devices.h"

class Bus65C816 : public Bus
{
public:
};

class WDC65C816 : public Device
{
public:
	WDC65C816(Bus65C816 *, uint16_t, uint16_t);
protected:
	struct
	{
		// processor state
		struct
		{
			bool
				n : 1, // negative
				v : 1, // overflow
				m : 1, // accumulator register size (0 = 16 bit, 1 = 8 bit)
				x : 1, // index register size (0 = 16 bit, 1 = 8 bit)
				d : 1, // decimal
				i : 1, // interrupt disable
				z : 1, // zero
				c : 1; // carry
		} p;

		uint16_t a; // accumulator
		uint16_t b; // accumulator
		uint16_t x; // index
		uint16_t y; // index
		uint32_t dp : 24; // direct page
		uint32_t s : 24; // stack pointer
		uint32_t db : 24; // data bank
		uint32_t pb : 24; // program bank
		uint32_t pc : 24; // counter
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

	uint8_t Add();

	uint8_t Decrement();

	uint8_t DecrementX();

	uint8_t DecrementY();

	uint8_t Increment();

	uint8_t IncrementX();

	uint8_t IncrementY();

	uint8_t Subtract();


	// -- bitwise ---

	uint8_t And();

	uint8_t Or();

	uint8_t RotateLeft();

	uint8_t RotateRight();

	uint8_t ShiftLeft();

	uint8_t ShiftRight();

	uint8_t Xor();


	// --- branching ---

	uint8_t Branch();

	uint8_t BranchCarryClear();

	uint8_t BranchCarrySet();

	uint8_t BranchEqual();

	uint8_t BranchLong();

	uint8_t BranchNegative();

	uint8_t BranchNotEqual();

	uint8_t BranchOverflowClear();

	uint8_t BranchPositive();


	// --- comparison ---

	uint8_t Compare();

	uint8_t CompareX();

	uint8_t CompareY();

	uint8_t TestBit();

	uint8_t TestResetBits();

	uint8_t TestSetBits();


	// --- interrupts ---

	uint8_t Break();

	uint8_t InterruptReturn();

	uint8_t Wait();


	// --- state control ---

	uint8_t ClearCarry();

	uint8_t ClearDecimal();

	uint8_t ClearInterruptDisable();

	uint8_t ClearOverflow();

	uint8_t ExchangeCarryEmulation();

	uint8_t ResetState();

	uint8_t SetCarry();

	uint8_t SetDecimal();

	uint8_t SetInterruptDisable();

	uint8_t SetState();


	// --- jumping ---

	uint8_t Jump();

	uint8_t JumpLong();

	uint8_t SubroutineJump();

	uint8_t SubroutineJumpLong();

	uint8_t SubroutineReturn();

	uint8_t SubroutineReturnLong();


	// --- load/store ---

	uint8_t LoadAccumulator();

	uint8_t LoadX();

	uint8_t LoadY();

	uint8_t Store0();

	uint8_t StoreAccumulator();

	uint8_t StoreState();

	uint8_t StoreX();

	uint8_t StoreY();


	// --- moving ---

	uint8_t MoveNegative();

	uint8_t MovePositive();


	// --- stack control ---

	uint8_t PopAccumulator();

	uint8_t PopDataBank();

	uint8_t PopDirectPage();

	uint8_t PopState();

	uint8_t PopX();

	uint8_t PopY();

	uint8_t PushAbsolute();

	uint8_t PushAccumulator();

	uint8_t PushDataBank();

	uint8_t PushDirectPage();

	uint8_t PushIndirect();

	uint8_t PushProgramBank();

	uint8_t PushRelativeIndirect();

	uint8_t PushState();

	uint8_t PushX();

	uint8_t PushY();


	// --- transfers ---

	uint8_t AccumulatorToDirectPage();

	uint8_t AccumulatorToStackPtr();

	uint8_t AccumulatorToX();

	uint8_t AccumulatorToY();

	uint8_t DirectPageToAccumulator();

	uint8_t ExchangeBA();

	uint8_t StackPtrToAccumulator();

	uint8_t StackPtrToX();

	uint8_t XToAccumulator();

	uint8_t XToStackPtr();

	uint8_t XToY();

	uint8_t YToAccumulator();

	uint8_t YToX();


	// --- misc ---

	uint8_t Coprocessor();

	uint8_t NoOperation();
};

#endif // _65C816_DEVICES_H
