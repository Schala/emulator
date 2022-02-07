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
	Opcode6502(7, &MOS6502::Implied, &MOS6502::BRK, "brk"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::ORA, "ora"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::SLO, "slo"sv),
	Opcode6502(2, &MOS6502::ZeroPage, &MOS6502::NOP, "nop"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::ORA, "ora"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ASL, "asl"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::SLO, "slo"sv),
	Opcode6502(3, &MOS6502::Implied, &MOS6502::PHP, "php"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ORA, "ora"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ASL, "asl"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ANC, "anc"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::ORA, "ora"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ASL, "asl"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::SLO, "slo"sv),

	// 1x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BPL, "bpl"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::ORA, "ora"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::SLO, "slo"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::ORA, "ora"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ASL, "asl"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::SLO, "slo"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::CLC, "clc"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::ORA, "ora"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::SLO, "slo"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::ORA, "ora"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ASL, "asl"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::SLO, "slo"sv),

	// 2x
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::JSR, "jsr"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::AND, "and"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::RLA, "rla"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::BIT, "bit"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::AND, "and"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ROL, "rol"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::RLA, "rla"sv),
	Opcode6502(4, &MOS6502::Implied, &MOS6502::PLP, "plp"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::AND, "and"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ROL, "rol"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ANC, "anc"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::BIT, "bit"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::AND, "and"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ROL, "rol"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::RLA, "rla"sv),

	// 3x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BMI, "bmi"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::AND, "and"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::RLA, "rla"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::AND, "and"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ROL, "rol"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::RLA, "rla"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::SEC, "sec"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::AND, "and"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::RLA, "rla"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::AND, "and"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ROL, "rol"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::RLA, "rla"sv),

	// 4x
	Opcode6502(6, &MOS6502::Implied, &MOS6502::RTI, "rti"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::EOR, "eor"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::SRE, "sre"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::NOP, "nop"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::EOR, "eor"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::LSR, "lsr"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::SRE, "sre"sv),
	Opcode6502(3, &MOS6502::Implied, &MOS6502::PHP, "pha"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::EOR, "eor"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::LSR, "lsr"sv),
	Opcode6502(2, &MOS6502::Absolute, &MOS6502::ALR, "alr"sv),
	Opcode6502(3, &MOS6502::Absolute, &MOS6502::JMP, "jmp"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::EOR, "eor"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::LSR, "lsr"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::SRE, "sre"sv),

	// 5x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BVC, "bvc"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::EOR, "eor"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::SRE, "sre"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::EOR, "eor"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::LSR, "lsr"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::SRE, "sre"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::CLI, "cli"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::EOR, "eor"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::SRE, "sre"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::EOR, "eor"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::LSR, "lsr"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::SRE, "sre"sv),

	// 6x
	Opcode6502(6, &MOS6502::Implied, &MOS6502::RTS, "rts"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::ADC, "adc"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::RRA, "rra"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::NOP, "nop"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::ADC, "adc"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ROR, "ror"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::RRA, "rra"sv),
	Opcode6502(4, &MOS6502::Implied, &MOS6502::PLA, "pla"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ADC, "adc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::ROR, "ror"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ARR, "arr"sv),
	Opcode6502(5, &MOS6502::Indirect, &MOS6502::JMP, "jmp"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::ADC, "adc"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ROR, "ror"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::RRA, "rra"sv),

	// 7x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BVS, "bvs"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::ADC, "adc"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::RRA, "rra"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::ADC, "adc"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ROR, "ror"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::RRA, "rra"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::SEI, "sei"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::ADC, "adc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::RRA, "rra"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::ADC, "adc"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ROR, "ror"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::RRA, "rra"sv),

	// 8x
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::STA, "sta"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::SAX, "sax"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::STY, "sty"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::STA, "sta"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::STX, "stx"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::SAX, "sax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::DEY, "dey"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::TXA, "txa"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::ANE, "ane"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::STY, "sty"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::STA, "sta"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::STX, "stx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::SAX, "sax"sv),

	// 9x
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BCC, "bcc"sv),
	Opcode6502(6, &MOS6502::IndirectY, &MOS6502::STA, "sta"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(6, &MOS6502::IndirectY, &MOS6502::SHA, "sha"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::STY, "sty"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::STA, "sta"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::STX, "stx"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::SAX, "sax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::TYA, "tya"sv),
	Opcode6502(5, &MOS6502::AbsoluteY, &MOS6502::STA, "sta"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::TXS, "txs"sv),
	Opcode6502(5, &MOS6502::AbsoluteY, &MOS6502::TAS, "tas"sv),
	Opcode6502(5, &MOS6502::AbsoluteX, &MOS6502::SHY, "shy"sv),
	Opcode6502(5, &MOS6502::AbsoluteX, &MOS6502::STA, "sta"sv),
	Opcode6502(6, &MOS6502::AbsoluteY, &MOS6502::SHX, "shx"sv),
	Opcode6502(5, &MOS6502::AbsoluteY, &MOS6502::SHA, "sha"sv),

	// Ax
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LDY, "ldy"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::LDA, "lda"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LDX, "ldx"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::LAX, "lax"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LDY, "ldy"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LDA, "lda"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LDX, "ldx"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::LAX, "lax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::TAY, "tay"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LDA, "lda"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::TAX, "tax"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::LXA, "lxa"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LDY, "ldy"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LDA, "lda"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LDX, "ldx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::LAX, "lax"sv),

	// Bx
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BCS, "bcs"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::LDA, "lda"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::LAX, "lax"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::LDY, "ldy"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::LDA, "lda"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::LDX, "ldx"sv),
	Opcode6502(4, &MOS6502::ZeroPageY, &MOS6502::LAX, "lax"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::CLV, "clv"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LDA, "lda"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::TSX, "tsx"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LAS, "las"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::LDY, "ldy"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::LDA, "lda"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LDX, "ldx"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::LAX, "lax"sv),

	// Cx
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::CPY, "cpy"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::CMP, "cmp"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::DCP, "dcp"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::CPY, "cpy"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::CMP, "cmp"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::DEC, "dec"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::DCP, "dcp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::INY, "iny"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::CMP, "cmp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::DEX, "dex"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::SBX, "sbx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::CPY, "cpy"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::CMP, "cmp"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::DEC, "dec"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::DCP, "dcp"sv),

	// Dx
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BNE, "bne"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::CMP, "cmp"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(8, &MOS6502::IndirectY, &MOS6502::DCP, "dcp"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::CMP, "cmp"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::DEC, "dec"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::DCP, "dcp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::CLD, "cld"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::CMP, "cmp"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::DCP, "dcp"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::CMP, "cmp"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::DEC, "dec"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::DCP, "dcp"sv),

	// Ex
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::CPX, "cpx"sv),
	Opcode6502(6, &MOS6502::IndirectX, &MOS6502::SBC, "sbc"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::NOP, "nop"sv),
	Opcode6502(8, &MOS6502::IndirectX, &MOS6502::ISC, "isc"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::CPX, "cpx"sv),
	Opcode6502(3, &MOS6502::ZeroPage, &MOS6502::SBC, "sbc"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::INC, "inc"sv),
	Opcode6502(5, &MOS6502::ZeroPage, &MOS6502::ISC, "isc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::INX, "inx"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::SBC, "sbc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(2, &MOS6502::Immediate, &MOS6502::USBC, "usbc"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::CPX, "cpx"sv),
	Opcode6502(4, &MOS6502::Absolute, &MOS6502::SBC, "sbc"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::INC, "inc"sv),
	Opcode6502(6, &MOS6502::Absolute, &MOS6502::ISC, "isc"sv),

	// Fx
	Opcode6502(2, &MOS6502::Relative, &MOS6502::BEQ, "beq"sv),
	Opcode6502(5, &MOS6502::IndirectY, &MOS6502::SBC, "sbc"sv),
	Opcode6502(0, nullptr, &MOS6502::JAM, "jam"sv),
	Opcode6502(4, &MOS6502::IndirectY, &MOS6502::ISC, "isc"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::ZeroPageX, &MOS6502::SBC, "sbc"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::INC, "inc"sv),
	Opcode6502(6, &MOS6502::ZeroPageX, &MOS6502::ISC, "isc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::SED, "sed"sv),
	Opcode6502(4, &MOS6502::AbsoluteY, &MOS6502::SBC, "sbc"sv),
	Opcode6502(2, &MOS6502::Implied, &MOS6502::NOP, "nop"sv),
	Opcode6502(7, &MOS6502::AbsoluteY, &MOS6502::ISC, "isc"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::NOP, "nop"sv),
	Opcode6502(4, &MOS6502::AbsoluteX, &MOS6502::SBC, "sbc"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::INC, "inc"sv),
	Opcode6502(7, &MOS6502::AbsoluteX, &MOS6502::ISC, "isc"sv)
};

MOS6502::MOS6502(MOS6502::BusType *bus, uint16_t startAddr, uint16_t endAddr):
	Device(bus, startAddr, endAddr),
	m_cache(0),
	m_cycles(0),
	m_lastOp(0),
	m_lastAbsAddr(0),
	m_lastRelAddr(0)
{
	m_regs.a = m_regs.x = m_regs.y = m_regs.pc = 0;
	m_regs.s = 253;
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
	s += fmt::format("SP: ${:02X}\tPC: ${:04X}\nState: ", m_regs.s, m_regs.pc);

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
	s += fmt::format("Last operation: {}\n", Ops[m_lastOp].Mnemonic);
	s += fmt::format("Cycles remaining: {}\n", m_cycles);

	return s;
}

void MOS6502::InitializeState()
{
	CLC();
	CLD();
	CLI();
	CLV();
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
	m_regs.s = 253;
	InitializeState();

	m_lastAbsAddr = 0xFFFC;
	m_regs.pc = FetchAddress();

	m_lastAbsAddr = 0;
	m_cycles = 8;
}

void MOS6502::SECNegativeZero(uint16_t value)
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
	return Read(256 + ++m_regs.s);
}

uint16_t MOS6502::StackReadAddress()
{
	return StackRead() | (StackRead() << 8);
}

void MOS6502::StackWrite(uint8_t data)
{
	Write(256 + m_regs.s--, data);
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
	m_bus->Write(addr, data);
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

uint8_t MOS6502::BCC()
{
	if (!m_regs.p.c)
		Branch();
	return 0;
}

uint8_t MOS6502::BCS()
{
	if (m_regs.p.c)
		Branch();
	return 0;
}

uint8_t MOS6502::BEQ()
{
	if (m_regs.p.z)
		Branch();
	return 0;
}

uint8_t MOS6502::BMI()
{
	if (m_regs.p.n)
		Branch();
	return 0;
}

uint8_t MOS6502::BNE()
{
	if (!m_regs.p.z)
		Branch();
	return 0;
}

uint8_t MOS6502::BVC()
{
	if (!m_regs.p.v)
		Branch();
	return 0;
}

uint8_t MOS6502::BVS()
{
	if (m_regs.p.v)
		Branch();
	return 0;
}

uint8_t MOS6502::BPL()
{
	if (m_regs.p.n)
		Branch();
	return 0;
}


// state manipulation

uint8_t MOS6502::CLC()
{
	m_regs.p.c = false;
	return 0;
}

uint8_t MOS6502::CLD()
{
	m_regs.p.d = false;
	return 0;
}

uint8_t MOS6502::CLI()
{
	m_regs.p.i = false;
	return 0;
}

uint8_t MOS6502::CLV()
{
	m_regs.p.v = false;
	return 0;
}

uint8_t MOS6502::SEC()
{
	m_regs.p.c = true;
	return 0;
}

uint8_t MOS6502::SED()
{
	m_regs.p.d = true;
	return 0;
}

uint8_t MOS6502::SEI()
{
	m_regs.p.i = true;
	return 0;
}


// interrupts

uint8_t MOS6502::BRK()
{
	SEI();
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

uint8_t MOS6502::RTI()
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

uint8_t MOS6502::RTS()
{
	m_regs.pc = StackReadAddress();
	return 0;
}


// stack manipulation

uint8_t MOS6502::PLA()
{
	m_regs.a = StackRead();
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t MOS6502::PLP()
{
	StackRead();
	m_regs.p.u = true;
	return 0;
}

uint8_t MOS6502::PHP()
{
	StackWrite(m_regs.a);
	return 0;
}

uint8_t MOS6502::PHP()
{
	m_regs.p.b = true;
	m_regs.p.u = true;
	StackWrite(StateByte());
	m_regs.p.b = false;
	m_regs.p.u = false;

	return 0;
}


// arithmetic

uint8_t MOS6502::ADC()
{
	uint16_t tmp = m_regs.a + Fetch() + (m_regs.p.c ? 1 : 0);

	SECNegativeZero(tmp);
	m_regs.p.v = ~((m_regs.a ^ m_cache) & (m_regs.a ^ tmp) & 128);
	m_regs.a = tmp & 255;

	return 1;
}

uint8_t MOS6502::DEC()
{
	uint8_t tmp = Fetch() - 1;
	WriteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t MOS6502::DEX()
{
	SetNegativeZero(--m_regs.x);
	return 0;
}

uint8_t MOS6502::DEY()
{
	SetNegativeZero(--m_regs.y);
	return 0;
}

uint8_t MOS6502::INC()
{
	uint8_t tmp = Fetch() + 1;
	WriteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t MOS6502::INX()
{
	SetNegativeZero(++m_regs.x);
	return 0;
}

uint8_t MOS6502::INY()
{
	SetNegativeZero(++m_regs.y);
	return 0;
}

uint8_t MOS6502::SBC()
{
	uint16_t value = Fetch() ^ 255; // invert the value
	uint16_t tmp = m_regs.a + value + (m_regs.p.c ? 1 : 0);

	SECNegativeZero(tmp);
	m_regs.p.v = (tmp ^ m_regs.a) & ((tmp ^ value) & 128);
	m_regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t MOS6502::AND()
{
	m_regs.a &= Fetch();
	SetNegativeZero(m_regs.a);

	return 1;
}

uint8_t MOS6502::ORA()
{
	m_regs.a |= Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6502::ROL()
{
	uint16_t tmp = (Fetch() << 1) | (m_regs.p.c ? 1 : 0);
	SECNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6502::ROR()
{
	Fetch();
	uint16_t tmp = (m_cache << 7) | (m_cache >> 1);

	if (m_cache & 1) SEC();
	CheckAddressMode(tmp);
	return 0;
}

uint8_t MOS6502::ASL()
{
	uint16_t tmp = Fetch() << 1;
	SECNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6502::LSR()
{
	if (Fetch() & 1) SEC();
	uint16_t tmp = m_cache >> 1;
	SetNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6502::EOR()
{
	m_regs.a ^= Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}


// comparison

uint8_t MOS6502::CMP()
{
	uint16_t tmp = m_regs.a - Fetch();

	if (m_regs.a >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6502::CPX()
{
	uint16_t tmp = m_regs.x - Fetch();

	if (m_regs.x >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6502::CPY()
{
	uint16_t tmp = m_regs.y - Fetch();

	if (m_regs.y >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6502::BIT()
{
	m_regs.p.z = ((m_regs.a & Fetch()) & 255) == 0;
	m_regs.p.n = m_cache & 128;
	m_regs.p.v = m_cache & 64;
	return 0;
}


// jumping

uint8_t MOS6502::JMP()
{
	m_regs.pc = m_lastAbsAddr;
	return 0;
}

uint8_t MOS6502::JSR()
{
	StackWriteAddress(m_regs.pc);
	return Jump();
}


// loading/storing

uint8_t MOS6502::LDA()
{
	m_regs.a = Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6502::LDX()
{
	m_regs.x = Fetch();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t MOS6502::LDY()
{
	m_regs.y = Fetch();
	SetNegativeZero(m_regs.y);
	return 1;
}

uint8_t MOS6502::STA()
{
	WriteToLastAddress(m_regs.a);
	return 0;
}

uint8_t MOS6502::STX()
{
	WriteToLastAddress(m_regs.x);
	return 0;
}

uint8_t MOS6502::STY()
{
	WriteToLastAddress(m_regs.y);
	return 0;
}


// transfers

uint8_t MOS6502::TAX()
{
	m_regs.x = m_regs.a;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t MOS6502::TAY()
{
	m_regs.y = m_regs.a;
	SetNegativeZero(m_regs.y);
	return 0;
}

uint8_t MOS6502::TSX()
{
	m_regs.x = m_regs.s;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t MOS6502::TXA()
{
	m_regs.a = m_regs.x;
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t MOS6502::TXS()
{
	m_regs.s = m_regs.x;
	return 0;
}

uint8_t MOS6502::TYA()
{
	m_regs.a = m_regs.y;
	SetNegativeZero(m_regs.a);
	return 0;
}


// illegals

uint8_t MOS6502::ALR()
{
	return AND() + LSR();
}

uint8_t MOS6502::ANC()
{
	uint8_t res = AND();
	SEC();
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
	return AND() + Add() + ROR();
}

uint8_t MOS6502::DCP()
{
	return DEC() + CMP();
}

uint8_t MOS6502::ISC()
{
	return INC() + SBC();
}

uint8_t MOS6502::JAM()
{
	WriteToLastAddress(255);
	throw std::runtime_error(fmt::format("JAM encountered:\n\n{}\n", FrameInfo()));
	return 0;
}

uint8_t MOS6502::LAS()
{
	return LDA() + TSX();
}

uint8_t MOS6502::LAX()
{
	return LDA() + LDX();
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
	return ROL() + AND();
}

uint8_t MOS6502::RRA()
{
	return ROR() + Add();
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
		SEC();

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
	return ASL() + ORA();
}

uint8_t MOS6502::SRE()
{
	return LSR() + EOR();
}

uint8_t MOS6502::TAS()
{
	m_regs.s = m_regs.a & m_regs.x;
	return SHA();
}

uint8_t MOS6502::USBC()
{
	return SBC() + NOP();
}
