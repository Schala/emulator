#include <bit>
#include <format>
#include <stdexcept>

#include "devices.h"

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
	return m_ram[addr];
}

void Bus6502::Write(uint16_t addr, uint8_t data)
{
	m_ram[addr] = data;
}


// CPU6502

static const std::array<Opcode6502, 256> Ops = {
	// 0x
	{ 7, "brk", &CPU6502::Implied, &CPU6502::Break },
	{ 6, "ora", &CPU6502::IndirectX, &CPU6502::Or },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "slo", &CPU6502::IndirectX, &CPU6502::SLO },
	{ 2, "nop", &CPU6502::ZeroPage, &CPU6502::NOP },
	{ 3, "ora", &CPU6502::ZeroPage, &CPU6502::Or },
	{ 5, "asl", &CPU6502::ZeroPage, &CPU6502::ShiftLeft },
	{ 5, "slo", &CPU6502::ZeroPage, &CPU6502::SLO },
	{ 3, "php", &CPU6502::Implied, &CPU6502::PushState },
	{ 2, "ora", &CPU6502::Immediate, &CPU6502::Or },
	{ 2, "asl", &CPU6502::Implied, &CPU6502::ShiftLeft },
	{ 2, "anc", &CPU6502::Immediate, &CPU6502::ANC },
	{ 4, "nop", &CPU6502::Absolute, &CPU6502::NOP },
	{ 4, "ora", &CPU6502::Absolute, &CPU6502::Or },
	{ 6, "asl", &CPU6502::Absolute, &CPU6502::ShiftLeft },
	{ 6, "slo", &CPU6502::Absolute, &CPU6502::SLO },

	// 1x
	{ 2, "bpl", &CPU6502::Relative, &CPU6502::BranchPositive },
	{ 5, "ora", &CPU6502::IndirectY, &CPU6502::Or },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "slo", &CPU6502::IndirectY, &CPU6502::SLO },
	{ 4, "nop", &CPU6502::ZeroPageX, &CPU6502::NOP },
	{ 4, "ora", &CPU6502::ZeroPageX, &CPU6502::Or },
	{ 6, "asl", &CPU6502::ZeroPageX, &CPU6502::ShiftLeft },
	{ 6, "slo", &CPU6502::ZeroPageX, &CPU6502::SLO },
	{ 2, "clc", &CPU6502::Implied, &CPU6502::ClearCarry },
	{ 4, "ora", &CPU6502::AbsoluteY, &CPU6502::Or },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 7, "slo", &CPU6502::AbsoluteY, &CPU6502::SLO },
	{ 4, "nop", &CPU6502::AbsoluteX, &CPU6502::NOP },
	{ 4, "ora", &CPU6502::AbsoluteX, &CPU6502::Or },
	{ 7, "asl", &CPU6502::AbsoluteX, &CPU6502::ShiftLeft },
	{ 7, "slo", &CPU6502::AbsoluteX, &CPU6502::SLO },

	// 2x
	{ 6, "jsr", &CPU6502::Absolute, &CPU6502::SubroutineJump },
	{ 6, "and", &CPU6502::IndirectX, &CPU6502::And },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "rla", &CPU6502::IndirectX, &CPU6502::RLA },
	{ 3, "bit", &CPU6502::ZeroPage, &CPU6502::TestBit },
	{ 3, "and", &CPU6502::ZeroPage, &CPU6502::And },
	{ 5, "rol", &CPU6502::ZeroPage, &CPU6502::RotateLeft },
	{ 5, "rla", &CPU6502::ZeroPage, &CPU6502::RLA },
	{ 4, "plp", &CPU6502::Implied, &CPU6502::PopState },
	{ 2, "and", &CPU6502::Immediate, &CPU6502::And },
	{ 2, "rol", &CPU6502::Implied, &CPU6502::RotateLeft },
	{ 2, "anc", &CPU6502::Immediate, &CPU6502::ANC },
	{ 4, "bit", &CPU6502::Absolute, &CPU6502::TestBit },
	{ 4, "and", &CPU6502::Absolute, &CPU6502::And },
	{ 6, "rol", &CPU6502::Absolute, &CPU6502::RotateLeft },
	{ 6, "rla", &CPU6502::Absolute, &CPU6502::RLA },

	// 3x
	{ 2, "bmi", &CPU6502::Relative, &CPU6502::BranchNegative },
	{ 5, "and", &CPU6502::IndirectY, &CPU6502::And },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "rla", &CPU6502::IndirectY, &CPU6502::RLA },
	{ 4, "nop", &CPU6502::ZeroPageX, &CPU6502::NOP },
	{ 4, "and", &CPU6502::ZeroPageX, &CPU6502::And },
	{ 6, "rol", &CPU6502::ZeroPageX, &CPU6502::RotateLeft },
	{ 6, "rla", &CPU6502::ZeroPageX, &CPU6502::RLA },
	{ 2, "sec", &CPU6502::Implied, &CPU6502::SetCarry },
	{ 4, "and", &CPU6502::AbsoluteY, &CPU6502::And },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 7, "rla", &CPU6502::AbsoluteY, &CPU6502::RLA },
	{ 4, "nop", &CPU6502::AbsoluteX, &CPU6502::NOP },
	{ 4, "and", &CPU6502::AbsoluteX, &CPU6502::And },
	{ 7, "rol", &CPU6502::AbsoluteX, &CPU6502::RotateLeft },
	{ 7, "rla", &CPU6502::AbsoluteX, &CPU6502::RLA },

	// 4x
	{ 6, "rti", &CPU6502::Implied, &CPU6502::InterruptReturn },
	{ 6, "eor", &CPU6502::IndirectX, &CPU6502::Xor },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "sre", &CPU6502::IndirectX, &CPU6502::SRE },
	{ 3, "nop", &CPU6502::ZeroPage, &CPU6502::NOP },
	{ 3, "eor", &CPU6502::ZeroPage, &CPU6502::Xor },
	{ 5, "lsr", &CPU6502::ZeroPage, &CPU6502::ShiftRight },
	{ 5, "sre", &CPU6502::ZeroPage, &CPU6502::SRE },
	{ 3, "pha", &CPU6502::Implied, &CPU6502::PushAccumulator },
	{ 2, "eor", &CPU6502::Immediate, &CPU6502::Xor },
	{ 2, "lsr", &CPU6502::Implied, &CPU6502::ShiftRight },
	{ 2, "alr", &CPU6502::Absolute, &CPU6502::ALR },
	{ 3, "jmp", &CPU6502::Absolute, &CPU6502::Jump },
	{ 4, "eor", &CPU6502::Absolute, &CPU6502::Xor },
	{ 6, "lsr", &CPU6502::Absolute, &CPU6502::ShiftRight },
	{ 6, "sre", &CPU6502::Absolute, &CPU6502::SRE },

	// 5x
	{ 2, "bvc", &CPU6502::Relative, &CPU6502::BranchOverflowClear },
	{ 5, "eor", &CPU6502::IndirectY, &CPU6502::Xor },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "sre", &CPU6502::IndirectY, &CPU6502::SRE },
	{ 4, "nop", &CPU6502::ZeroPageX, &CPU6502::NOP },
	{ 4, "eor", &CPU6502::ZeroPageX, &CPU6502::Xor },
	{ 6, "lsr", &CPU6502::ZeroPageX, &CPU6502::ShiftRight },
	{ 6, "sre", &CPU6502::ZeroPageX, &CPU6502::SRE },
	{ 2, "cli", &CPU6502::Implied, &CPU6502::ClearDisableInterrupts },
	{ 4, "eor", &CPU6502::AbsoluteY, &CPU6502::Xor },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 7, "sre", &CPU6502::AbsoluteY, &CPU6502::SRE },
	{ 4, "nop", &CPU6502::AbsoluteX, &CPU6502::NOP },
	{ 4, "eor", &CPU6502::AbsoluteX, &CPU6502::Xor },
	{ 7, "lsr", &CPU6502::AbsoluteX, &CPU6502::ShiftRight },
	{ 7, "sre", &CPU6502::AbsoluteX, &CPU6502::SRE },

	// 6x
	{ 6, "rts", &CPU6502::Implied, &CPU6502::SubroutineReturn },
	{ 6, "adc", &CPU6502::IndirectX, &CPU6502::Add },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "rra", &CPU6502::IndirectX, &CPU6502::RRA },
	{ 3, "nop", &CPU6502::ZeroPage, &CPU6502::NOP },
	{ 3, "adc", &CPU6502::ZeroPage, &CPU6502::Add },
	{ 5, "ror", &CPU6502::ZeroPage, &CPU6502::RotateRight },
	{ 5, "rra", &CPU6502::ZeroPage, &CPU6502::RRA },
	{ 4, "pla", &CPU6502::Implied, &CPU6502::PopAccumulator },
	{ 2, "adc", &CPU6502::Immediate, &CPU6502::Add },
	{ 2, "ror", &CPU6502::Implied, &CPU6502::RotateRight },
	{ 2, "arr", &CPU6502::Immediate, &CPU6502::ARR },
	{ 5, "jmp", &CPU6502::Indirect, &CPU6502::Jump },
	{ 4, "adc", &CPU6502::Absolute, &CPU6502::Add },
	{ 6, "ror", &CPU6502::Absolute, &CPU6502::RotateRight },
	{ 6, "rra", &CPU6502::Absolute, &CPU6502::RRA },

	// 7x
	{ 2, "bvs", &CPU6502::Relative, &CPU6502::BranchOverflowSet },
	{ 5, "adc", &CPU6502::IndirectY, &CPU6502::Add },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "rra", &CPU6502::IndirectY, &CPU6502::RRA },
	{ 4, "nop", &CPU6502::ZeroPageX, &CPU6502::NOP },
	{ 4, "adc", &CPU6502::ZeroPageX, &CPU6502::Add },
	{ 6, "ror", &CPU6502::ZeroPageX, &CPU6502::RotateRight },
	{ 6, "rra", &CPU6502::ZeroPageX, &CPU6502::RRA },
	{ 2, "sei", &CPU6502::Implied, &CPU6502::SetDisableInterrupts },
	{ 4, "adc", &CPU6502::AbsoluteY, &CPU6502::Add },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 7, "rra", &CPU6502::AbsoluteY, &CPU6502::RRA },
	{ 4, "nop", &CPU6502::AbsoluteX, &CPU6502::NOP },
	{ 4, "adc", &CPU6502::AbsoluteX, &CPU6502::Add },
	{ 7, "ror", &CPU6502::AbsoluteX, &CPU6502::RotateRight },
	{ 7, "rra", &CPU6502::AbsoluteX, &CPU6502::RRA },

	// 8x
	{ 2, "nop", &CPU6502::Immediate, &CPU6502::NOP },
	{ 6, "sta", &CPU6502::IndirectX, &CPU6502::StoreAccumulator },
	{ 2, "nop", &CPU6502::Immediate, &CPU6502::NOP },
	{ 6, "sax", &CPU6502::IndirectX, &CPU6502::SAX },
	{ 3, "sty", &CPU6502::ZeroPage, &CPU6502::StoreY },
	{ 3, "sta", &CPU6502::ZeroPage, &CPU6502::StoreAccumulator },
	{ 3, "stx", &CPU6502::ZeroPage, &CPU6502::StoreX },
	{ 3, "sax", &CPU6502::ZeroPage, &CPU6502::SAX },
	{ 2, "dey", &CPU6502::Implied, &CPU6502::DecrementY },
	{ 2, "nop", &CPU6502::Immediate, &CPU6502::NOP },
	{ 2, "txa", &CPU6502::Implied, &CPU6502::XToAccumulator },
	{ 2, "ane", &CPU6502::Immediate, &CPU6502::ANE },
	{ 4, "sty", &CPU6502::Absolute, &CPU6502::StoreY },
	{ 4, "sta", &CPU6502::Absolute, &CPU6502::StoreAccumulator },
	{ 4, "stx", &CPU6502::Absolute, &CPU6502::StoreX },
	{ 4, "sax", &CPU6502::Absolute, &CPU6502::SAX },

	// 9x
	{ 2, "bcc", &CPU6502::Relative, &CPU6502::BranchCarryClear },
	{ 6, "sta", &CPU6502::IndirectY, &CPU6502::StoreAccumulator },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 6, "sha", &CPU6502::IndirectY, &CPU6502::SHA },
	{ 4, "sty", &CPU6502::ZeroPageX, &CPU6502::StoreY },
	{ 4, "sta", &CPU6502::ZeroPageX, &CPU6502::StoreAccumulator },
	{ 4, "stx", &CPU6502::ZeroPagey, &CPU6502::StoreX },
	{ 4, "sax", &CPU6502::ZeroPagey, &CPU6502::SAX },
	{ 2, "tya", &CPU6502::Implied, &CPU6502::YToAccumulator },
	{ 5, "sta", &CPU6502::AbsoluteY, &CPU6502::StoreAccumulator },
	{ 2, "txs", &CPU6502::Implied, &CPU6502::XToStackPtr },
	{ 5, "tas", &CPU6502::AbsoluteY, &CPU6502::TAS },
	{ 5, "shy", &CPU6502::AbsoluteX, &CPU6502::SHY },
	{ 5, "sta", &CPU6502::AbsoluteX, &CPU6502::StoreAccumulator },
	{ 6, "shx", &CPU6502::AbsoluteY, &CPU6502::SHX },
	{ 5, "sha", &CPU6502::AbsoluteY, &CPU6502::SHA },

	// Ax
	{ 2, "ldy", &CPU6502::Immediate, &CPU6502::LoadY },
	{ 6, "lda", &CPU6502::IndirectX, &CPU6502::LoadAccumulator },
	{ 2, "ldx", &CPU6502::Immediate, &CPU6502::LoadX },
	{ 6, "lax", &CPU6502::IndirectX, &CPU6502::LAX },
	{ 3, "ldy", &CPU6502::ZeroPage, &CPU6502::LoadY },
	{ 3, "lda", &CPU6502::ZeroPage, &CPU6502::LoadAccumulator },
	{ 3, "ldx", &CPU6502::ZeroPage, &CPU6502::LoadX },
	{ 3, "lax", &CPU6502::ZeroPage, &CPU6502::LAX },
	{ 2, "tay", &CPU6502::Implied, &CPU6502::AccumulatorToY },
	{ 2, "lda", &CPU6502::Immediate, &CPU6502::LoadAccumulator },
	{ 2, "tax", &CPU6502::Implied, &CPU6502::AccumulatorToX },
	{ 2, "lxa", &CPU6502::Immediate, &CPU6502::LXA },
	{ 4, "ldy", &CPU6502::Absolute, &CPU6502::LoadY },
	{ 4, "lda", &CPU6502::Absolute, &CPU6502::LoadAccumulator },
	{ 4, "ldx", &CPU6502::Absolute, &CPU6502::LoadX },
	{ 4, "lax", &CPU6502::Absolute, &CPU6502::LAX },

	// Bx
	{ 2, "bcs", &CPU6502::Relative, &CPU6502::BranchCarrySet },
	{ 5, "lda", &CPU6502::IndirectY, &CPU6502::LoadAccumulator },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 5, "lax", &CPU6502::IndirectY, &CPU6502::LAX },
	{ 4, "ldy", &CPU6502::ZeroPageX, &CPU6502::LoadY },
	{ 4, "lda", &CPU6502::ZeroPageX, &CPU6502::LoadAccumulator },
	{ 4, "ldx", &CPU6502::ZeroPagey, &CPU6502::LoadX },
	{ 4, "lax", &CPU6502::ZeroPagey, &CPU6502::LAX },
	{ 2, "clv", &CPU6502::Implied, &CPU6502::ClearOverflow },
	{ 4, "lda", &CPU6502::AbsoluteY, &CPU6502::LoadAccumulator },
	{ 2, "tsx", &CPU6502::Implied, &CPU6502::StackPtrToX },
	{ 4, "las", &CPU6502::AbsoluteY, &CPU6502::LAS },
	{ 4, "ldy", &CPU6502::AbsoluteX, &CPU6502::LoadY },
	{ 4, "lda", &CPU6502::AbsoluteX, &CPU6502::LoadAccumulator },
	{ 4, "ldx", &CPU6502::AbsoluteY, &CPU6502::LoadX },
	{ 4, "lax", &CPU6502::AbsoluteY, &CPU6502::LAX },

	// Cx
	{ 2, "cpy", &CPU6502::Immediate, &CPU6502::CompareY },
	{ 6, "cmp", &CPU6502::IndirectX, &CPU6502::Compare },
	{ 2, "nop", &CPU6502::Immediate, &CPU6502::NOP },
	{ 8, "dcp", &CPU6502::IndirectX, &CPU6502::DCP },
	{ 3, "cpy", &CPU6502::ZeroPage, &CPU6502::CompareY },
	{ 3, "cmp", &CPU6502::ZeroPage, &CPU6502::Compare },
	{ 5, "dec", &CPU6502::ZeroPage, &CPU6502::Decrement },
	{ 5, "dcp", &CPU6502::ZeroPage, &CPU6502::DCP },
	{ 2, "iny", &CPU6502::Implied, &CPU6502::IncrementY },
	{ 2, "cmp", &CPU6502::Immediate, &CPU6502::Compare },
	{ 2, "dex", &CPU6502::Implied, &CPU6502::DecrementX },
	{ 2, "sbx", &CPU6502::Immediate, &CPU6502::SBX },
	{ 4, "cpy", &CPU6502::Absolute, &CPU6502::CompareY },
	{ 4, "cmp", &CPU6502::Absolute, &CPU6502::Compare },
	{ 6, "dec", &CPU6502::Absolute, &CPU6502::Decrement },
	{ 6, "dcp", &CPU6502::Absolute, &CPU6502::DCP },

	// Dx
	{ 2, "bne", &CPU6502::Relative, &CPU6502::BranchNotEqual },
	{ 5, "cmp", &CPU6502::IndirectY, &CPU6502::Compare },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 8, "dcp", &CPU6502::IndirectY, &CPU6502::DCP },
	{ 4, "nop", &CPU6502::ZeroPageX, &CPU6502::NOP },
	{ 4, "cmp", &CPU6502::ZeroPageX, &CPU6502::Compare },
	{ 6, "dec", &CPU6502::ZeroPageX, &CPU6502::Decrement },
	{ 6, "dcp", &CPU6502::ZeroPageX, &CPU6502::DCP },
	{ 2, "cld", &CPU6502::Implied, &CPU6502::ClearDecimal },
	{ 4, "cmp", &CPU6502::AbsoluteY, &CPU6502::Compare },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 7, "dcp", &CPU6502::AbsoluteY, &CPU6502::DCP },
	{ 4, "nop", &CPU6502::AbsoluteX, &CPU6502::NOP },
	{ 4, "cmp", &CPU6502::AbsoluteX, &CPU6502::Compare },
	{ 7, "dec", &CPU6502::AbsoluteX, &CPU6502::Decrement },
	{ 7, "dcp", &CPU6502::AbsoluteX, &CPU6502::DCP },

	// Ex
	{ 2, "cpx", &CPU6502::Immediate, &CPU6502::CompareX },
	{ 6, "sbc", &CPU6502::IndirectX, &CPU6502::Subtract },
	{ 2, "nop", &CPU6502::Immediate, &CPU6502::NOP },
	{ 8, "isc", &CPU6502::IndirectX, &CPU6502::ISC },
	{ 3, "cpx", &CPU6502::ZeroPage, &CPU6502::CompareX },
	{ 3, "sbc", &CPU6502::ZeroPage, &CPU6502::Subtract },
	{ 5, "inc", &CPU6502::ZeroPage, &CPU6502::Increment },
	{ 5, "isc", &CPU6502::ZeroPage, &CPU6502::ISC },
	{ 2, "inx", &CPU6502::Implied, &CPU6502::IncrementX },
	{ 2, "sbc", &CPU6502::Immediate, &CPU6502::Subtract },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 2, "usbc", &CPU6502::Immediate, &CPU6502::USBC },
	{ 4, "cpx", &CPU6502::Absolute, &CPU6502::CompareX },
	{ 4, "sbc", &CPU6502::Absolute, &CPU6502::Subtract },
	{ 6, "inc", &CPU6502::Absolute, &CPU6502::Increment },
	{ 6, "isc", &CPU6502::Absolute, &CPU6502::ISC },

	// Fx
	{ 2, "beq", &CPU6502::Relative, &CPU6502::BranchEqual },
	{ 5, "sbc", &CPU6502::IndirectY, &CPU6502::Subtract },
	{ 0, "jam", nullptr, &CPU6502::JAM },
	{ 4, "isc", &CPU6502::IndirectY, &CPU6502::ISC },
	{ 4, "nop", &CPU6502::ZeroPageX, &CPU6502::NOP },
	{ 4, "sbc", &CPU6502::ZeroPageX, &CPU6502::Subtract },
	{ 6, "inc", &CPU6502::ZeroPageX, &CPU6502::Increment },
	{ 6, "isc", &CPU6502::ZeroPageX, &CPU6502::ISC },
	{ 2, "sed", &CPU6502::Implied, &CPU6502::SetDecimal },
	{ 4, "sbc", &CPU6502::AbsoluteY, &CPU6502::Subtract },
	{ 2, "nop", &CPU6502::Implied, &CPU6502::NOP },
	{ 7, "isc", &CPU6502::AbsoluteY, &CPU6502::ISC },
	{ 4, "nop", &CPU6502::AbsoluteX, &CPU6502::NOP },
	{ 4, "sbc", &CPU6502::AbsoluteX, &CPU6502::Subtract },
	{ 7, "inc", &CPU6502::AbsoluteX, &CPU6502::Increment },
	{ 7, "isc", &CPU6502::AbsoluteX, &CPU6502::ISC }
};

CPU6502::CPU6502(Bus6502 *bus, uint16_t startAddr, uint16_t endAddr):
	Device(dynamic_cast<Bus *>(bus), startAddr, endAddr),
	m_cache(0),
	m_cycles(0),
	m_lastOp(0),
	m_lastAbsAddr(0),
	m_lastRelAddr(0),
{
	m_regs.a = m_regs.x = m_regs.y = m_regs.pc = 0;
	m_regs.sp = StackPtrInitAddress;
	InitializeState();
}

void CPU6502::Branch()
{
	m_cycles++;
	m_lastAbsAddr = m_regs.pc + m_lastRelAddr;

	// need an additional cycle if different page
	if (Hi16(m_lastAbsAddr) != Hi16(m_regs.pc))
		m_cycles++;

	// jump to address
	m_regs.pc = m_lastAbsAddr;
}

void CPU6502::CheckAddressMode(uint16_t value)
{
	if (Ops[m_lastOp].AddressMode == &CPU6502::Implied)
		m_regs.a = value & 255;
	else
		WriteToLastAddress(value & 255);
}

void CPU6502::Clock()
{
	if (m_cycles == 0)
	{
		// get and increment the counter
		m_lastOp = ReadROM();

		// set cycles, see if any additional cycles are needed
		m_cycles = Ops[m_lastOp].Cycles;
		m_cycles += Ops[m_lastOp].AddressMode(this);
		m_cycles += Ops[m_lastOp].Operation(this);
	}

	m_cycles--;
}

void CPU6502::Disassemble(uint16_t startAddr, uint16_t endAddr)
{
	m_disasm.clear(); // dump any old disassembly

	uint16_t addr = startAddr; // for readability

	while (addr <= endAddr)
	{
		Disassembly6502 disasm;
		disasm.Address = addr;

		Opcode6502 &op = Ops[Read(addr++)];

		std::string line = op.Label;

		if (op.AddressMode == CPU6502::Immediate)
			line += std::format(" #${:02X}", Read(addr++));
		else if (op.AddressMode == CPU6502::ZeroPage)
			line += std::format(" ${:02X}", Read(addr++));
		else if (op.AddressMode == CPU6502::ZeroPageX)
			line += std::format(" ${:02X}, x", Read(addr++));
		else if (op.AddressMode == CPU6502::ZeroPageY)
			line += std::format(" ${:02X}, y", Read(addr++));
		else if (op.AddressMode == CPU6502::IndirectX)
			line += std::format(" (${:02X}, x)", Read(addr++));
		else if (op.AddressMode == CPU6502::IndirectY)
			line += std::format(" (${:02X}, y)", Read(addr++));
		else if (op.AddressMode == CPU6502::Absolute)
		{
			line += std::format(" ${:04X}", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == CPU6502::AbsoluteX)
		{
			line += std::format(" ${:04X}, x", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == CPU6502::AbsoluteY)
		{
			line += std::format(" ${:04X}, y", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == CPU6502::Indirect)
		{
			line += std::format(" (${:04X})", Read(addr));
			addr += 2;
		}
		else if (op.AddressMode == CPU6502::Relative)
		{
			uint8_t value = Read(addr++);
			line += std::format(" ${:02X} [${:04X}]", value, addr + static_cast<int8_t>(value));
		}
		else continue; // 'implied' takes no operands

		disasm.Line = line;
		m_disasm.push_back(disasm);
	}
}

uint8_t CPU6502::Fetch()
{
	if (Ops[m_lastOp].AddressMode != &CPU6502::Implied)
		m_cache = ReadFromLastAddress();
	return m_cache;
}

uint16_t CPU6502::FetchAddress()
{
	return Fetch() | (Fetch() << 8);
}

std::string CPU6502::FrameInfo() const
{
	std::string s = std::format("A: {:02X}\tX: {:02X}\tY: {:02X}\n", m_regs.a, m_regs.x, m_regs.y);
	s += std::format("SP: ${:02X}\tPC: ${:04X}\nState: ", m_regs.sp, m_regs.pc);

	s += m_regs.state.c ? 'C' : 'x';
	s += m_regs.state.z ? 'Z' : 'x';
	s += m_regs.state.i ? 'I' : 'x';
	s += m_regs.state.d ? 'D' : 'x';
	s += m_regs.state.b ? 'B' : 'x';
	s += m_regs.state.u ? 'U' : 'x';
	s += m_regs.state.v ? 'V' : 'x';
	s += m_regs.state.n ? 'N' : 'x';

	s += std::format("\n\nLast absolute address: ${:04X}\n", m_lastAbsAddr);
	s += std::format("Last relative address: ${:02X}\n", m_lastRelAddr);
	s += std::format("Last fetched byte: {:02X}\n", m_cache);
	s += std::format("Last operation: {}\n", Ops[m_lastOp].Label);
	s += std::format("Cycles remaining: {}\n", m_cycles);

	return s;
}

void CPU6502::InitializeState()
{
	ClearCarry();
	ClearDecimal();
	ClearDisableInterrupts();
	ClearOverflow();
	m_regs.state.z = m_regs.state.b = m_regs.state.n = false;
	m_regs.state.u = true;
}

void CPU6502::Interrupt(uint16_t newAbsAddr, uint8_t newCycles)
{
	// write the counter's current value to stack
	StackWriteAddress(m_regs.pc);

	// set and write state register to stack too
	m_regs.state.b = false;
	m_regs.state.i = m_regs.state.u = true;
	StackWrite(StateByte());

	// get a new counter
	m_lastAbsAddr = newAbsAddr;
	m_regs.pc = FetchAddress();

	m_cycles = newCycles;
}

uint8_t CPU6502::Magic() const
{
	return m_regs.a | MagicValue;
}

uint8_t CPU6502::Read(uint16_t addr) const
{
	return dynamic_cast<Bus6502 *>(m_bus)->Read(addr);
}

uint16_t CPU6502::ReadAddress(uint16_t addr) const
{
	return Read(addr) | (Read(addr + 1) << 8);
}

uint8_t CPU6502::ReadFromLastAddress() const
{
	return ReadAddress(m_lastAbsAddr);
}

uint8_t CPU6502::ReadROM()
{
	return Read(m_regs.pc++);
}

uint16_t CPU6502::ReadROMAddress()
{
	return ReadROM() | (ReadROM() << 8);
}

void CPU6502::Reset()
{
	m_cache = m_lastRelAddr = m_regs.a = m_regs.x = m_regs.y = m_regs.pc = 0;
	m_regs.sp = StackPtrInitAddress;
	InitializeState();

	m_lastAbsAddr = ResetAddress;
	m_regs.pc = FetchAddress();

	m_lastAbsAddr = 0;
	m_cycles = 8;
}

void CPU6502::SetCarryNegativeZero(uint16_t value)
{
	m_regs.state.c = value > 255;
	SetNegativeZero(value);
}

void CPU6502::SetNegativeZero(uint16_t value)
{
	m_regs.state.z = (value & 255) == 0;
	m_regs.state.n = value & 128;
}

uint8_t CPU6502::StackRead()
{
	return Read(StackBaseAddress + ++m_regs.sp);
}

uint16_t CPU6502::StackReadAddress()
{
	return StackRead() | (StackRead() << 8);
}

void CPU6502::StackWrite(uint8_t data)
{
	Write(StackBaseAddress + m_regs.sp--, data);
}

void CPU6502::StackWriteAddress(uint16_t addr)
{
	StackWrite(addr & 255);
	StackWrite((addr >> 8) & 255);
}

uint8_t CPU6502::StateByte() const
{
	return std::bit_cast(m_regs.state);
}

void CPU6502::Write(uint16_t addr, uint8_t data)
{
	dynamic_cast<Bus6502 *>(m_bus)->Write(addr, data);
}

uint16_t CPU6502::WriteToFetchedAddress(uint8_t data)
{
	uint16_t addr = FetchAddress();
	Write(addr, data);
	return addr;
}


// address modes

uint8_t CPU6502::Absolute()
{
	m_lastAbsAddr = ReadROMAddress();
	return 0;
}

uint8_t CPU6502::AbsoluteX()
{
	uint16_t addr = ReadROMAddress() + m_regs.x;
	return Hi16(m_lastAbsAddr) == Hi16(addr) ? 0 : 1;
}

uint8_t CPU6502::AbsoluteY()
{
	uint16_t addr = ReadROMAddress() + m_regs.y;
	return Hi16(m_lastAbsAddr) == Hi16(addr) ? 0 : 1;
}

uint8_t CPU6502::Immediate()
{
	m_lastAbsAddr = ++m_regs.pc;
	return 0;
}

uint8_t CPU6502::Implied()
{
	m_cache = m_regs.a;
	return 0;
}

uint8_t CPU6502::Indirect()
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

uint8_t CPU6502::IndirectX()
{
	m_lastAbsAddr = ReadAddress((ReadROMAddress() + m_regs.x) & 255);
	return 0;
}

uint8_t CPU6502::IndirectY()
{
	uint16_t t = ReadROMAddress();
	uint16_t lo = Read(t & 255);
	uint16_t hi = Read((t + 1) & 255);

	m_lastAbsAddr = (hi << 8) | lo + m_regs.y;

	return Hi16(m_lastAbsAddr) != hi << 8 ? 1 : 0;
}

uint8_t CPU6502::Relative()
{
	m_lastRelAddr = ReadROM();

	// check for signed bit
	if (m_lastRelAddr & 128)
		m_lastRelAddr |= 0xFF00;

	return 0;
}

uint8_t CPU6502::ZeroPage()
{
	m_lastAbsAddr = ReadROM();
	return 0;
}

uint8_t CPU6502::ZeroPageX()
{
	m_lastRelAddr = (ReadROM() + m_regs.x) & 255;
	return 0;
}

uint8_t CPU6502::ZeroPageY()
{
	m_lastRelAddr = (ReadROM() + m_regs.y) & 255;
	return 0;
}


// branching

uint8_t CPU6502::BranchCarryClear()
{
	if (!m_regs.state.c)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchCarrySet()
{
	if (m_regs.state.c)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchEqual()
{
	if (m_regs.state.z)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchNegative()
{
	if (m_regs.state.n)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchNotEqual()
{
	if (!m_regs.state.z)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchOverflowClear()
{
	if (!m_regs.state.v)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchOverflowSet()
{
	if (m_regs.state.v)
		Branch();
	return 0;
}

uint8_t CPU6502::BranchPositive()
{
	if (m_regs.state.n)
		Branch();
	return 0;
}


// state manipulation

uint8_t CPU6502::ClearCarry()
{
	m_regs.state.c = false;
	return 0;
}

uint8_t CPU6502::ClearDecimal()
{
	m_regs.state.d = false;
	return 0;
}

uint8_t CPU6502::ClearDisableInterrupts()
{
	m_regs.state.i = false;
	return 0;
}

uint8_t CPU6502::ClearOverflow()
{
	m_regs.state.v = false;
	return 0;
}

uint8_t CPU6502::SetCarry()
{
	m_regs.state.c = true;
	return 0;
}

uint8_t CPU6502::SetDecimal()
{
	m_regs.state.d = true;
	return 0;
}

uint8_t CPU6502::SetDisableInterrupts()
{
	m_regs.state.i = true;
	return 0;
}


// interrupts

uint8_t CPU6502::Break()
{
	SetDisableInterrupts();
	StackWriteAddress(++m_regs.pc);

	m_regs.state.b = true;
	StackWrite(StateByte());
	m_regs.state.b = false;

	m_regs.pc = ReadAddress(InterruptRequestAddress);

	return 0;
}

uint8_t CPU6502::InterruptRequest()
{
	if (!m_regs.state.i)
		Interrupt(InterruptRequestAddress, 7);
	return 0;
}

uint8_t CPU6502::InterruptReturn()
{
	uint8_t bits = StackRead();

	// restore state
	m_regs.state = std::bit_cast(bits);
	m_regs.state.b &= ~m_regs.state.b;
	m_regs.state.u &= ~m_regs.state.u;

	// and the counter
	m_regs.pc = StackReadAddress();

	return 0;
}

uint8_t CPU6502::NonMaskableInterrupt()
{
	Interrupt(NonMaskableInterruptAddress, 8);
	return 0;
}

uint8_t CPU6502::SubroutineReturn()
{
	m_regs.pc = StackReadAddress();
	return 0;
}


// stack manipulation

uint8_t CPU6502::PopAccumulator()
{
	m_regs.a = StackRead();
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t CPU6502::PopState()
{
	StackRead();
	m_regs.flags.u = true;
	return 0;
}

uint8_t CPU6502::PushAccumulator()
{
	StackWrite(m_regs.a);
	return 0;
}

uint8_t CPU6502::PushState()
{
	m_regs.state.b = true;
	m_regs.state.u = true;
	StackWrite(StateByte());
	m_regs.state.b = false;
	m_regs.state.u = false;

	return 0;
}


// arithmetic

uint8_t CPU6502::Add()
{
	uint16_t tmp = m_regs.a + Fetch() + (m_regs.state.c ? 1 : 0);

	SetCarryNegativeZero(tmp);
	m_regs.state.v = ~((m_regs.a ^ m_cache) & (m_regs.a ^ tmp) & 128);
	m_regs.a = tmp & 255;

	return 1;
}

uint8_t CPU6502::Decrement()
{
	uint8_t tmp = Fetch() - 1;
	WriteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t CPU6502::DecrementX()
{
	SetNegativeZero(--m_regs.x);
	return 0;
}

uint8_t CPU6502::DecrementY()
{
	SetNegativeZero(--m_regs.y);
	return 0;
}

uint8_t CPU6502::Increment()
{
	uint8_t tmp = Fetch() + 1;
	WriteToLastAddress(tmp);
	SetNegativeZero(m_regs.x);

	return 0;
}

uint8_t CPU6502::IncrementX()
{
	SetNegativeZero(++m_regs.x);
	return 0;
}

uint8_t CPU6502::IncrementY()
{
	SetNegativeZero(++m_regs.y);
	return 0;
}

uint8_t CPU6502::Subtract()
{
	uint16_t value = Fetch() ^ 255; // invert the value
	uint16_t tmp = m_regs.a + value + (m_regs.state.c ? 1 : 0);

	SetCarryNegativeZero(tmp);
	m_regs.state.v = (tmp ^ m_regs.a) & ((tmp ^ value) & 128);
	m_regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t CPU6502::And()
{
	m_regs.a &= Fetch();
	SetNegativeZero(m_regs.a);

	return 1;
}

uint8_t CPU6502::Or()
{
	m_regs.a |= Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t CPU6502::RotateLeft()
{
	uint16_t tmp = (Fetch() << 1) | (m_regs.state.c ? 1 : 0);
	SetCarryNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t CPU6502::RotateRight()
{
	Fetch();
	uint16_t tmp = (m_cache << 7) | (m_cache >> 1);

	if (m_cache & 1) SetCarry();
	CheckAddressMode();
	return 0;
}

uint8_t CPU6502::ShiftLeft()
{
	uint16_t tmp = Fetch() << 1;
	SetCarryNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t CPU6502::ShiftRight()
{
	if (Fetch() & 1) SetCarry();
	uint16_t tmp = m_cache >> 1;
	SetNegativeZero(tmp);
	CheckAddressMode(tmp);

	return 0;
}

uint8_t CPU6502::Xor()
{
	m_regs.a ^= Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}


// comparison

uint8_t CPU6502::Compare()
{
	uint16_t tmp = m_regs.a - Fetch();

	if (m_regs.a >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t CPU6502::CompareX()
{
	uint16_t tmp = m_regs.x - Fetch();

	if (m_regs.x >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t CPU6502::CompareY()
{
	uint16_t tmp = m_regs.y - Fetch();

	if (m_regs.y >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	return 1;
}

uint8_t CPU6502::TestBit()
{
	m_regs.state.z = ((m_regs.a & Fetch()) & 255) == 0;
	m_regs.state.n = m_cache & 128;
	m_regs.state.v = m_cache & 64;
	return 0;
}


// jumping

uint8_t CPU6502::Jump()
{
	m_regs.pc = m_lastAbsAddr;
	return 0;
}

uint8_t CPU6502::SubroutineJump()
{
	StackWriteAddress(m_regs.pc);
	return Jump();
}


// loading/storing

uint8_t CPU6502::LoadAccumulator()
{
	m_regs.a = Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t CPU6502::LoadX()
{
	m_regs.x = Fetch();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t CPU6502::LoadY()
{
	m_regs.y = Fetch();
	SetNegativeZero(m_regs.y);
	return 1;
}

uint8_t CPU6502::StoreAccumulator()
{
	WriteToLastAddress(m_regs.a);
	return 0;
}

uint8_t CPU6502::StoreX()
{
	WriteToLastAddress(m_regs.x);
	return 0;
}

uint8_t CPU6502::StoreY()
{
	WriteToLastAddress(m_regs.y);
	return 0;
}


// transfers

uint8_t CPU6502::AccumulatorToX()
{
	m_regs.x = m_regs.a;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t CPU6502::AccumulatorToY()
{
	m_regs.y = m_regs.a;
	SetNegativeZero(m_regs.y);
	return 0;
}

uint8_t CPU6502::StackPtrToX()
{
	m_regs.x = m_regs.sp;
	SetNegativeZero(m_regs.x);
	return 0;
}

uint8_t CPU6502::XToAccumulator()
{
	m_regs.a = m_regs.x;
	SetNegativeZero(m_regs.a);
	return 0;
}

uint8_t CPU6502::XToStackPtr()
{
	m_regs.sp = m_regs.x;
	return 0;
}

uint8_t CPU6502::YToAccumulator()
{
	m_regs.a = m_regs.y;
	SetNegativeZero(m_regs.a);
	return 0;
}


// illegals

uint8_t CPU6502::ALR()
{
	return And() + ShiftRight();
}

uint8_t CPU6502::ANC()
{
	uint8_t res = And();
	SetCarry();
	return res;
}

uint8_t CPU6502::ANE()
{
	m_regs.a = Magic() & m_regs.x & Fetch();
	SetNegativeZero(m_regs.a);
	return 1;
}

uint8_t CPU6502::ARR()
{
	return And() + Add() + RotateRight();
}

uint8_t CPU6502::DCP()
{
	return Decrement() + Compare();
}

uint8_t CPU6502::ISC()
{
	return Increment() + Subtract();
}

uint8_t CPU6502::JAM()
{
	WriteToLastAddress(255);
	throw std::runtime_error(std::format("JAM encountered:\n\n{}\n", FrameInfo()));
	return 0;
}

uint8_t CPU6502::LAS()
{
	return LoadAccumulator() + StackPtrToX();
}

uint8_t CPU6502::LAX()
{
	return LoadAccumulator() + LoadX();
}

uint8_t CPU6502::LXA()
{
	m_regs.x = m_regs.a = Magic() & Fetch();
	SetNegativeZero(m_regs.x);
	return 1;
}

uint8_t CPU6502::NOP() const
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

uint8_t CPU6502::RLA()
{
	return RotateLeft() + And();
}

uint8_t CPU6502::RRA()
{
	return RotateRight() + Add();
}

uint8_t CPU6502::SAX()
{
	WriteToLastAddress(m_regs.a & m_regs.x);
	return 0;
}

uint8_t CPU6502::SBX()
{
	uint16_t tmp = m_regs.a - Fetch();

	if (m_regs.a >= m_cache)
		SetCarry();

	SetNegativeZero(tmp);
	m_regs.x--;

	return 1;
}

uint8_t CPU6502::SHA()
{
	uint16_t addr = FetchAddress();
	Write(addr, m_regs.a & m_regs.x & (Hi16(addr + 1));
	return 0;
}

uint8_t CPU6502::SHX()
{
	uint16_t addr = FetchAddress();
	Write(addr, m_regs.x & (Hi16(addr + 1));
	return 0;
}

uint8_t CPU6502::SHY()
{
	uint16_t addr = FetchAddress();
	Write(addr, m_regs.y & (Hi16(addr + 1));
	return 0;
}

uint8_t CPU6502::SLO()
{
	return ShiftLeft() + Or();
}

uint8_t CPU6502::SRE()
{
	return ShiftRight() + Xor();
}

uint8_t CPU6502::TAS()
{
	m_regs.sp = m_regs.a & m_regs.x;
	return SHA();
}

uint8_t CPU6502::USBC()
{
	return Subtract() + NOP();
}
