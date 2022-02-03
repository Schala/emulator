#include <bit>
#include <fmt/core.h> // todo: <format>
#include <stdexcept>
#include <utility>
#include <version>

#include "devices.h"

using namespace std::literals:string_view_literals;

static constexpr uint16_t Hi16(uint16_t value)
{
	return value & 0xFF00;
}

// Bus6502

Bus6502::Bus6502(size_t ramSize): Bus(ramSize)
{
}

uint8_t Bus6502::Read(uint16_t addr) const
{
	return m_ram.at(addr);
}

void Bus6502::Write(uint16_t addr, uint8_t data)
{
	m_ram[addr] = data;
}

// Opcode6502

Opcode6502::Opcode6502(uint8_t cycles, Instruction6502 addrMode, Instruction6502 op,
	std::string_view mneumonic):
	Cycles(cycles),
	AddressMode(addrMode),
	Operation(op),
	Mneumonic(std::move(mneumonic))
{
}


// MOS6502

const std::array<Opcode6502, 256> MOS6502::Ops =
{
	// 0x
	Opcode6502(7, &MOS6502::Implied, &MOS6502::Break, "brk"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::Or, "ora"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::SLO, "slo"sv),
	Opcode6502(2, &MOS6502::ZeroPage, &MOS6502::NOP, "nop"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::Or, "ora"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ShiftLeft, "asl"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::SLO, "slo"sv),
	Opcode6502(3, &MOS6502::Implied, &MOS6502::PushState, "php"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::Or, "ora"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ShiftLeft, "asl"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ANC, "anc"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::Or, "ora"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ShiftLeft, "asl"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::SLO, "slo"sv),

	// 1x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchPositive, "bpl"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::Or, "ora"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::SLO, "slo"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::Or, "ora"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ShiftLeft, "asl"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::SLO, "slo"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ClearCarry, "clc"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::Or, "ora"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::SLO, "slo"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::Or, "ora"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ShiftLeft, "asl"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::SLO, "slo"sv),

	// 2x
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::SubroutineJump, "jsr"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::And, "and"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::RLA, "rla"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::TestBit, "bit"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::And, "and"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::RotateLeft, "rol"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::RLA, "rla"sv),
	Opcode6502(4, &MOS6502::Implied, &MOS6502::PopState, "plp"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::And, "and"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::RotateLeft, "rol"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ANC, "anc"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::TestBit, "bit"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::And, "and"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::RotateLeft, "rol"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::RLA, "rla"sv),

	// 3x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchNegative, "bmi"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::And, "and"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::RLA, "rla"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::And, "and"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::RotateLeft, "rol"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::RLA, "rla"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::SetCarry, "sec"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::And, "and"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::RLA, "rla"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::And, "and"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::RotateLeft, "rol"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::RLA, "rla"sv),

	// 4x
	Opcode6502(6, &MOS6502::Implied, &MOS6502::InterruptReturn, "rti"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::Xor, "eor"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::SRE, "sre"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::NOP, "nop"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::Xor, "eor"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ShiftRight, "lsr"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::SRE, "sre"sv),
	Opcode6502(3, &MOS6502::Implied, &MOS6502::PushAccumulator, "pha"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::Xor, "eor"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ShiftRight, "lsr"sv),
	Opcode6502(2, &MOS6502::Absolute, &MOS6502::ALR, "alr"sv),
	Opcode6502(3, &MOS6502::Absolute, &MOS6502::Jump, "jmp"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::Xor, "eor"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ShiftRight, "lsr"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::SRE, "sre"sv),

	// 5x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchOverflowClear, "bvc"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::Xor, "eor"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::SRE, "sre"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::Xor, "eor"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ShiftRight, "lsr"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::SRE, "sre"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ClearDisableInterrupts, "cli"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::Xor, "eor"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::SRE, "sre"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::Xor, "eor"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ShiftRight, "lsr"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::SRE, "sre"sv),

	// 6x
	Opcode6502(6, &MOS6502::Implied, &MOS6502::SubroutineReturn, "rts"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::Add, "adc"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::RRA, "rra"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::NOP, "nop"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::Add, "adc"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::RotateRight, "ror"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::RRA, "rra"sv),
	Opcode6502(4, &MOS6502::Implied, &MOS6502::PopAccumulator, "pla"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::Add, "adc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::RotateRight, "ror"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ARR, "arr"sv),
	Opcode6502(5, &MOS6502::Indirect, &MOS6502::Jump, "jmp"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::Add, "adc"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::RotateRight, "ror"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::RRA, "rra"sv),

	// 7x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchOverflowSet, "bvs"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::Add, "adc"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::RRA, "rra"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::Add, "adc"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::RotateRight, "ror"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::RRA, "rra"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::SetDisableInterrupts, "sei"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::Add, "adc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::RRA, "rra"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::Add, "adc"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::RotateRight, "ror"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::RRA, "rra"sv),

	// 8x
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::SAX, "sax"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::StoreY, "sty"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::StoreX, "stx"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::SAX, "sax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::DecrementY, "dey"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::XToAccumulator, "txa"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ANE, "ane"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::StoreY, "sty"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::StoreX, "stx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::SAX, "sax"sv),

	// 9x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchCarryClear, "bcc"sv),
	Opcode6502(6, &MOS6502::IndirectY, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(6, &MOS6502::IndirectY, &MOS6502::SHA, "sha"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::StoreY, "sty"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::StoreX, "stx"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::SAX, "sax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::YToAccumulator, "tya"sv),
	Opcode6502(5, &MOS6502::AbsoluteY, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::XToStackPtr, "txs"sv),
	Opcode6502(5, &MOS6502::AbsoluteY, &MOS6502::TAS, "tas"sv),
	Opcode6502(5, &MOS6502::AbsoluteX, &MOS6502::SHY, "shy"sv),
	Opcode6502(5, &MOS6502::AbsoluteX, &MOS6502::StoreAccumulator, "sta"sv),
	Opcode6502(6, &MOS6502::AbsoluteY, &MOS6502::SHX, "shx"sv),
	Opcode6502(5, &MOS6502::AbsoluteY, &MOS6502::SHA, "sha"sv),

	// Ax
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LoadY, "ldy"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LoadX, "ldx"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::LAX, "lax"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LoadY, "ldy"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LoadX, "ldx"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LAX, "lax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::AccumulatorToY, "tay"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::AccumulatorToX, "tax"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LXA, "lxa"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LoadY, "ldy"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LoadX, "ldx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LAX, "lax"sv),

	// Bx
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchCarrySet, "bcs"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::LAX, "lax"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::LoadY, "ldy"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::LoadX, "ldx"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::LAX, "lax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ClearOverflow, "clv"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::StackPtrToX, "tsx"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LAS, "las"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::LoadY, "ldy"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::LoadAccumulator, "lda"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LoadX, "ldx"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LAX, "lax"sv),

	// Cx
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::CompareY, "cpy"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::Compare, "cmp"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::DCP, "dcp"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::CompareY, "cpy"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::Compare, "cmp"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::Decrement, "dec"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::DCP, "dcp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::IncrementY, "iny"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::Compare, "cmp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::DecrementX, "dex"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::SBX, "sbx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::CompareY, "cpy"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::Compare, "cmp"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::Decrement, "dec"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::DCP, "dcp"sv),

	// Dx
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchNotEqual, "bne"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::Compare, "cmp"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::DCP, "dcp"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::Compare, "cmp"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::Decrement, "dec"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::DCP, "dcp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ClearDecimal, "cld"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::Compare, "cmp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::DCP, "dcp"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::Compare, "cmp"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::Decrement, "dec"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::DCP, "dcp"sv),

	// Ex
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::CompareX, "cpx"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::ISC, "isc"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::CompareX, "cpx"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::Increment, "inc"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ISC, "isc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::IncrementX, "inx"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::USBC, "usbc"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::CompareX, "cpx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::Increment, "inc"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ISC, "isc"sv),

	// Fx
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BranchEqual, "beq"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(4, &MOS6502::IndirectY, &MOS6502::ISC, "isc"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::Increment, "inc"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ISC, "isc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::SetDecimal, "sed"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::ISC, "isc"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::Subtract, "sbc"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::Increment, "inc"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ISC, "isc"sv)
};

MOS6502::MOS6502(Bus6502 *bus, uint16_t startAddr, uint16_t endAddr):
	Device(dynamic_cast<Bus *>(bus), startAddr, endAddr),
	m_cache(0),
	m_cycles(0),
	m_lastOp(0),
	m_lastAbsAddr(0),
	m_lastRelAddr(0)
{
	m_regs.a = m_regs.x = m_regs.y = m_regs.pc = 0;
	m_regs.sp = 253;
	InitializeState();
}

void MOS6502::Branch()
{
	m_cycles++;
	m_lastAbsAddr = m_regs.pc + m_lastRelAddr;

	// need an additional cycle if different page
	if (Hi16(m_lastAbsAddr) != Hi16(m_regs.pc))
		m_cycles++;

	// jump to address
	m_regs.pc = m_lastAbsAddr;
}

void MOS6502::CheckAddressMode(uint16_t value)
{
	if (Ops[m_lastOp].AddressMode == &MOS6502::Implied)
		m_regs.a = value & 255;
	else
		WriteToLastAddress(value & 255);
}

void MOS6502::Clock()
{
	if (m_cycles == 0)
	{
		// get and increment the counter
		m_lastOp = ReadROM();

		// set cycles, see if any additional cycles are needed
		m_cycles = Ops[m_lastOp].Cycles;
		m_cycles += (this->*Ops[m_lastOp].AddressMode)();
		m_cycles += (this->*Ops[m_lastOp].Operation)();
	}

	m_cycles--;
}

void MOS6502::Disassemble(uint16_t startAddr, uint16_t endAddr)
{
	m_disasm.clear(); // dump any old disassembly

	uint16_t addr = startAddr; // for readability

	while (addr <= endAddr)
	{
		Disassembly6502 disasm;
		disasm.Address = addr;

		const Opcode6502 &op = Ops[Read(addr++)];

		std::string line = op.Mneumonic.data();

		if (op.AddressMode == &MOS6502::Immediate)
			line += fmt::format(" #${:02X}", Read(addr++));
		else if (op.AddressMode == &MOS6502::ZeroPage)
			line += fmt::format(" ${:02X}", Read(addr++));
		else if (op.AddressMode == &MOS6502::ZeroPageX)
			line += fmt::format(" ${:02X}, x", Read(addr++));
		else if (op.AddressMode == &MOS6502::ZeroPageY)
			line += fmt::format(" ${:02X}, y", Read(addr++));
		else if (op.AddressMode == &MOS6502::IndirectX)
			line += fmt::format(" (${:02X}, x)", Read(addr++));
		else if (op.AddressMode == &MOS6502::IndirectY)
			line += fmt::format(" (${:02X}, y)", Read(addr++));
		else if (op.AddressMode == &MOS6502::Absolute)
		{
			line += fmt::format(" ${:04X}", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6502::AbsoluteX)
		{
			line += fmt::format(" ${:04X}, x", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6502::AbsoluteY)
		{
			line += fmt::format(" ${:04X}, y", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6502::Indirect)
		{
			line += fmt::format(" (${:04X})", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6502::Relative)
		{
			uint8_t value = Read(addr++);
			line += fmt::format(" ${:02X} [${:04X}]", value, addr + static_cast<int8_t>(value));
		}
		else continue; // 'implied' takes no operands

		disasm.Line = line;
		m_disasm.push_back(disasm);
	}
}

uint8_t MOS6502::Fetch()
{
	if (Ops[m_lastOp].AddressMode != &MOS6502::Implied)
		m_cache = ReadFromLastAddress();
	return m_cache;
}

uint16_t MOS6502::FetchAddress()
{
	return Fetch() | (Fetch() << 8);
}

std::string MOS6502::FrameInfo() const
{
	std::string s = fmt::format("A: {:02X}\tX: {:02X}\tY: {:02X}\n", m_regs.a, m_regs.x, m_regs.y);
	s += fmt::format("SP: ${:02X}\tPC: ${:04X}\nState: ", m_regs.sp, m_regs.pc);

	s += m_regs.p.c ? 'C' : 'x';
	s += m_regs.p.z ? 'Z' : 'x';
	s += m_regs.p.i ? 'I' : 'x';
	s += m_regs.p.d ? 'D' : 'x';
	s += m_regs.p.b ? 'B' : 'x';
	s += m_regs.p.u ? 'U' : 'x';
	s += m_regs.p.v ? 'V' : 'x';
	s += m_regs.p.n ? 'N' : 'x';

	s += fmt::format("\n\nLast absolute address: ${:04X}\n", m_lastAbsAddr);
	s += fmt::format("Last relative address: ${:02X}\n", m_lastRelAddr);
	s += fmt::format("Last fetched byte: {:02X}\n", m_cache);
	s += fmt::format("Last operation: {}\n", Ops[m_lastOp].Mneumonic);
	s += fmt::format("Cycles remaining: {}\n", m_cycles);

	return s;
}

void MOS6502::InitializeState()
{
	ClearCarry();
	ClearDecimal();
	ClearDisableInterrupts();
	ClearOverflow();
	m_regs.p.z = m_regs.p.b = m_regs.p.n = false;
	m_regs.p.u = true;
}

void MOS6502::Interrupt(uint16_t newAbsAddr, uint8_t newCycles)
{
	// write the counter's current value to stack
	StackWriteAddress(m_regs.pc);

	// set and write state register to stack too
	m_regs.p.b = false;
	m_regs.p.i = m_regs.p.u = true;
	StackWrite(StateByte());

	// get a new counter
	m_lastAbsAddr = newAbsAddr;
	m_regs.pc = FetchAddress();

	m_cycles = newCycles;
}

uint8_t MOS6502::Magic() const
{
	return m_regs.a | 255;
}

uint8_t MOS6502::Read(uint16_t addr) const
{
	return dynamic_cast<Bus6502 *>(m_bus)->Read(addr);
}

uint16_t MOS6502::ReadAddress(uint16_t addr) const
{
	return Read(addr) | (Read(addr + 1) << 8);
}

uint8_t MOS6502::ReadFromLastAddress() const
{
	return ReadAddress(m_lastAbsAddr);
}

uint8_t MOS6502::ReadROM()
{
	return Read(m_regs.pc++);
}

uint16_t MOS6502::ReadROMAddress()
{
	return ReadROM() | (ReadROM() << 8);
}

void MOS6502::Reset()
{
	m_cache = m_lastRelAddr = m_regs.a = m_regs.x = m_regs.y = m_regs.pc = 0;
	m_regs.sp = 253;
	InitializeState();

	m_lastAbsAddr = 0xFFFC;
	m_regs.pc = FetchAddress();

	m_lastAbsAddr = 0;
	m_cycles = 8;
}

void MOS6502::SetCarryNegativeZero(uint16_t value)
{
	m_regs.p.c = value > 255;
	SetNegativeZero(value);
}

void MOS6502::SetNegativeZero(uint16_t value)
{
	m_regs.p.z = (value & 255) == 0;
	m_regs.p.n = value & 128;
}

void MOS6502::SetResetVector(uint16_t addr)
{
	WriteAddress(0xFFFC, addr);
}

uint8_t MOS6502::StackRead()
{
	return Read(256 + ++m_regs.sp);
}

uint16_t MOS6502::StackReadAddress()
{
	return StackRead() | (StackRead() << 8);
}

void MOS6502::StackWrite(uint8_t data)
{
	Write(256 + m_regs.sp--, data);
}

void MOS6502::StackWriteAddress(uint16_t addr)
{
	StackWrite((addr & 0xFF00) >> 8);
	StackWrite(addr & 255);
}

uint8_t MOS6502::StateByte() const
{
	return std::bit_cast<uint8_t>(m_regs.p);
}

void MOS6502::Write(uint16_t addr, uint8_t data)
{
	dynamic_cast<Bus6502 *>(m_bus)->Write(addr, data);
}

void MOS6502::WriteAddress(uint16_t addr, uint16_t vector)
{
	Write(addr, vector & 255);
	Write(addr + 1, (vector & 0xFF00) >> 8);
}

uint16_t MOS6502::WriteToFetchedAddress(uint8_t data)
{
	uint16_t addr = FetchAddress();
	Write(addr, data);
	return addr;
}

void MOS6502::WriteToLastAddress(uint8_t data)
{
	Write(m_lastAbsAddr, data);
}


// address modes

uint8_t MOS6502::Absolute()
{
	m_lastAbsAddr = ReadROMAddress();
	return 0;
}

uint8_t MOS6502::AbsoluteX()
{
	uint16_t addr = ReadROMAddress() + m_regs.x;
	return Hi16(m_lastAbsAddr) == Hi16(addr) ? 0 : 1;
}

uint8_t MOS6502::AbsoluteY()
{
	uint16_t addr = ReadROMAddress() + m_regs.y;
	return Hi16(m_lastAbsAddr) == Hi16(addr) ? 0 : 1;
}

uint8_t MOS6502::Immediate()
{
	m_lastAbsAddr = ++m_regs.pc;
	return 0;
}

uint8_t MOS6502::Implied()
{
	m_cache = m_regs.a;
	return 0;
}

uint8_t MOS6502::Indirect()
{
	uint16_t ptr = ReadROMAddress();

	if (ptr & 255)
		// emulate page boundary hardware bug
		m_lastAbsAddr = ReadAddress(ptr);
	else
		// normal behavior
		m_lastAbsAddr = ReadAddress(ptr + 1);

	return 0;
}

uint8_t MOS6502::IndirectX()
{
	m_lastAbsAddr = ReadAddress((ReadROMAddress() + m_regs.x) & 255);
	return 0;
}

uint8_t MOS6502::IndirectY()
{
	uint16_t t = ReadROMAddress();
	uint16_t lo = Read(t & 255);
	uint16_t hi = Read((t + 1) & 255);

	m_lastAbsAddr = (hi << 8) | lo + m_regs.y;

	return Hi16(m_lastAbsAddr) != hi << 8 ? 1 : 0;
}

uint8_t MOS6502::Relative()
{
	m_lastRelAddr = ReadROM();

	// check for signed bit
	if (m_lastRelAddr & 128)
		m_lastRelAddr |= 0xFF00;

	return 0;
}

uint8_t MOS6502::ZeroPage()
{
	m_lastAbsAddr = ReadROM();
	return 0;
}

uint8_t MOS6502::ZeroPageX()
{
	m_lastRelAddr = (ReadROM() + m_regs.x) & 255;
	return 0;
}

uint8_t MOS6502::ZeroPageY()
{
	m_lastRelAddr = (ReadROM() + m_regs.y) & 255;
	return 0;
}


// branching

uint8_t MOS6502::BranchCarryClear()
{
	if (!m_regs.p.c)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchCarrySet()
{
	if (m_regs.p.c)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchEqual()
{
	if (m_regs.p.z)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchNegative()
{
	if (m_regs.p.n)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchNotEqual()
{
	if (!m_regs.p.z)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchOverflowClear()
{
	if (!m_regs.p.v)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchOverflowSet()
{
	if (m_regs.p.v)
		Branch();
	return 0;
}

uint8_t MOS6502::BranchPositive()
{
	if (m_regs.p.n)
		Branch();
	return 0;
}


// state manipulation

uint8_t MOS6502::ClearCarry()
{
	m_regs.p.c = false;
	return 0;
}

uint8_t MOS6502::ClearDecimal()
{
	m_regs.p.d = false;
	return 0;
}

uint8_t MOS6502::ClearDisableInterrupts()
{
	m_regs.p.i = false;
	return 0;
}

uint8_t MOS6502::ClearOverflow()
{
	m_regs.p.v = false;
	return 0;
}

uint8_t MOS6502::SetCarry()
{
	m_regs.p.c = true;
	return 0;
}

uint8_t MOS6502::SetDecimal()
{
	m_regs.p.d = true;
	return 0;
}

uint8_t MOS6502::SetDisableInterrupts()
{
	m_regs.p.i = true;
	return 0;
}


// interrupts

uint8_t MOS6502::Break()
{
	SetDisableInterrupts();
	StackWriteAddress(++m_regs.pc);

	m_regs.p.b = true;
	StackWrite(StateByte());
	m_regs.p.b = false;

	m_regs.pc = ReadAddress(0xFFFE);

	return 0;
}

uint8_t MOS6502::InterruptRequest()
{
	if (!m_regs.p.i)
		Interrupt(0xFFFE, 7);
	return 0;
}

uint8_t MOS6502::InterruptReturn()
{
	uint8_t bits = StackRead();

	// restore state
	m_regs.p = std::bit_cast<MOS6502::Registers::State>(bits);
	m_regs.p.b &= ~m_regs.p.b;
	m_regs.p.u &= ~m_regs.p.u;

	// and the counter
	m_regs.pc = StackReadAddress();

	return 0;
}

uint8_t MOS6502::NonMaskableInterrupt()
{
	Interrupt(0xFFFA, 8);
	return 0;
}

uint8_t MOS6502::SubroutineReturn()
{
	m_regs.pc = StackReadAddress();
	return 0;
}


// stack manipulation

uint8_t MOS6502::PopAccumulator()
{
	m_regs.a = StackRead();
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t MOS6502::PopState()
{
	StackRead();
	m_regs.p.u = true;
	return 0;
}

uint8_t MOS6502::PushAccumulator()
{
	StackWrite(m_regs.a);
	return 0;
}

uint8_t MOS6502::PushState()
{
	m_regs.p.b = true;
	m_regs.p.u = true;
	StackWrite(StateByte());
	m_regs.p.b = false;
	m_regs.p.u = false;

	return 0;
}


// arithmetic

uint8_t MOS6502::Add()
{
	uint16_t tmp = m_regs.a + Fetch() + (m_regs.p.c ? 1 : 0);

	SetCarryNegativeZero(tmp);
	m_regs.p.v = ~((m_regs.a ^ m_cache) & (m_regs.a ^ tmp) & 128);
	m_regs.a = tmp & 255;

	return 1;
}

uint8_t MOS6502::Decrement()
{
	uint8_t tmp = Fetch() - 1;
	WriteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t MOS6502::DecrementX()
{
	SetNegativeZero(--m_regs.x);
	return 0;
}

uint8_t MOS6502::DecrementY()
{
	SetNegativeZero(--m_regs.y);
	return 0;
}

uint8_t MOS6502::Increment()
{
	uint8_t tmp = Fetch() + 1;
	WriteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t MOS6502::IncrementX()
{
	SetNegativeZero(++m_regs.x);
	return 0;
}

uint8_t MOS6502::IncrementY()
{
	SetNegativeZero(++m_regs.y);
	return 0;
}

uint8_t MOS6502::Subtract()
{
	uint16_t value = Fetch() ^ 255; // invert the value
	uint16_t tmp = m_regs.a + value + (m_regs.p.c ? 1 : 0);

	SetCarryNegativeZero(tmp);
	m_regs.p.v = (tmp ^ m_regs.a) & ((tmp ^ value) & 128);
	m_regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t MOS6502::And()
{
	m_regs.a &= Fetch();
	SetNegativeZero(m_regs.a);

	return 1;
}

uint8_t MOS6502::Or()
{
	m_regs.a |= Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6502::RotateLeft()
{
	uint16_t tmp = (Fetch() << 1) | (m_regs.p.c ? 1 : 0);
	SetCarryNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6502::RotateRight()
{
	Fetch();
	uint16_t tmp = (m_cache << 7) | (m_cache >> 1);

	if (m_cache & 1) SetCarry();
	CheckAddressMode(tmp);
	return 0;
}

uint8_t MOS6502::ShiftLeft()
{
	uint16_t tmp = Fetch() << 1;
	SetCarryNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6502::ShiftRight()
{
	if (Fetch() & 1) SetCarry();
	uint16_t tmp = m_cache >> 1;
	SetNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6502::Xor()
{
	m_regs.a ^= Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}


// comparison

uint8_t MOS6502::Compare()
{
	uint16_t tmp = m_regs.a - Fetch();

	if (m_regs.a >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6502::CompareX()
{
	uint16_t tmp = m_regs.x - Fetch();

	if (m_regs.x >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6502::CompareY()
{
	uint16_t tmp = m_regs.y - Fetch();

	if (m_regs.y >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6502::TestBit()
{
	m_regs.p.z = ((m_regs.a & Fetch()) & 255) == 0;
	m_regs.p.n = m_cache & 128;
	m_regs.p.v = m_cache & 64;
	return 0;
}


// jumping

uint8_t MOS6502::Jump()
{
	m_regs.pc = m_lastAbsAddr;
	return 0;
}

uint8_t MOS6502::SubroutineJump()
{
	StackWriteAddress(m_regs.pc);
	return Jump();
}


// loading/storing

uint8_t MOS6502::LoadAccumulator()
{
	m_regs.a = Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6502::LoadX()
{
	m_regs.x = Fetch();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t MOS6502::LoadY()
{
	m_regs.y = Fetch();
	SetNegativeZero(m_regs.y);
	return 1;
}

uint8_t MOS6502::StoreAccumulator()
{
	WriteToLastAddress(m_regs.a);
	return 0;
}

uint8_t MOS6502::StoreX()
{
	WriteToLastAddress(m_regs.x);
	return 0;
}

uint8_t MOS6502::StoreY()
{
	WriteToLastAddress(m_regs.y);
	return 0;
}


// transfers

uint8_t MOS6502::AccumulatorToX()
{
	m_regs.x = m_regs.a;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t MOS6502::AccumulatorToY()
{
	m_regs.y = m_regs.a;
	SetNegativeZero(m_regs.y);
	return 0;
}

uint8_t MOS6502::StackPtrToX()
{
	m_regs.x = m_regs.sp;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t MOS6502::XToAccumulator()
{
	m_regs.a = m_regs.x;
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t MOS6502::XToStackPtr()
{
	m_regs.sp = m_regs.x;
	return 0;
}

uint8_t MOS6502::YToAccumulator()
{
	m_regs.a = m_regs.y;
	SetNegativeZero(m_regs.a);
	return 0;
}


// illegals

uint8_t MOS6502::ALR()
{
	return And() + ShiftRight();
}

uint8_t MOS6502::ANC()
{
	uint8_t res = And();
	SetCarry();
	return res;
}

uint8_t MOS6502::ANE()
{
	m_regs.a = Magic() & m_regs.x & Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6502::ARR()
{
	return And() + Add() + RotateRight();
}

uint8_t MOS6502::DCP()
{
	return Decrement() + Compare();
}

uint8_t MOS6502::ISC()
{
	return Increment() + Subtract();
}

uint8_t MOS6502::JAM()
{
	WriteToLastAddress(255);
	throw std::runtime_error(fmt::format("JAM encountered:\n\n{}\n", FrameInfo()));
	return 0;
}

uint8_t MOS6502::LAS()
{
	return LoadAccumulator() + StackPtrToX();
}

uint8_t MOS6502::LAX()
{
	return LoadAccumulator() + LoadX();
}

uint8_t MOS6502::LXA()
{
	m_regs.x = m_regs.a = Magic() & Fetch();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t MOS6502::NOP()
{
	switch (m_lastOp)
	{
		case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC:
			return 1;
		default:
			return 0;
	}
}

uint8_t MOS6502::RLA()
{
	return RotateLeft() + And();
}

uint8_t MOS6502::RRA()
{
	return RotateRight() + Add();
}

uint8_t MOS6502::SAX()
{
	WriteToLastAddress(m_regs.a & m_regs.x);
	return 0;
}

uint8_t MOS6502::SBX()
{
	uint16_t tmp = m_regs.a - Fetch();

	if (m_regs.a >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	m_regs.x--;

	return 1;
}

uint8_t MOS6502::SHA()
{
	uint16_t addr = FetchAddress();
	Write(addr, m_regs.a & m_regs.x & (Hi16(addr + 1)));
	return 0;
}

uint8_t MOS6502::SHX()
{
	uint16_t addr = FetchAddress();
	Write(addr, m_regs.x & (Hi16(addr + 1)));
	return 0;
}

uint8_t MOS6502::SHY()
{
	uint16_t addr = FetchAddress();
	Write(addr, m_regs.y & (Hi16(addr + 1)));
	return 0;
}

uint8_t MOS6502::SLO()
{
	return ShiftLeft() + Or();
}

uint8_t MOS6502::SRE()
{
	return ShiftRight() + Xor();
}

uint8_t MOS6502::TAS()
{
	m_regs.sp = m_regs.a & m_regs.x;
	return SHA();
}

uint8_t MOS6502::USBC()
{
	return Subtract() + NOP();
}
