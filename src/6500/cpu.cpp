#include <algorithm>
#include <bit>
#include <fmt/core.h> // todo: <format>
#include <stdexcept>
#include <utility>

#include "../core/utility.h"
#include "cpu.h"

using namespace std::literals::string_view_literals;

// Opcode6500

Opcode6500::Opcode6500(uint8_t cycles, Instruction6500 addrMode, Instruction6500 op,
	std::string_view mnemonic):
	Cycles(cycles),
	AddressMode(addrMode),
	Operation(op),
	Mnemonic(std::move(mnemonic))
{
}


// MOS6500

const std::array<Opcode6500, 256> MOS6500::Ops =
{
	// 0x
	Opcode6500(7, &MOS6500::Implied, &MOS6500::BRK, "brk"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::ORA, "ora"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectX, &MOS6500::SLO, "slo"sv),
	Opcode6500(2, &MOS6500::ZeroPage, &MOS6500::NOP, "nop"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::ORA, "ora"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::ASL, "asl"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::SLO, "slo"sv),
	Opcode6500(3, &MOS6500::Implied, &MOS6500::PHP, "php"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::ORA, "ora"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::ASL, "asl"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::ANC, "anc"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::ORA, "ora"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::ASL, "asl"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::SLO, "slo"sv),

	// 1x
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BPL, "bpl"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::ORA, "ora"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectY, &MOS6500::SLO, "slo"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::ORA, "ora"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::ASL, "asl"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::SLO, "slo"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::CLC, "clc"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::ORA, "ora"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(7, &MOS6500::AbsoluteY, &MOS6500::SLO, "slo"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::ORA, "ora"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::ASL, "asl"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::SLO, "slo"sv),

	// 2x
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::JSR, "jsr"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::AND, "and"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectX, &MOS6500::RLA, "rla"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::BIT, "bit"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::AND, "and"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::ROL, "rol"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::RLA, "rla"sv),
	Opcode6500(4, &MOS6500::Implied, &MOS6500::PLP, "plp"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::AND, "and"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::ROL, "rol"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::ANC, "anc"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::BIT, "bit"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::AND, "and"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::ROL, "rol"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::RLA, "rla"sv),

	// 3x
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BMI, "bmi"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::AND, "and"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectY, &MOS6500::RLA, "rla"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::AND, "and"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::ROL, "rol"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::RLA, "rla"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::SEC, "sec"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::AND, "and"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(7, &MOS6500::AbsoluteY, &MOS6500::RLA, "rla"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::AND, "and"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::ROL, "rol"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::RLA, "rla"sv),

	// 4x
	Opcode6500(6, &MOS6500::Implied, &MOS6500::RTI, "rti"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::EOR, "eor"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectX, &MOS6500::SRE, "sre"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::NOP, "nop"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::EOR, "eor"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::LSR, "lsr"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::SRE, "sre"sv),
	Opcode6500(3, &MOS6500::Implied, &MOS6500::PHP, "pha"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::EOR, "eor"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::LSR, "lsr"sv),
	Opcode6500(2, &MOS6500::Absolute, &MOS6500::ALR, "alr"sv),
	Opcode6500(3, &MOS6500::Absolute, &MOS6500::JMP, "jmp"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::EOR, "eor"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::LSR, "lsr"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::SRE, "sre"sv),

	// 5x
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BVC, "bvc"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::EOR, "eor"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectY, &MOS6500::SRE, "sre"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::EOR, "eor"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::LSR, "lsr"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::SRE, "sre"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::CLI, "cli"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::EOR, "eor"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(7, &MOS6500::AbsoluteY, &MOS6500::SRE, "sre"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::EOR, "eor"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::LSR, "lsr"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::SRE, "sre"sv),

	// 6x
	Opcode6500(6, &MOS6500::Implied, &MOS6500::RTS, "rts"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::ADC, "adc"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectX, &MOS6500::RRA, "rra"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::NOP, "nop"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::ADC, "adc"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::ROR, "ror"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::RRA, "rra"sv),
	Opcode6500(4, &MOS6500::Implied, &MOS6500::PLA, "pla"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::ADC, "adc"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::ROR, "ror"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::ARR, "arr"sv),
	Opcode6500(5, &MOS6500::Indirect, &MOS6500::JMP, "jmp"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::ADC, "adc"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::ROR, "ror"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::RRA, "rra"sv),

	// 7x
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BVS, "bvs"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::ADC, "adc"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectY, &MOS6500::RRA, "rra"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::ADC, "adc"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::ROR, "ror"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::RRA, "rra"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::SEI, "sei"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::ADC, "adc"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(7, &MOS6500::AbsoluteY, &MOS6500::RRA, "rra"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::ADC, "adc"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::ROR, "ror"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::RRA, "rra"sv),

	// 8x
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::NOP, "nop"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::STA, "sta"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::NOP, "nop"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::SAX, "sax"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::STY, "sty"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::STA, "sta"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::STX, "stx"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::SAX, "sax"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::DEY, "dey"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::NOP, "nop"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::TXA, "txa"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::ANE, "ane"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::STY, "sty"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::STA, "sta"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::STX, "stx"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::SAX, "sax"sv),

	// 9x
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BCC, "bcc"sv),
	Opcode6500(6, &MOS6500::IndirectY, &MOS6500::STA, "sta"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(6, &MOS6500::IndirectY, &MOS6500::SHA, "sha"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::STY, "sty"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::STA, "sta"sv),
	Opcode6500(4, &MOS6500::ZeroPageY, &MOS6500::STX, "stx"sv),
	Opcode6500(4, &MOS6500::ZeroPageY, &MOS6500::SAX, "sax"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::TYA, "tya"sv),
	Opcode6500(5, &MOS6500::AbsoluteY, &MOS6500::STA, "sta"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::TXS, "txs"sv),
	Opcode6500(5, &MOS6500::AbsoluteY, &MOS6500::TAS, "tas"sv),
	Opcode6500(5, &MOS6500::AbsoluteX, &MOS6500::SHY, "shy"sv),
	Opcode6500(5, &MOS6500::AbsoluteX, &MOS6500::STA, "sta"sv),
	Opcode6500(6, &MOS6500::AbsoluteY, &MOS6500::SHX, "shx"sv),
	Opcode6500(5, &MOS6500::AbsoluteY, &MOS6500::SHA, "sha"sv),

	// Ax
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::LDY, "ldy"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::LDA, "lda"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::LDX, "ldx"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::LAX, "lax"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::LDY, "ldy"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::LDA, "lda"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::LDX, "ldx"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::LAX, "lax"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::TAY, "tay"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::LDA, "lda"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::TAX, "tax"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::LXA, "lxa"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::LDY, "ldy"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::LDA, "lda"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::LDX, "ldx"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::LAX, "lax"sv),

	// Bx
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BCS, "bcs"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::LDA, "lda"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::LAX, "lax"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::LDY, "ldy"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::LDA, "lda"sv),
	Opcode6500(4, &MOS6500::ZeroPageY, &MOS6500::LDX, "ldx"sv),
	Opcode6500(4, &MOS6500::ZeroPageY, &MOS6500::LAX, "lax"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::CLV, "clv"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::LDA, "lda"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::TSX, "tsx"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::LAS, "las"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::LDY, "ldy"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::LDA, "lda"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::LDX, "ldx"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::LAX, "lax"sv),

	// Cx
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::CPY, "cpy"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::CMP, "cmp"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::NOP, "nop"sv),
	Opcode6500(8, &MOS6500::IndirectX, &MOS6500::DCP, "dcp"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::CPY, "cpy"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::CMP, "cmp"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::DEC, "dec"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::DCP, "dcp"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::INY, "iny"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::CMP, "cmp"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::DEX, "dex"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::SBX, "sbx"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::CPY, "cpy"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::CMP, "cmp"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::DEC, "dec"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::DCP, "dcp"sv),

	// Dx
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BNE, "bne"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::CMP, "cmp"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(8, &MOS6500::IndirectY, &MOS6500::DCP, "dcp"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::CMP, "cmp"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::DEC, "dec"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::DCP, "dcp"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::CLD, "cld"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::CMP, "cmp"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(7, &MOS6500::AbsoluteY, &MOS6500::DCP, "dcp"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::CMP, "cmp"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::DEC, "dec"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::DCP, "dcp"sv),

	// Ex
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::CPX, "cpx"sv),
	Opcode6500(6, &MOS6500::IndirectX, &MOS6500::SBC, "sbc"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::NOP, "nop"sv),
	Opcode6500(8, &MOS6500::IndirectX, &MOS6500::ISC, "isc"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::CPX, "cpx"sv),
	Opcode6500(3, &MOS6500::ZeroPage, &MOS6500::SBC, "sbc"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::INC, "inc"sv),
	Opcode6500(5, &MOS6500::ZeroPage, &MOS6500::ISC, "isc"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::INX, "inx"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::SBC, "sbc"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(2, &MOS6500::Immediate, &MOS6500::USBC, "usbc"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::CPX, "cpx"sv),
	Opcode6500(4, &MOS6500::Absolute, &MOS6500::SBC, "sbc"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::INC, "inc"sv),
	Opcode6500(6, &MOS6500::Absolute, &MOS6500::ISC, "isc"sv),

	// Fx
	Opcode6500(2, &MOS6500::Relative, &MOS6500::BEQ, "beq"sv),
	Opcode6500(5, &MOS6500::IndirectY, &MOS6500::SBC, "sbc"sv),
	Opcode6500(0, nullptr, &MOS6500::JAM, "jam"sv),
	Opcode6500(4, &MOS6500::IndirectY, &MOS6500::ISC, "isc"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::ZeroPageX, &MOS6500::SBC, "sbc"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::INC, "inc"sv),
	Opcode6500(6, &MOS6500::ZeroPageX, &MOS6500::ISC, "isc"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::SED, "sed"sv),
	Opcode6500(4, &MOS6500::AbsoluteY, &MOS6500::SBC, "sbc"sv),
	Opcode6500(2, &MOS6500::Implied, &MOS6500::NOP, "nop"sv),
	Opcode6500(7, &MOS6500::AbsoluteY, &MOS6500::ISC, "isc"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::NOP, "nop"sv),
	Opcode6500(4, &MOS6500::AbsoluteX, &MOS6500::SBC, "sbc"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::INC, "inc"sv),
	Opcode6500(7, &MOS6500::AbsoluteX, &MOS6500::ISC, "isc"sv)
};

MOS6500::MOS6500(BusLE16 *bus, uint16_t startAddr, uint16_t endAddr):
	CPU(bus, startAddr, endAddr, 0xFFFC, 256, 253),
	m_lastOp(0)
{
}

void MOS6500::Branch()
{
	m_cycles++;
	lastAbsAddress = counter + static_cast<int8_t>(lastRelAddress);

	// need an additional cycle if different page
	if (Hi16(lastAbsAddress) != Hi16(counter))
		m_cycles++;

	// jump to address
	counter = lastAbsAddress;
}

void MOS6500::CheckAddressMode(uint16_t value)
{
	if (Ops[m_lastOp].AddressMode == &MOS6500::Implied)
		m_regs.a = value & 255;
	else
		WriteByteToLastAddress(value & 255);
}

void MOS6500::Clock()
{
	if (m_cycles == 0)
	{
		m_lastDisasm = Disassemble(counter);

		// get and increment the counter
		m_lastOp = ReadROMByte();

		// set cycles, see if any additional cycles are needed
		m_cycles = Ops[m_lastOp].Cycles;
		m_cycles += (this->*Ops[m_lastOp].AddressMode)();
		m_cycles += (this->*Ops[m_lastOp].Operation)();
	}

	m_cycles--;
}

size_t MOS6500::Cycles() const
{
	return m_cycles;
}

Disassembly MOS6500::Disassemble(size_t addr)
{
	size_t address = addr;
	const Opcode6500 &op = Ops[ReadByte(addr++)];
	std::string line = op.Mnemonic.data();

	if (op.AddressMode == &MOS6500::Immediate)
		line += fmt::format(" #${:02X}", ReadByte(addr++));
	else if (op.AddressMode == &MOS6500::ZeroPage)
		line += fmt::format(" ${:02X}", ReadByte(addr++));
	else if (op.AddressMode == &MOS6500::ZeroPageX)
		line += fmt::format(" ${:02X}, x", ReadByte(addr++));
	else if (op.AddressMode == &MOS6500::ZeroPageY)
		line += fmt::format(" ${:02X}, y", ReadByte(addr++));
	else if (op.AddressMode == &MOS6500::IndirectX)
		line += fmt::format(" (${:02X}, x)", ReadByte(addr++));
	else if (op.AddressMode == &MOS6500::IndirectY)
		line += fmt::format(" (${:02X}, y)", ReadByte(addr++));
	else if (op.AddressMode == &MOS6500::Absolute)
	{
		line += fmt::format(" ${:04X}", ReadByte(addr));
		addr += 2;
	}
	else if (op.AddressMode == &MOS6500::AbsoluteX)
	{
		line += fmt::format(" ${:04X}, x", ReadByte(addr));
		addr += 2;
	}
	else if (op.AddressMode == &MOS6500::AbsoluteY)
	{
		line += fmt::format(" ${:04X}, y", ReadByte(addr));
		addr += 2;
	}
	else if (op.AddressMode == &MOS6500::Indirect)
	{
		line += fmt::format(" (${:04X})", ReadByte(addr));
		addr += 2;
	}
	else if (op.AddressMode == &MOS6500::Relative)
	{
		uint8_t value = ReadByte(addr++);
		line += fmt::format(" ${:02X} [${:04X}]", value, addr + static_cast<int8_t>(value));
	}
	else ; // 'implied' takes no operands

	return std::make_pair(address, std::move(line));
}

/*void MOS6500::Disassemble(size_t startAddr, size_t endAddr)
{
	disassembly.clear(); // dump any old disassembly

	uint16_t addr = startAddr; // for readability

	while (addr <= endAddr)
	{
		size_t address = addr;

		const Opcode6500 &op = Ops[ReadByte(addr++)];

		std::string line = op.Mnemonic.data();

		if (op.AddressMode == &MOS6500::Immediate)
			line += fmt::format(" #${:02X}", ReadByte(addr++));
		else if (op.AddressMode == &MOS6500::ZeroPage)
			line += fmt::format(" ${:02X}", ReadByte(addr++));
		else if (op.AddressMode == &MOS6500::ZeroPageX)
			line += fmt::format(" ${:02X}, x", ReadByte(addr++));
		else if (op.AddressMode == &MOS6500::ZeroPageY)
			line += fmt::format(" ${:02X}, y", ReadByte(addr++));
		else if (op.AddressMode == &MOS6500::IndirectX)
			line += fmt::format(" (${:02X}, x)", ReadByte(addr++));
		else if (op.AddressMode == &MOS6500::IndirectY)
			line += fmt::format(" (${:02X}, y)", ReadByte(addr++));
		else if (op.AddressMode == &MOS6500::Absolute)
		{
			line += fmt::format(" ${:04X}", ReadByte(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6500::AbsoluteX)
		{
			line += fmt::format(" ${:04X}, x", ReadByte(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6500::AbsoluteY)
		{
			line += fmt::format(" ${:04X}, y", ReadByte(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6500::Indirect)
		{
			line += fmt::format(" (${:04X})", ReadByte(addr));
			addr += 2;
		}
		else if (op.AddressMode == &MOS6500::Relative)
		{
			uint8_t value = ReadByte(addr++);
			line += fmt::format(" ${:02X} [${:04X}]", value, addr + static_cast<int8_t>(value));
		}
		else continue; // 'implied' takes no operands

		disassembly.push_back(std::make_pair(address, std::move(line)));
	}
}*/

uint8_t MOS6500::FetchByte()
{
	if (Ops[m_lastOp].AddressMode != &MOS6500::Implied)
		m_cache = ReadByteFromLastAddress();
	return m_cache;
}

size_t MOS6500::FetchAddress()
{
	return FetchByte() | (FetchByte() << 8);
}

std::string MOS6500::FrameInfo()
{
	auto frame = StackFrame();

	std::string s = fmt::format("Offset {:04X}: {}\n", m_lastDisasm.first, m_lastDisasm.second);

	s += "Stack frame:\n";
	for (uint8_t b : frame)
		s += fmt::format("{:02X} ", b);

	s += fmt::format("\n\nA: $#{:02X}\tX: $#{:02X}\tY: $#{:02X}\n", m_regs.a, m_regs.x, m_regs.y);
	s += fmt::format("S: ${:02X}\tPC: ${:04X}\nP: ", stackPtr, counter);

	s += m_regs.p.c ? 'C' : 'x';
	s += m_regs.p.z ? 'Z' : 'x';
	s += m_regs.p.i ? 'I' : 'x';
	s += m_regs.p.d ? 'D' : 'x';
	s += m_regs.p.b ? 'B' : 'x';
	s += m_regs.p.u ? 'U' : 'x';
	s += m_regs.p.v ? 'V' : 'x';
	s += m_regs.p.n ? 'N' : 'x';

	s += fmt::format("\n\nLast absolute address: ${:04X}\n", lastAbsAddress);
	s += fmt::format("Last relative address: ${:02X}\n", lastRelAddress);
	s += fmt::format("Last fetched byte: {:02X}\n", m_cache);
	s += fmt::format("Last operation: {} ({:02X})\n", Ops[m_lastOp].Mnemonic, m_lastOp);
	//s += fmt::format("Cycles remaining: {}\n", m_cycles);
	s += "--------------------------------\n";

	return s;
}

void MOS6500::InitializeState()
{
	CLC();
	CLD();
	CLI();
	CLV();
	m_regs.p.z = m_regs.p.b = m_regs.p.n = false;
	m_regs.p.u = true;
}

void MOS6500::Interrupt(uint16_t newAbsAddr, uint8_t newCycles)
{
	// write the counter's current value to stack
	StackWriteAddress(counter);

	// set and write state register to stack too
	m_regs.p.b = false;
	m_regs.p.i = m_regs.p.u = true;
	StackWriteByte(StateByte());

	// get a new counter
	lastAbsAddress = newAbsAddr;
	counter = FetchAddress();

	m_cycles = newCycles;
}

Disassembly & MOS6500::LastDisassembly()
{
	return m_lastDisasm;
}

uint8_t MOS6500::Magic() const
{
	return m_regs.a | 255;
}

size_t MOS6500::ReadROMAddress()
{
	return ReadROMByte() | (ReadROMByte() << 8);
}

void MOS6500::Reset()
{
	m_cache = lastRelAddress = m_regs.a = m_regs.x = m_regs.y = 0;
	stackPtr = stackInit;
	InitializeState();

	lastAbsAddress = resetVector;
	counter = ReadAddressFromLastAddress();

	lastAbsAddress = 0;
	m_cycles = 8;
}

void MOS6500::SetCarryNegativeZero(uint16_t value)
{
	m_regs.p.c = value > 255;
	SetNegativeZero(value);
}

void MOS6500::SetNegativeZero(uint16_t value)
{
	m_regs.p.z = (value & 255) == 0;
	m_regs.p.n = value & 128;
}

std::array<uint8_t, 256> MOS6500::StackFrame()
{
	std::array<uint8_t, 256> frame;

	for (uint16_t i = 0; i < 256; i++)
		frame[i] = ReadByte(i);

	return std::move(frame);
}

size_t MOS6500::StackReadAddress()
{
	return StackReadByte() | (StackReadByte() << 8);
}

void MOS6500::StackWriteAddress(size_t addr)
{
	StackWriteByte((addr & 0xFF00) >> 8);
	StackWriteByte(addr & 255);
}

uint8_t MOS6500::StateByte() const
{
	return std::bit_cast<uint8_t>(m_regs.p);
}


// address modes

uint8_t MOS6500::Absolute()
{
	lastAbsAddress = ReadROMAddress();
	return 0;
}

uint8_t MOS6500::AbsoluteX()
{
	uint16_t addr = ReadROMAddress() + m_regs.x;
	return Hi16(lastAbsAddress) == Hi16(addr) ? 0 : 1;
}

uint8_t MOS6500::AbsoluteY()
{
	uint16_t addr = ReadROMAddress() + m_regs.y;
	return Hi16(lastAbsAddress) == Hi16(addr) ? 0 : 1;
}

uint8_t MOS6500::Immediate()
{
	lastAbsAddress = ++counter;
	return 0;
}

uint8_t MOS6500::Implied()
{
	m_cache = m_regs.a;
	return 0;
}

uint8_t MOS6500::Indirect()
{
	uint16_t ptr = ReadROMAddress();

	if (ptr & 255)
		// emulate page boundary hardware bug
		lastAbsAddress = ReadAddress(ptr);
	else
		// normal behavior
		lastAbsAddress = ReadAddress(ptr + 1);

	return 0;
}

uint8_t MOS6500::IndirectX()
{
	lastAbsAddress = ReadAddress((ReadROMAddress() + m_regs.x) & 255);
	return 0;
}

uint8_t MOS6500::IndirectY()
{
	uint16_t t = ReadROMAddress();
	uint16_t lo = ReadByte(t & 255);
	uint16_t hi = ReadByte((t + 1) & 255);

	lastAbsAddress = (hi << 8) | lo + m_regs.y;

	return Hi16(lastAbsAddress) != hi << 8 ? 1 : 0;
}

uint8_t MOS6500::Relative()
{
	lastRelAddress = ReadROMByte();

	// check for signed bit
	if (lastRelAddress & 128)
		lastRelAddress |= 0xFF00;

	return 0;
}

uint8_t MOS6500::ZeroPage()
{
	lastAbsAddress = ReadROMByte();
	return 0;
}

uint8_t MOS6500::ZeroPageX()
{
	lastRelAddress = (ReadROMByte() + m_regs.x) & 255;
	return 0;
}

uint8_t MOS6500::ZeroPageY()
{
	lastRelAddress = (ReadROMByte() + m_regs.y) & 255;
	return 0;
}


// branching

uint8_t MOS6500::BCC()
{
	if (!m_regs.p.c)
		Branch();
	return 0;
}

uint8_t MOS6500::BCS()
{
	if (m_regs.p.c)
		Branch();
	return 0;
}

uint8_t MOS6500::BEQ()
{
	if (m_regs.p.z)
		Branch();
	return 0;
}

uint8_t MOS6500::BMI()
{
	if (m_regs.p.n)
		Branch();
	return 0;
}

uint8_t MOS6500::BNE()
{
	if (!m_regs.p.z)
		Branch();
	return 0;
}

uint8_t MOS6500::BVC()
{
	if (!m_regs.p.v)
		Branch();
	return 0;
}

uint8_t MOS6500::BVS()
{
	if (m_regs.p.v)
		Branch();
	return 0;
}

uint8_t MOS6500::BPL()
{
	if (m_regs.p.n)
		Branch();
	return 0;
}


// state manipulation

uint8_t MOS6500::CLC()
{
	m_regs.p.c = false;
	return 0;
}

uint8_t MOS6500::CLD()
{
	m_regs.p.d = false;
	return 0;
}

uint8_t MOS6500::CLI()
{
	m_regs.p.i = false;
	return 0;
}

uint8_t MOS6500::CLV()
{
	m_regs.p.v = false;
	return 0;
}

uint8_t MOS6500::SEC()
{
	m_regs.p.c = true;
	return 0;
}

uint8_t MOS6500::SED()
{
	m_regs.p.d = true;
	return 0;
}

uint8_t MOS6500::SEI()
{
	m_regs.p.i = true;
	return 0;
}


// interrupts

uint8_t MOS6500::BRK()
{
	SEI();
	StackWriteAddress(++counter);

	m_regs.p.b = true;
	StackWriteByte(StateByte());
	m_regs.p.b = false;

	counter = ReadAddress(0xFFFE);

	return 0;
}

uint8_t MOS6500::IRQ()
{
	if (!m_regs.p.i)
		Interrupt(0xFFFE, 7);
	return 0;
}

uint8_t MOS6500::NMI()
{
	Interrupt(0xFFFA, 8);
	return 0;
}

uint8_t MOS6500::RTI()
{
	uint8_t bits = StackReadByte();

	// restore state
	m_regs.p = std::bit_cast<MOS6500::Registers::State>(bits);
	m_regs.p.b &= ~m_regs.p.b;
	m_regs.p.u &= ~m_regs.p.u;

	// and the counter
	counter = StackReadAddress();

	return 0;
}

uint8_t MOS6500::RTS()
{
	counter = StackReadAddress();
	return 0;
}


// stack manipulation

uint8_t MOS6500::PLA()
{
	m_regs.a = StackReadByte();
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t MOS6500::PLP()
{
	StackReadByte();
	m_regs.p.u = true;
	return 0;
}

uint8_t MOS6500::PHA()
{
	StackWriteByte(m_regs.a);
	return 0;
}

uint8_t MOS6500::PHP()
{
	m_regs.p.b = true;
	m_regs.p.u = true;
	StackWriteByte(StateByte());
	m_regs.p.b = false;
	m_regs.p.u = false;

	return 0;
}


// arithmetic

uint8_t MOS6500::ADC()
{
	uint16_t tmp = m_regs.a + FetchByte() + (m_regs.p.c ? 1 : 0);

	SetCarryNegativeZero(tmp);
	m_regs.p.v = ~((m_regs.a ^ m_cache) & (m_regs.a ^ tmp) & 128);
	m_regs.a = tmp & 255;

	return 1;
}

uint8_t MOS6500::DEC()
{
	uint8_t tmp = FetchByte() - 1;
	WriteByteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t MOS6500::DEX()
{
	SetNegativeZero(--m_regs.x);
	return 0;
}

uint8_t MOS6500::DEY()
{
	SetNegativeZero(--m_regs.y);
	return 0;
}

uint8_t MOS6500::INC()
{
	uint8_t tmp = FetchByte() + 1;
	WriteByteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t MOS6500::INX()
{
	SetNegativeZero(++m_regs.x);
	return 0;
}

uint8_t MOS6500::INY()
{
	SetNegativeZero(++m_regs.y);
	return 0;
}

uint8_t MOS6500::SBC()
{
	uint16_t value = FetchByte() ^ 255; // invert the value
	uint16_t tmp = m_regs.a + value + (m_regs.p.c ? 1 : 0);

	SetCarryNegativeZero(tmp);
	m_regs.p.v = (tmp ^ m_regs.a) & ((tmp ^ value) & 128);
	m_regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t MOS6500::AND()
{
	m_regs.a &= FetchByte();
	SetNegativeZero(m_regs.a);

	return 1;
}

uint8_t MOS6500::ORA()
{
	m_regs.a |= FetchByte();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6500::ROL()
{
	uint16_t tmp = (FetchByte() << 1) | (m_regs.p.c ? 1 : 0);
	SetCarryNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6500::ROR()
{
	FetchByte();
	uint16_t tmp = (m_cache << 7) | (m_cache >> 1);

	if (m_cache & 1) SEC();
	CheckAddressMode(tmp);
	return 0;
}

uint8_t MOS6500::ASL()
{
	uint16_t tmp = FetchByte() << 1;
	SetCarryNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6500::LSR()
{
	if (FetchByte() & 1) SEC();
	uint16_t tmp = m_cache >> 1;
	SetNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t MOS6500::EOR()
{
	m_regs.a ^= FetchByte();
	SetNegativeZero(m_regs.a);
	return 1;
}


// comparison

uint8_t MOS6500::CMP()
{
	uint16_t tmp = m_regs.a - FetchByte();

	if (m_regs.a >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6500::CPX()
{
	uint16_t tmp = m_regs.x - FetchByte();

	if (m_regs.x >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6500::CPY()
{
	uint16_t tmp = m_regs.y - FetchByte();

	if (m_regs.y >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t MOS6500::BIT()
{
	m_regs.p.z = ((m_regs.a & FetchByte()) & 255) == 0;
	m_regs.p.n = m_cache & 128;
	m_regs.p.v = m_cache & 64;
	return 0;
}


// jumping

uint8_t MOS6500::JMP()
{
	counter = lastAbsAddress;
	return 0;
}

uint8_t MOS6500::JSR()
{
	StackWriteAddress(counter);
	return JMP();
}


// loading/storing

uint8_t MOS6500::LDA()
{
	m_regs.a = FetchByte();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6500::LDX()
{
	m_regs.x = FetchByte();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t MOS6500::LDY()
{
	m_regs.y = FetchByte();
	SetNegativeZero(m_regs.y);
	return 1;
}

uint8_t MOS6500::STA()
{
	WriteByteToLastAddress(m_regs.a);
	return 0;
}

uint8_t MOS6500::STX()
{
	WriteByteToLastAddress(m_regs.x);
	return 0;
}

uint8_t MOS6500::STY()
{
	WriteByteToLastAddress(m_regs.y);
	return 0;
}


// transfers

uint8_t MOS6500::TAX()
{
	m_regs.x = m_regs.a;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t MOS6500::TAY()
{
	m_regs.y = m_regs.a;
	SetNegativeZero(m_regs.y);
	return 0;
}

uint8_t MOS6500::TSX()
{
	m_regs.x = stackPtr;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t MOS6500::TXA()
{
	m_regs.a = m_regs.x;
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t MOS6500::TXS()
{
	stackPtr = m_regs.x;
	return 0;
}

uint8_t MOS6500::TYA()
{
	m_regs.a = m_regs.y;
	SetNegativeZero(m_regs.a);
	return 0;
}


// illegals

uint8_t MOS6500::ALR()
{
	return AND() + LSR();
}

uint8_t MOS6500::ANC()
{
	uint8_t res = AND();
	SEC();
	return res;
}

uint8_t MOS6500::ANE()
{
	m_regs.a = Magic() & m_regs.x & FetchByte();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t MOS6500::ARR()
{
	return AND() + ADC() + ROR();
}

uint8_t MOS6500::DCP()
{
	return DEC() + CMP();
}

uint8_t MOS6500::ISC()
{
	return INC() + SBC();
}

uint8_t MOS6500::JAM()
{
	WriteByteToLastAddress(255);
	throw std::runtime_error(fmt::format("JAM encountered:\n\n{}\n", FrameInfo()));
	return 0;
}

uint8_t MOS6500::LAS()
{
	return LDA() + TSX();
}

uint8_t MOS6500::LAX()
{
	return LDA() + LDX();
}

uint8_t MOS6500::LXA()
{
	m_regs.x = m_regs.a = Magic() & FetchByte();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t MOS6500::NOP()
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

uint8_t MOS6500::RLA()
{
	return ROL() + AND();
}

uint8_t MOS6500::RRA()
{
	return ROR() + ADC();
}

uint8_t MOS6500::SAX()
{
	WriteByteToLastAddress(m_regs.a & m_regs.x);
	return 0;
}

uint8_t MOS6500::SBX()
{
	uint16_t tmp = m_regs.a - FetchByte();

	if (m_regs.a >= m_cache)
		SEC();

	SetNegativeZero(tmp);
	m_regs.x--;

	return 1;
}

uint8_t MOS6500::SHA()
{
	uint16_t addr = FetchAddress();
	WriteByte(addr, m_regs.a & m_regs.x & (Hi16(addr + 1)));
	return 0;
}

uint8_t MOS6500::SHX()
{
	uint16_t addr = FetchAddress();
	WriteByte(addr, m_regs.x & (Hi16(addr + 1)));
	return 0;
}

uint8_t MOS6500::SHY()
{
	uint16_t addr = FetchAddress();
	WriteByte(addr, m_regs.y & (Hi16(addr + 1)));
	return 0;
}

uint8_t MOS6500::SLO()
{
	return ASL() + ORA();
}

uint8_t MOS6500::SRE()
{
	return LSR() + EOR();
}

uint8_t MOS6500::TAS()
{
	stackPtr = m_regs.a & m_regs.x;
	return SHA();
}

uint8_t MOS6500::USBC()
{
	return SBC() + NOP();
}
